#include "main.h"





void get_date_range(const string& start_date, const string& end_date, vector<string>& dates)
{
	tm ta = {}, tb = {};

	istringstream iss;

	iss.clear();
	iss.str(start_date);
	iss >> get_time(&ta, "%d%b%Y");

	iss.clear();
	iss.str(end_date);
	iss >> get_time(&tb, "%d%b%Y");

	if (date_less_than(tb, ta))
		swap(ta, tb);

	dates.clear();

	while (false == date_equals(ta, tb))
	{
		ostringstream oss;
		oss << put_time(&ta, "%d%b%Y");
		dates.push_back(oss.str());

		ta.tm_mday += 1;
		mktime(&ta);
	} 

	ostringstream oss;
	oss << put_time(&ta, "%d%b%Y");
	dates.push_back(oss.str());
}

class comp  //custom comparator
{
public:
	bool operator()(const tm &left, const tm &right) const 
	{
		if (right.tm_year > left.tm_year)
			return true;
		else if (right.tm_year < left.tm_year)
			return false;

		if (right.tm_mon > left.tm_mon)
			return true;
		else if (right.tm_mon < left.tm_mon)
			return false;

		if (right.tm_mday > left.tm_mday)
			return true;
		else if (right.tm_mday < left.tm_mday)
			return false;

		return false;
	}
};

void convert_rows_to_polypharmacy_rows(const vector<npduis_row>& vn, vector<npduis_row>& vout)
{
	vout.clear();

	const size_t grace_period = 30;

	map<tm, vector<npduis_row>, comp> date_map;

	for (size_t i = 0; i < vn.size(); i++)
	{
		vector<string> dates;
		get_date_range(vn[i].episode_beg_dt, vn[i].episode_end_dt, dates);

		for (size_t d = 0; d < dates.size(); d++)
		{
			tm ta = {};

			istringstream iss;
			iss.str(dates[d]);
			iss >> get_time(&ta, "%d%b%Y");

			//cout << dates[d] << " " << (ta.tm_year + 1900) << " " << (ta.tm_mon + 1) << " " << ta.tm_mday << endl;
			//cout << endl;

			npduis_row nr = vn[i];

			nr.episode_beg_dt = dates[d];
			nr.episode_end_dt = dates[d];

			date_map[ta].push_back(nr);
		}
	}

	for (map<tm, vector<npduis_row>, comp>::iterator ci = date_map.begin(); ci != date_map.end(); ci++)
	{
		if (ci->second.size() == 0)
		{
			// this should never happen
			continue;
		}
		else if (ci->second.size() == 1)
		{
			ci->second[0].drug_desc_grp = ci->second[0].drug_desc;
			ci->second[0].drug_set = ci->second[0].drug_code;
		}
		else
		{
			// polypharmacy
			size_t num_meds = ci->second.size();

			vector<string> drug_set_strings;

			for (size_t j = 0; j < num_meds; j++)
				drug_set_strings.push_back(ci->second[j].drug_code);

			sort(drug_set_strings.begin(), drug_set_strings.end());

			string drug_set = "";

			for (size_t j = 0; j < drug_set_strings.size(); j++)
			{
				drug_set += drug_set_strings[j];

				if (j != drug_set_strings.size() - 1)
					drug_set += ';';
			}

			ci->second[0].drug_desc_grp = "Polypharmacy";
			ci->second[0].drug_set = drug_set;

			ci->second.resize(1);
		}

		vout.push_back(ci->second[0]);
	}



	cout << "Culling rows" << endl;
	list<npduis_row> ln;

	for (size_t i = 0; i < vout.size(); i++)
		ln.push_back(vout[i]);

	list<npduis_row>::iterator curr = ln.begin();
	curr++;

	// culling rows
	for (; curr != ln.end(); )
	{
		list<npduis_row>::iterator prev = curr;
		prev--;

		string prev_end = prev->episode_end_dt;
		string curr_end = curr->episode_end_dt;

		string extended_prev_end;
		add_days_to_date(prev_end, grace_period + 1, extended_prev_end);

		tm ta = {}, tb = {};

		istringstream iss(curr_end);
		iss >> get_time(&ta, "%d%b%Y");

		iss.clear();
		iss.str(extended_prev_end);
		iss >> get_time(&tb, "%d%b%Y");


		if (curr->drug_desc_grp == prev->drug_desc_grp &&
			curr->drug_set == prev->drug_set &&
			date_less_than(ta, tb))
		{
			// Erase this row
			prev->episode_end_dt = curr->episode_beg_dt;
			curr = ln.erase(curr);
		}
		else
		{
			// Keep this row
			curr++;
		}
	
	}

	vout.clear();

	for(list<npduis_row>::const_iterator ci = ln.begin(); ci != ln.end(); ci++)
		vout.push_back(*ci);

	ln.clear();
}




int main(void)
{
	generic_table_data all, dad_nacrs_rows, npduis_rows;

	cout << "Reading" << endl;
	if (false == all.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Drug_spell_grouping/Agg_records_2023_medicated_for_consol_remerged_step1.csv"))
		return -1;



	cout << "Splitting" << endl;
	split(all, dad_nacrs_rows, npduis_rows);


	cout << "Getting rows" << endl;

	size_t row_count = npduis_rows.get_row_count();

	if (row_count == 0)
		return 0;

	size_t curr_mbun = stoi(npduis_rows.get_npduis_row(0).mbun);

	vector<npduis_row> vtemp;
	vector<npduis_row> all_final;

	for (size_t i = 0; i < row_count; i++)
	{
		npduis_row nr = npduis_rows.get_npduis_row(i);

		size_t this_mbun = stoi(nr.mbun);

		if (this_mbun != curr_mbun)
		{	
			if (vtemp.size() != 0)
			{
				vector<npduis_row> final;
				convert_rows_to_polypharmacy_rows(vtemp, final);

				for (size_t j = 0; j < final.size(); j++)
					all_final.push_back(final[j]);
			}

			vtemp.clear();
			curr_mbun = this_mbun;
		}

		vtemp.push_back(nr);
	}

	if (vtemp.size() != 0)
	{
		vector<npduis_row> final;
		convert_rows_to_polypharmacy_rows(vtemp, final);

		for (size_t j = 0; j < final.size(); j++)
			all_final.push_back(final[j]);
	}

	// Keep column headers
	npduis_rows.clear_rows();


	// copy from vn to npduis_rows
	cout << "Copying to table" << endl;

	for (size_t i = 0; i < all_final.size(); i++)
		npduis_rows.add_npduis_row(all_final[i]);

	all_final.clear();


	cout << "Merging" << endl;
	generic_table_data generic_out0;
	merge<generic_table_data>(npduis_rows, dad_nacrs_rows, generic_out0);

	npduis_rows.clear_memory();
	dad_nacrs_rows.clear_memory();



	cout << "Sorting columns" << endl;
	vector<string> sorted_column_names = {
		"mbun", "province", "age", "birth_yr", "female", 
		"source_dad", "psychosis_non", "rural_unkn", "fiscal_yr", "neutropenia", 
		"schizoph", "schizaff", "bipolar", "psychosis_org", "self_harm",
		"myocarditis", "cardiomyopathy", "episode_beg_dt", "episode_end_dt", 
		"drug_code", "drug_desc", "drug_desc_grp", "drug_set"
	};

	generic_out0.sort_columns(sorted_column_names);

	// Save to file
	if (false == generic_out0.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Drug_spell_grouping/Agg_records_2023_medicated_for_consol_remerged_step2.csv"))
		return -1;


	


	return 0;
}


