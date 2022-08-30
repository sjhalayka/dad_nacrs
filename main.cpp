#include "dad_table_data.h"
#include "nacrs_table_data.h"


int main(void)
{
	dad_table_data dtd;

	if (false == dtd.load_from_CSV("Z:/Smartphone_2/Shawn/Neutropenia/dad_cohorts_08_18.csv"))
		return -1;

	if (false == dtd.save_to_CSV("Z:/Smartphone_2/Shawn/Neutropenia/dad_cohorts_08_18_with_neutropenia_indicator.csv"))
		return -1;

	if (false == dtd.load_from_CSV("Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18.csv"))
		return -1;

	if (false == dtd.save_to_CSV("Z:/Smartphone_2/Shawn/Neutropenia/dad_post_08_18_with_neutropenia_indicator.csv"))
		return -1;


	nacrs_table_data ntd;

	if (false == ntd.load_from_CSV("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_cohorts_08_18.csv"))
		return -1;

	if(false == ntd.save_to_CSV("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_cohorts_08_18_with_neutropenia_indicator.csv"))
		return -1;

	if (false == ntd.load_from_CSV("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18.csv"))
		return -1;

	if (false == ntd.save_to_CSV("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18_with_neutropenia_indicator.csv"))
		return -1;

	return 0;
}