#include "nacrs_table_data.h"


bool nacrs_table_data::get_various_column_indices(void)
{
	if (false == get_index("Neutropenia_Indicator", neutropenia_index))
		return false;

	diag_codes.clear();

	size_t diag_codes_index = 0;

	if (false == get_index("ED_DISCHARGE_DIAG_CODE_1", diag_codes_index))
		return false;
	else
		diag_codes.push_back(diag_codes_index);

	if (false == get_index("ED_DISCHARGE_DIAG_CODE_2", diag_codes_index))
		return false;
	else
		diag_codes.push_back(diag_codes_index);

	if (false == get_index("ED_DISCHARGE_DIAG_CODE_3", diag_codes_index))
		return false;
	else
		diag_codes.push_back(diag_codes_index);

	if (false == get_index("MAIN_PROBLEM", diag_codes_index))
		return false;
	else
		diag_codes.push_back(diag_codes_index);

	for (size_t i = 1; i <= 9; i++)
	{
		ostringstream oss;
		oss << i;

		const string d = "OTHER_PROBLEM_" + oss.str();

		if (false == get_index(d, diag_codes_index))
			return false;
		else
			diag_codes.push_back(diag_codes_index);
	}

	// Something went horribly wrong
	if (diag_codes.size() != 13)
		return false;

	return true;
}