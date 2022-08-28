#include "dad_table_data.h"
#include "nacrs_table_data.h"


int main(void)
{
	std::chrono::high_resolution_clock::time_point start_time, end_time;
	start_time = std::chrono::high_resolution_clock::now();

	dad_table_data dtd;

	// "Z:/Smartphone_2/Shawn/Neutropenia/dad_08_18_cohorts.csv"
	// "Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18.csv"
	if (false == dtd.load_from_CSV("Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18.csv"))
		return -1;

	//cout << dtd.get_row_count() << " rows" << endl;
	//cout << dtd.get_D700_count() << " neutropenia count" << endl;
	//cout << endl;

	end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
	cout << elapsed.count() / 1000.0f << endl;

	start_time = std::chrono::high_resolution_clock::now();

	dtd.save_to_CSV("Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18_with_neutropenia_indicator.csv");

	end_time = std::chrono::high_resolution_clock::now();
	elapsed = end_time - start_time;
	cout << elapsed.count() / 1000.0f << endl;







	//nacrs_table_data ntd;

	//// "Z:/Smartphone_2/Shawn/Neutropenia/nacrs_cohorts_08_18.csv"
	//// "Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18.csv"
	//if (false == ntd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18.csv"))
	//	return -1;

	//cout << ntd.get_row_count() << " rows" << endl;
	//cout << ntd.get_D700_count() << " neutropenia count" << endl;
	//cout << endl;

	//ntd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18_with_neutropenia_indicator.csv");

	return 0;
}