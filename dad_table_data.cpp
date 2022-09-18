#include "dad_table_data.h"


bool dad_table_data::get_various_column_indices(void)
{
	diagnosis_indicator_indices.clear();

	cout << "getting column indices" << endl;

	for (size_t i = 0; i < indicators.size(); i++)
	{
		const string name = indicators[i].diagnosis_name;

		size_t index = 0;

		if (false == get_index(name, index))
			return false;

		diagnosis_indicator_indices[name] = index;
	}

	cout << "done getting indices" << endl;

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

	cout << "done getting diag codes" << endl;

	return true;
}