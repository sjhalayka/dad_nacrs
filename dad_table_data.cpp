#include "dad_table_data.h"



dad_table_data::dad_table_data(const vector<diagnosis_indicator>& src_indicators)
{
	indicators = src_indicators;
}

bool dad_table_data::get_various_diag_codes(void)
{
	// Get column numbers for DIAG_CODE_1 through DIAG_CODE_25
	diag_codes.clear();

	for (size_t i = 1; i <= 25; i++)
	{
		ostringstream oss;
		oss << i;

		const string d = "DIAG_CODE_" + oss.str();

		size_t diag_codes_index = 0;

		if (false == get_index(d, diag_codes_index))
			return false;
		else
			diag_codes.push_back(diag_codes_index);
	}

	// Something went horribly wrong
	if (diag_codes.size() != 25)
		return false;

	return true;
}



