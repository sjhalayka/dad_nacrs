#pragma once

#include "classes.h"

class nacrs_table_data : public table_data
{
public:
	bool init(const string& filename)
	{
		if (false == get_data(filename))
			return false;

		if (false == get_various_column_indices(neutropenia_index, diag_codes))
			return false;

		const size_t row_count = get_row_count();

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

		return true;
	}

	bool get_data(const string& filename)
	{
		column_headers.clear();
		data.clear();

		ifstream infile(filename);

		if (infile.fail())
		{
			cout << "Could not open file" << endl;
			return false;
		}

		size_t line_num = 0;

		string line;

		// Get first line (the column titles)
		getline(infile, line);

		line_num++;

		if (line == "")
		{
			cout << "First line is blank!" << endl;
			return false;
		}

		column_headers = std_strtok(line, "[,]");

		// Add column manually
		column_headers.push_back("Neutropenia_Indicator");

		data.resize(column_headers.size());

		// Get subsequent lines (the data)
		while (getline(infile, line))
		{
			line_num++;

			if (line == "")
				continue;

			vector<string> data_cells = std_strtok(line, "[,]");

			if (data_cells.size() > (column_headers.size() - 1))
			{
				data_cells.resize(column_headers.size() - 1);
			}
			else if (data_cells.size() < (column_headers.size() - 1))
			{
				size_t num_to_add = (column_headers.size() - 1) - data_cells.size();

				for (size_t i = 0; i < num_to_add; i++)
				{
					data_cells.push_back("");
				}
			}

			// Initialize Neutropenia indicator
			data_cells.push_back("0");

			if (data_cells.size() != column_headers.size())
			{
				cout << "Data size mismatch on line " << line_num + 1 << ": " << data_cells.size() << " " << column_headers.size() << endl;
				cout << line_num + 1 << endl;
				continue;// return false;
			}

			for (size_t i = 0; i < column_headers.size(); i++)
				data[i].push_back(data_cells[i]);
		}

		return true;
	}

	bool get_various_column_indices(size_t& neutropenia_index, vector<size_t>& diag_codes)
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

		cout << "found diag and main" << endl;

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