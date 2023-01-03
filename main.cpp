#include "main.h"


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

	cout << "Loading DAD cohorts data" << endl;

	if (false == dtd0.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/dad_cohorts_08_18.csv"))
		return -1;

	dtd0.delete_column("age"); // redo age from scratch
	dtd0.calc_age("admission_date", "BIRTHDATE_TRUNCATED"); // redo age from scratch
	dtd0.rename_column("BIRTHDATE_TRUNCATED", "birth_yr");
	dtd0.rename_column("GENDER_CODE", "female");
	dtd0.replace("female", "M", "0");
	dtd0.replace("female", "F", "1");
	dtd0.add_column("source_dad", "1");
	dtd0.rename_column("admission_date", "Episode_beg_dt");
	dtd0.rename_column("discharge_date", "Episode_end_dt");


	//dtd0.print_indicators();

	cout << "Loading DAD post data" << endl;

	if (false == dtd1.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/dad_post_08_18.csv"))
		return -1;

	dtd1.delete_column("age"); // redo age from scratch
	dtd1.calc_age("admission_date", "birthdate_truncated");
	dtd1.rename_column("birthdate_truncated", "birth_yr");
	dtd1.rename_column("GENDER_CODE", "female");
	dtd1.replace("female", "M", "0");
	dtd1.replace("female", "F", "1");
	dtd1.add_column("source_dad", "1");
	dtd1.rename_column("admission_date", "Episode_beg_dt");
	dtd1.rename_column("discharge_date", "Episode_end_dt");

	//dtd1.print_indicators();



	cout << "Merging DAD files" << endl;

	// Merge DAD data, then clear the unneeded memory
	generic_table_data generic_out0;
	merge<dad_table_data>(dtd0, dtd1, generic_out0);
	dtd0.clear_memory();
	dtd1.clear_memory();



	// Handle NACRS data
	nacrs_table_data ntd0(indicators);
	nacrs_table_data ntd1(indicators);

	cout << "Loading NACRS cohorts data" << endl;

	if (false == ntd0.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/nacrs_cohorts_08_18.csv"))
		return -1;

	ntd0.calc_age("DATE_OF_REGISTRATION", "BIRTHDATE_TRUNCATED");
	ntd0.rename_column("BIRTHDATE_TRUNCATED", "birth_yr");
	ntd0.rename_column("GENDER_CODE", "female");
	ntd0.replace("female", "M", "0");
	ntd0.replace("female", "F", "1");
	ntd0.add_column("source_dad", "0");
	ntd0.rename_column("DATE_OF_REGISTRATION", "Episode_beg_dt");
	ntd0.rename_column("DISPOSITION_DATE", "Episode_end_dt");


	cout << "Loading NACRS post data" << endl;

	if (false == ntd1.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/nacrs_post_08_18.csv"))
		return -1;

	ntd1.calc_age("DATE_OF_REGISTRATION", "birthdate_truncated");
	ntd1.rename_column("birthdate_truncated", "birth_yr");
	ntd1.rename_column("GENDER_CODE", "female");
	ntd1.replace("female", "M", "0");
	ntd1.replace("female", "F", "1");
	ntd1.add_column("source_dad", "0");
	ntd1.rename_column("DATE_OF_REGISTRATION", "Episode_beg_dt");
	ntd1.rename_column("DISPOSITION_DATE", "Episode_end_dt");

	
	cout << "Merging NACRS files" << endl;

	// Merge NACRS data, then clear the unneeded memory
	generic_table_data generic_out1;
	merge<nacrs_table_data>(ntd0, ntd1, generic_out1);
	ntd0.clear_memory();
	ntd1.clear_memory();



	cout << "Merging DAD/NACRS files" << endl;

	// Do final DAD/NACRS merge
	generic_table_data generic_out2;
	merge<generic_table_data>(generic_out0, generic_out1, generic_out2);
	generic_out0.clear_memory();
	generic_out1.clear_memory();

	cout << "Polishing data" << endl;

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

	generic_out2.unify_column_names_case();

	cout << "Sorting columns" << endl;

	// Sort by example
	vector<string> sorted_column_names = { "Mbun", "Province", "Birth_yr", "Age", "Female",
											"Rural_unkn", "Fiscal_yr", "Source_dad", "Schizoph", "Schizaff",
											"Bipolar", "Psychosis_org", "Psychosis_non", "Self_harm", "Myocarditis",
											"Cardiomyopathy", "Neutropenia", "Episode_beg_dt", "Episode_end_dt"};

	generic_out2.sort_columns(sorted_column_names);

	cout << "Saving to file" << endl;

	if (false == generic_out2.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/aggregate.csv"))
		return -1;




	//// Note, NPDUIS data already have
	//// mbun, province, fiscal_year, source_dad
	//// Let's add in the rest of the columns
	//npduis.add_column("birth_yr", "");
	//npduis.add_column("age", "");
	//npduis.add_column("female", "");
	//npduis.add_column("rural_unkn", "");
	//npduis.add_column("schizoph", "");
	//npduis.add_column("schizaff", "");
	//npduis.add_column("bipolar", "");
	//npduis.add_column("psychosis_org", "");
	//npduis.add_column("psychosis_non", "");
	//npduis.add_column("self_harm", "");
	//npduis.add_column("myocarditis", "");
	//npduis.add_column("cardiomyopathy", "");
	//npduis.add_column("neutropenia", "");
	//npduis.replace("province", "BC", "9");
	//npduis.replace("province", "Sask", "7");
	//npduis.replace("province", "Man", "6");
	//npduis.unify_column_names_case();
	//// Sort by example
	//vector<string> sorted_column_names = { "Mbun", "Province", "Birth_yr", "Age", "Female",
	//										"Rural_unkn", "Fiscal_yr", "Source_dad", "Schizoph", "Schizaff",
	//										"Bipolar", "Psychosis_org", "Psychosis_non", "Self_harm", "Myocarditis",
	//										"Cardiomyopathy", "Neutropenia" };

	//npduis.sort_columns(sorted_column_names);

	//if (false == npduis.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/aggregate.csv"))
	//	return -1;




	//generic_table_data generic_out3;
	//merge<generic_table_data>(generic_out2, npduis, generic_out3);
	//generic_out2.clear_memory();
	//npduis.clear_memory();

	//// Sort by example
	//vector<string> sorted_column_names = { "Mbun", "Province", "Birth_yr", "Age", "Female",
	//										"Rural_unkn", "Fiscal_yr", "Source_dad", "Schizoph", "Schizaff",
	//										"Bipolar", "Psychosis_org", "Psychosis_non", "Self_harm", "Myocarditis",
	//										"Cardiomyopathy", "Neutropenia" };

	//generic_out3.sort_columns(sorted_column_names);

	//if (false == generic_out3.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/shawn_aggregate.csv"))
	//	return -1;



	// It's all good
	return 0;
}


