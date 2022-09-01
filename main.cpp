#include "dad_table_data.h"
#include "nacrs_table_data.h"


int main(void)
{
	// Handle DAD data
	dad_table_data dtd;

	// Use the buffer method, which is a little faster than doing it line by line
	if (false == dtd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/dad_cohorts_08_18.csv"))
		return -1;

	// Saving uses the buffer method always 
	// Doing it line by line is so very glacially slow that I haven't even included
	// it in this source
	if (false == dtd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/dad_cohorts_08_18_with_neutropenia_indicator.csv"))
		return -1;

	if (false == dtd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18.csv"))
		return -1;

	if (false == dtd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18_with_neutropenia_indicator.csv"))
		return -1;


	// Handle NACRS data
	nacrs_table_data ntd;

	if (false == ntd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_cohorts_08_18.csv"))
		return -1;

	if(false == ntd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_cohorts_08_18_with_neutropenia_indicator.csv"))
		return -1;

	if (false == ntd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18.csv"))
		return -1;

	if (false == ntd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18_with_neutropenia_indicator.csv"))
		return -1;

	// It's all good
	return 0;
}