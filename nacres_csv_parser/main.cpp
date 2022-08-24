	#include "main.h"

int main(void)
{
	// Get column headers and data
	vector<string> column_headers;
	vector<vector<string>> data;


//	if (false == get_data("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_cohorts_08_18.csv", column_headers, data))
	if (false == get_data("Z:/Smartphone_2/Shawn/Neutropenia/nacrs_post_08_18.csv", column_headers, data))
		return 0;
	else
		cout << "Read file successfully" << endl;

	// Get indices of interest
	size_t neutropenia_index = 0;
	vector<size_t> diag_codes;

	if(false == get_various_column_indices(column_headers, neutropenia_index, diag_codes))
		return 0;
	else
		cout << "Found column indices successfully" << endl;

	// Get row count from 1st datum
	const size_t row_count = data[0].size();

	// Search for D700 code(s)
	for (size_t i = 0; i < row_count; i++)
	{
		for (size_t j = 0; j < diag_codes.size(); j++)
		{
			const size_t index = diag_codes[j];

			// If found D700, then adjust the 
			// Neutropenia indicator and go to next row
			if (data[index][i] == "D700")
			{		
				data[neutropenia_index][i] = "1";
				break;
			}
		}
	}

	// Count D700s
	size_t D700_count = 0;

	for (size_t i = 0; i < row_count; i++)
	{
		if (data[neutropenia_index][i] == "1")
			D700_count++;
	}
	
	cout << "Row count: " << row_count << ", D700 count: " << D700_count << endl;

	return 1;
}