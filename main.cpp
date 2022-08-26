#include "dad_table_data.h"
#include "nacrs_table_data.h"

int main(void)
{
	dad_table_data dtd;

	// "Z:/Smartphone_2/Shawn/Neutropenia/dad_08_18_cohorts.csv"
	// "Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18.csv"
	if (false == dtd.init("Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18.csv"))
		return -1;

	cout << dtd.get_row_count() << " rows" << endl;
	cout << dtd.get_D700_count() << " neutropenia count" << endl;
	cout << endl;

	dtd.print_to_file("Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18_with_neutropenia_indicator.csv");


	nacrs_table_data ntd;

	// "Z:/Smartphone_2/Shawn/Neutropenia/nacrs_cohorts_08_18.csv"
	// "Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18.csv"
	if (false == ntd.init("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18.csv"))
		return -1;

	cout << ntd.get_row_count() << " rows" << endl;
	cout << ntd.get_D700_count() << " neutropenia count" << endl;
	cout << endl;

	ntd.print_to_file("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18_with_neutropenia_indicator.csv");

	return 0;
}