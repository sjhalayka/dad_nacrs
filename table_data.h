#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <map>
#include <string>
#include <regex>
using namespace std;

class table_data
{
private:
	
	vector<string> column_headers;
	vector<vector<string>> data;
	size_t neutropenia_index = 0;
	vector<size_t> diag_codes;

public:

	// This function *must* be implemented by inheriting classes
	// Making it equal to 0 means that one cannot instantiate the
	// base class table_data
	virtual bool get_various_column_indices(size_t& neutropenia_index, vector<size_t>& diag_codes) = 0;

	void print_to_file(const string& filename)
	{
		ostringstream oss;
		
		for (size_t i = 0; i < (column_headers.size() - 1); i++)
			oss << column_headers[i] << ',';

		oss << column_headers[column_headers.size() - 1] << endl;

		const size_t row_count = get_row_count();

		for (size_t i = 0; i < row_count; i++)
		{
			for (size_t j = 0; j < (column_headers.size() - 1); j++)
				oss << data[j][i] << ',';

			oss << data[column_headers.size() - 1][i] << endl;
		}

		ofstream outfile(filename);

		outfile << oss.str().c_str();
	}

	// Functions that are dataset agnostic
	size_t get_row_count(void)
	{
		if (data.size() == 0)
			return 0;

		return data[0].size();
	}

	size_t get_D700_count(void)
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

	vector<string> std_strtok(const string& s, const string& regex_s)
	{
		vector<string> tokens;

		regex r(regex_s);

		sregex_token_iterator iter(s.begin(), s.end(), r, -1);
		sregex_token_iterator end;

		while (iter != end)
		{
			tokens.push_back(*iter);
			iter++;
		}

		return tokens;
	}

	size_t get_index(const string& column_name)
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

	bool init(const string &filename)
	{
		if (false == get_data(filename))
			return false;

		if (false == get_various_column_indices(neutropenia_index, diag_codes))
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
};