#include "dad_table_data.h"
#include "nacrs_table_data.h"




int main(void)
{
	vector<diagnosis_indicator> indicators;
	diagnosis_indicator d;

	d.diagnosis_name = "Neutropenia";
	d.diagnosis_codes.push_back("D700");
	indicators.push_back(d);

	d.diagnosis_name = "Myocarditis";
	d.diagnosis_codes.clear();
	d.diagnosis_codes.push_back("I401");
	d.diagnosis_codes.push_back("I408");
	d.diagnosis_codes.push_back("I409");
	d.diagnosis_codes.push_back("I41");
	d.diagnosis_codes.push_back("I514");
	indicators.push_back(d);

	d.diagnosis_name = "Cardiomyopathy";
	d.diagnosis_codes.clear();
	d.diagnosis_codes.push_back("I420");
	d.diagnosis_codes.push_back("I421");
	d.diagnosis_codes.push_back("I422");
	d.diagnosis_codes.push_back("I423");
	d.diagnosis_codes.push_back("I424");
	d.diagnosis_codes.push_back("I425");
	d.diagnosis_codes.push_back("I427");
	d.diagnosis_codes.push_back("I428");
	d.diagnosis_codes.push_back("I429");
	indicators.push_back(d);



	// Handle DAD data
	dad_table_data dtd(indicators);

	// Use the buffer method to load the CSV file
	// This is a tiny bit faster than doing it line by line
	if (false == dtd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/dad_cohorts_08_18.csv"))
		return -1;

	cout << dtd.get_filename() << endl;
	cout << "Neutropenia count:    " << dtd.get_count("Neutropenia_Indicator") << endl;
	cout << "Myocarditis count:    " << dtd.get_count("Myocarditis_Indicator") << endl;
	cout << "Cardiomyopathy count: " << dtd.get_count("Cardiomyopathy_Indicator") << endl;
	cout << endl;

	if (false == dtd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/dad_cohorts_08_18_with_indicators.csv"))
		return -1;

	if (false == dtd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18.csv"))
		return -1;

	cout << dtd.get_filename() << endl;
	cout << "Neutropenia count:    " << dtd.get_count("Neutropenia_Indicator") << endl;
	cout << "Myocarditis count:    " << dtd.get_count("Myocarditis_Indicator") << endl;
	cout << "Cardiomyopathy count: " << dtd.get_count("Cardiomyopathy_Indicator") << endl;
	cout << endl;

	if (false == dtd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18_with_indicators.csv"))
		return -1;



	// Handle NACRS data
	nacrs_table_data ntd(indicators);

	if (false == ntd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_cohorts_08_18.csv"))
		return -1;

	cout << ntd.get_filename() << endl;
	cout << "Neutropenia count:    " << ntd.get_count("Neutropenia_Indicator") << endl;
	cout << "Myocarditis count:    " << ntd.get_count("Myocarditis_Indicator") << endl;
	cout << "Cardiomyopathy count: " << ntd.get_count("Cardiomyopathy_Indicator") << endl;
	cout << endl;

	if(false == ntd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_cohorts_08_18_with_indicators.csv"))
		return -1;

	if (false == ntd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18.csv"))
		return -1;

	cout << ntd.get_filename() << endl;
	cout << "Neutropenia count:    " << ntd.get_count("Neutropenia_Indicator") << endl;
	cout << "Myocarditis count:    " << ntd.get_count("Myocarditis_Indicator") << endl;
	cout << "Cardiomyopathy count: " << ntd.get_count("Cardiomyopathy_Indicator") << endl;
	cout << endl;

	if (false == ntd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18_with_indicators.csv"))
		return -1;



	// It's all good
	return 0;
}