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


bool get_data(const string& filename, vector<string> &column_headers, vector<vector<string>> & data)
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

		if (data_cells.size() > column_headers.size() - 1)
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

size_t get_index(const string& column_name, const vector<string> &column_headers)
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

bool get_various_column_indices(const vector<string> &column_headers, size_t& neutropenia_index, vector<size_t>& diag_codes)
{
	neutropenia_index = get_index("Neutropenia_Indicator", column_headers);

	if (neutropenia_index == -1)
		return false;

	// Get column numbers for DIAG_CODE_1 through DIAG_CODE_25
	diag_codes.clear();

	for (size_t i = 1; i <= 25; i++)
	{
		ostringstream oss;
		oss << i;

		const string d = "DIAG_CODE_" + oss.str();

		size_t diag_codes_index = get_index(d, column_headers);

		if (diag_codes_index == -1)
			return false;
		else
			diag_codes.push_back(diag_codes_index);
	}

	if (diag_codes.size() != 25)
		return false;

	return true;
}