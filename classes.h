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
public:
	vector<string> column_headers;
	vector<vector<string>> data;

	size_t neutropenia_index = 0;
	vector<size_t> diag_codes;

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
};