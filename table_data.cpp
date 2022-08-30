#include "table_data.h"


size_t table_data::get_index(const string& column_name)
{
	size_t index = 0;

	for (vector<string>::const_iterator ci = column_headers.begin(); ci != column_headers.end(); ci++)
	{
		if (*ci == column_name)
			return index;
		else
			index++;
	}

	// If we made it this far, there was no match at all
	return -1;
}

bool table_data::get_data_line_by_line(const string& filename)
{
	column_headers.clear();
	data.clear();

	ifstream infile(filename);

	if (infile.fail())
	{
		cout << "Could not open file" << endl;
		return false;
	}

	string line;

	// Get first line (the variable names)
	getline(infile, line);

	if (line == "")
	{
		cout << "First line is blank!" << endl;
		return false;
	}

	std_strtok(line, "[,]", column_headers);

	// Add column
	column_headers.push_back("Neutropenia_Indicator");

	data.resize(column_headers.size());

	// Get subsequent lines (the data)
	while (getline(infile, line))
	{
		if (line == "")
			continue;

		vector<string> data_cells;
		std_strtok(line, "[,]", data_cells);

		// Touch up the data in case it's broken
		if (data_cells.size() > (column_headers.size() - 1))
		{
			// Too many data, chop off the end
			data_cells.resize(column_headers.size() - 1);
		}
		else if (data_cells.size() < (column_headers.size() - 1))
		{
			// Not enough data, pad with empty strings
			size_t num_to_add = (column_headers.size() - 1) - data_cells.size();

			for (size_t i = 0; i < num_to_add; i++)
				data_cells.push_back("");
		}

		// Initialize Neutropenia indicator
		data_cells.push_back("0");

		for (size_t i = 0; i < column_headers.size(); i++)
			data[i].push_back(data_cells[i]);
	}

	return true;
}

void table_data::std_strtok(const string& s, const string& regex_s, vector<string>& tokens)
{
	tokens.clear();

	regex r(regex_s);

	sregex_token_iterator iter(s.begin(), s.end(), r, -1);
	sregex_token_iterator end;

	while (iter != end)
	{
		tokens.push_back(*iter);
		iter++;
	}
}

bool table_data::save_to_CSV(const string& filename)
{
	// Throw everything into a single string
	// This takes up 2x the RAM, but it's about as fast
	// as it can get
	string s;

	for (size_t i = 0; i < (column_headers.size() - 1); i++)
	{
		s += column_headers[i];
		s += ',';
	}

	s += column_headers[column_headers.size() - 1];
	s += '\n';

	const size_t row_count = get_row_count();

	for (size_t i = 0; i < row_count; i++)
	{
		for (size_t j = 0; j < (column_headers.size() - 1); j++)
		{
			s += data[j][i];
			s += ',';
		}

		s += data[column_headers.size() - 1][i];
		s += '\n';
	}

	// Write string contents to file in one shot
	ofstream outfile(filename, ios_base::binary);
	outfile.write(s.c_str(), s.length());

	return true;
}

bool table_data::load_from_CSV(const string& filename)
{
	if (false == get_data_line_by_line(filename))
		return false;

	if (false == get_various_column_indices())
		return false;

	const size_t row_count = get_row_count();

	// Search for D700 code(s), 
	// to populate the Neutropenia indicator
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

size_t table_data::get_row_count(void)
{
	if (data.size() == 0)
		return 0;

	return data[0].size();
}

size_t table_data::get_D700_count(void)
{
	const size_t row_count = get_row_count();

	size_t D700_count = 0;

	for (size_t i = 0; i < row_count; i++)
	{
		if (data[neutropenia_index][i] == "1")
			D700_count++;
	}

	return D700_count;
}
