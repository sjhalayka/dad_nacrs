#pragma once

#include "table_data.h"

class nacrs_table_data : public table_data
{
public:

	// Function that is dataset specific
	bool get_various_column_indices(void)
	{
		neutropenia_index = get_index("Neutropenia_Indicator");

		if (neutropenia_index == -1)
			return false;

		diag_codes.clear();

		size_t diag_codes_index = get_index("ED_DISCHARGE_DIAG_CODE_1");

		if (diag_codes_index == -1)
			return false;
		else
			diag_codes.push_back(diag_codes_index);

		diag_codes_index = get_index("ED_DISCHARGE_DIAG_CODE_2");

		if (diag_codes_index == -1)
			return false;
		else
			diag_codes.push_back(diag_codes_index);

		diag_codes_index = get_index("ED_DISCHARGE_DIAG_CODE_3");

		if (diag_codes_index == -1)
			return false;
		else
			diag_codes.push_back(diag_codes_index);

		diag_codes_index = get_index("MAIN_PROBLEM");

		if (diag_codes_index == -1)
			return false;
		else
			diag_codes.push_back(diag_codes_index);

		for (size_t i = 1; i <= 9; i++)
		{
			ostringstream oss;
			oss << i;

			const string d = "OTHER_PROBLEM_" + oss.str();

			diag_codes_index = get_index(d);

			if (diag_codes_index == -1)
				return false;
			else
				diag_codes.push_back(diag_codes_index);
		}

		if (diag_codes.size() != 13)
			return false;

		return true;
	}
};


