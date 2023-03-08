#include "main.h"












int main(void)
{
	generic_table_data all, dad_nacrs_rows, npduis_rows;
	const size_t grace_period = 30;



	cout << "Reading" << endl;
	if (false == all.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Drug_spell_grouping/meds_for_consolidation.csv"))
		return -1;


	cout << "Splitting" << endl;
	split(all, dad_nacrs_rows, npduis_rows);


	cout << "Getting rows" << endl;
	vector<npduis_row> vn;

	size_t row_count = npduis_rows.get_row_count();

	for (size_t i = 0; i < row_count; i++)
		vn.push_back(npduis_rows.get_npduis_row(i));


	// Keep column headers
	npduis_rows.clear_rows();


	cout << "Sorting rows" << endl;
	sort(vn.begin(), vn.end());


	for (size_t i = 0; i < vn.size(); i++)
		if (vn[i].mbun == "1")
			cout << "  " << vn[i].mbun << " " << vn[i].episode_beg_dt << " " << vn[i].episode_end_dt << " " << vn[i].drug_code << endl;



	cout << "Culling rows" << endl;
	list<npduis_row> ln;

	for (size_t i = 0; i < vn.size(); i++)
		ln.push_back(vn[i]);

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

		if (curr->mbun == prev->mbun &&
			curr->drug_code == prev->drug_code &&
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

	vn.clear();

	for(list<npduis_row>::const_iterator ci = ln.begin(); ci != ln.end(); ci++)
		vn.push_back(*ci);

	ln.clear();
 
	for (size_t i = 0; i < vn.size(); i++)
		if (vn[i].episode_beg_dt == vn[i].episode_end_dt)
			add_days_to_date(vn[i].episode_beg_dt, grace_period, vn[i].episode_end_dt);

	for (size_t i = 0; i < vn.size(); i++)
		if(vn[i].mbun == "1")
			cout << "  " << vn[i].mbun << " " << vn[i].episode_beg_dt << " " << vn[i].episode_end_dt << " " << vn[i].drug_code << endl;




	// copy from vn to npduis_rows
	cout << "Copying to table" << endl;

	for (size_t i = 0; i < vn.size(); i++)
		npduis_rows.add_npduis_row(vn[i]);

	vn.clear();




	cout << "Merging" << endl;
	generic_table_data generic_out0;
	merge<generic_table_data>(npduis_rows, dad_nacrs_rows, generic_out0);

	cout << "Sorting columns" << endl;
	vector<string> sorted_column_names = {
		"mbun", "province", "age", "birth_yr", "female", "source_dad", "psychosis_non", "rural_unkn", "fiscal_yr", "neutropenia", "schizoph", "schizaff", "bipolar", "psychosis_org", "self_harm", "myocarditis", "cardiomyopathy", "episode_beg_dt", "episode_end_dt", "drug_code", "drug_desc"
	};

	generic_out0.sort_columns(sorted_column_names);


	// Save to file
	if (false == generic_out0.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Drug_spell_grouping/meds_for_consolidation_remerged.csv"))
		return -1;

	return 0;
}


