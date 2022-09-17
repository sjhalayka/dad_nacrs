#include "dad_table_data.h"
#include "nacrs_table_data.h"



int main(void)
{
	// Handle DAD data
	dad_table_data dtd;

	// Use the buffer method to load the CSV file
	// This is a tiny bit faster than doing it line by line
	if (false == dtd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/dad_cohorts_08_18.csv"))
		return -1;

	cout << dtd.get_filename() << endl;
	cout << "Neutropenia count: " << dtd.get_neutropenia_count() << endl;
	cout << "Myocarditis count: " << dtd.get_myocarditis_count() << endl;
	cout << "Cardiomyopathy count: " << dtd.get_cardiomyopathy_count() << endl;
	cout << endl;

	if (false == dtd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/dad_cohorts_08_18_with_indicators.csv"))
		return -1;

	if (false == dtd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18.csv"))
		return -1;

	cout << dtd.get_filename() << endl;
	cout << "Neutropenia count: " << dtd.get_neutropenia_count() << endl;
	cout << "Myocarditis count: " << dtd.get_myocarditis_count() << endl;
	cout << "Cardiomyopathy count: " << dtd.get_cardiomyopathy_count() << endl;
	cout << endl;

	if (false == dtd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18_with_indicators.csv"))
		return -1;



	// Handle NACRS data
	nacrs_table_data ntd;

	if (false == ntd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_cohorts_08_18.csv"))
		return -1;

	cout << ntd.get_filename() << endl;
	cout << "Neutropenia count: " << ntd.get_neutropenia_count() << endl;
	cout << "Myocarditis count: " << ntd.get_myocarditis_count() << endl;
	cout << "Cardiomyopathy count: " << ntd.get_cardiomyopathy_count() << endl;
	cout << endl;

	if(false == ntd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_cohorts_08_18_with_indicators.csv"))
		return -1;

	if (false == ntd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18.csv"))
		return -1;

	cout << ntd.get_filename() << endl;
	cout << "Neutropenia count: " << ntd.get_neutropenia_count() << endl;
	cout << "Myocarditis count: " << ntd.get_myocarditis_count() << endl;
	cout << "Cardiomyopathy count: " << ntd.get_cardiomyopathy_count() << endl;
	cout << endl;

	if (false == ntd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18_with_indicators.csv"))
		return -1;



	// It's all good
	return 0;
}