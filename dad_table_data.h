#pragma once

#include "table_data.h"

class dad_table_data : public table_data
{
public:

	// Function that is dataset specific
	bool get_various_column_indices(void)
	{
		neutropenia_index = get_index("Neutropenia_Indicator");

		if (neutropenia_index == -1)
			return false;

		// Get column numbers for DIAG_CODE_1 through DIAG_CODE_25
		diag_codes.clear();

		for (size_t i = 1; i <= 25; i++)
		{
			ostringstream oss;
			oss << i;

			const string d = "DIAG_CODE_" + oss.str();

			size_t diag_codes_index = get_index(d);

			if (diag_codes_index == -1)
				return false;
			else
				diag_codes.push_back(diag_codes_index);
		}

		if (diag_codes.size() != 25)
			return false;

		return true;
	}
};

