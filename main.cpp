#include "dad_table_data.h"
#include "nacrs_table_data.h"
#include "generic_table_data.h"



template <class T>
void merge(const T& left, const T& right, generic_table_data& out)
{
	out.column_headers.clear();
	out.data.clear();

	unordered_map<string, size_t> header_count;

	for (size_t i = 0; i < left.column_headers.size(); i++)
		header_count[left.column_headers[i]]++;

	for (size_t i = 0; i < right.column_headers.size(); i++)
		header_count[right.column_headers[i]]++;

	for (auto ci = header_count.begin(); ci != header_count.end(); ci++)
	{
		size_t index_left = 0;
		size_t index_right = 0;

		if (false == left.get_index(ci->first, index_left) || false == right.get_index(ci->first, index_right))
		{
			//cout << "dropping " << ci->first << endl;
		}
		else
		{
			//cout << "merging " << ci->first << endl;

			vector<string> empty_vec;

			out.column_headers.push_back(ci->first);
			out.data.push_back(empty_vec);

			for (size_t i = 0; i < left.data[index_left].size(); i++)
				out.data[out.data.size() - 1].push_back(left.data[index_left][i]);

			for (size_t i = 0; i < right.data[index_right].size(); i++)
				out.data[out.data.size() - 1].push_back(right.data[index_right][i]);
		}
	}
}



int main(void)
{
	// Set up indicators for DAD and NACRS data
	vector<diagnosis_indicator> indicators;
	diagnosis_indicator d;

	d.diagnosis_name = "neutropenia";
	d.diagnosis_codes = { "D700" };
	indicators.push_back(d);

	d.diagnosis_name = "myocarditis";
	d.diagnosis_codes = { "I401", "I408", "I409", "I41", "I514" };
	indicators.push_back(d);

	d.diagnosis_name = "cardiomyopathy";
	d.diagnosis_codes = { "I420", "I421", "I422", "I423", "I424", "I425", "I427", "I428", "I429" };
	indicators.push_back(d);

	d.diagnosis_name = "schizoph";
	d.diagnosis_codes = { "F200", "F201", "F202", "F203", "F204", "F205", "F206", "F208", "F209" };
	indicators.push_back(d);

	d.diagnosis_name = "schizaff";
	d.diagnosis_codes = { "F250", "F251", "F258", "F259" };
	indicators.push_back(d);
	
	d.diagnosis_name = "bipolar";
	d.diagnosis_codes = { "F310", "F311", "F312", "F313", "F314", "F315", "F316", "F317", "F318", "F319" };
	indicators.push_back(d);

	d.diagnosis_name = "psychosis_non";
	d.diagnosis_codes = { "F28" };
	indicators.push_back(d);

	d.diagnosis_name = "psychosis_org";
	d.diagnosis_codes = { "F29" };
	indicators.push_back(d);
	
	d.diagnosis_name = "self_harm";
	d.diagnosis_codes = { "X60", "X61", "X62", "X63", "X64", "X65", "X66", "X67", "X68", "X69", "X70", "X71", "X72", "X73", "X74", "X75", "X76", "X77", "X78", "X79", "X80", "X81", "X82", "X83", "X84" };
	indicators.push_back(d);



	// Handle DAD data
	dad_table_data dtd0(indicators);
	dad_table_data dtd1(indicators);

	if (false == dtd0.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/dad_cohorts_08_18.csv"))
		return -1;

	dtd0.rename_column("GENDER_CODE", "female");
	dtd0.replace("female", "M", "0");
	dtd0.replace("female", "F", "1");
	dtd0.add_column("source_DAD", "1");
	dtd0.delete_column("age"); // redo age from scratch
	dtd0.calc_age("admission_date", "BIRTHDATE_TRUNCATED"); // redo age from scratch

	if (false == dtd1.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/dad_post_08_18.csv"))
		return -1;

	dtd1.calc_age("admission_date", "BIRTHDATE_TRUNCATED");
	dtd1.rename_column("BIRTHDATE_TRUNCATED", "birth_yr");
	dtd1.rename_column("GENDER_CODE", "female");
	dtd1.replace("female", "M", "0");
	dtd1.replace("female", "F", "1");
	dtd1.add_column("source_DAD", "1");



	// Merge DAD data, then clear the unneeded memory
	generic_table_data generic_out0;
	merge<dad_table_data>(dtd0, dtd1, generic_out0);
	dtd0.clear_memory();
	dtd1.clear_memory();



	// Handle NACRS data
	nacrs_table_data ntd0(indicators);
	nacrs_table_data ntd1(indicators);

	if (false == ntd0.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/nacrs_cohorts_08_18.csv"))
		return -1;
	
	ntd0.calc_age("DATE_OF_REGISTRATION", "BIRTHDATE_TRUNCATED");
	ntd0.rename_column("BIRTHDATE_TRUNCATED", "birth_yr");
	ntd0.rename_column("GENDER_CODE", "female");
	ntd0.replace("female", "M", "0");
	ntd0.replace("female", "F", "1");
	ntd0.add_column("source_DAD", "0");

	if (false == ntd1.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/nacrs_post_08_18.csv"))
		return -1;

	ntd1.calc_age("DATE_OF_REGISTRATION", "birthdate_truncated");
	ntd1.rename_column("birthdate_truncated", "birth_yr");
	ntd1.rename_column("GENDER_CODE", "female");
	ntd1.replace("female", "M", "0");
	ntd1.replace("female", "F", "1");
	ntd1.add_column("source_DAD", "0");



	// Merge NACRS data, then clear the unneeded memory
	generic_table_data generic_out1;
	merge<nacrs_table_data>(ntd0, ntd1, generic_out1);
	ntd0.clear_memory();
	ntd1.clear_memory();



	// Do final merge
	generic_table_data generic_out2;
	merge<generic_table_data>(generic_out0, generic_out1, generic_out2);
	generic_out0.clear_memory();
	generic_out1.clear_memory();



	// Polish the data a little bit
	generic_out2.rename_column("SUBMITTING_PROV_CODE", "province");
	generic_out2.rename_column("FISCAL_YEAR", "fiscal_yr");
	generic_out2.rename_column("URBAN_RURAL_REMOTE", "rural_unkn");
	generic_out2.replace("rural_unkn", "RURAL/REMOTE", "1");
	generic_out2.replace("rural_unkn", "UNK", "1");
	generic_out2.replace("rural_unkn", "URBAN", "0");

	generic_out2.delete_column("DEID_INST_CODE");
	generic_out2.delete_column("DEID_XFER_FROM_INST_CODE");
	generic_out2.delete_column("DEID_XFER_TO_INST_CODE");
	generic_out2.delete_column("case_id");

	// Sort by example
	vector<string> sorted_column_names = { "mbun", "province", "birth_yr", "age", "female",
											"rural_unkn", "fiscal_yr", "source_DAD", "schizoph", "schizaff",
											"bipolar", "psychosis_org", "psychosis_non", "self_harm", "myocarditis",
											"cardiomyopathy", "neutropenia" };

	generic_out2.sort_columns(sorted_column_names);

	// Make the variable names' case fit the requirement (for example: Mbun, Province, etc.)
	generic_out2.unify_column_names_case();

	if (false == generic_out2.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/shawn_aggregate.csv"))
		return -1;



	// It's all good
	return 0;
}


