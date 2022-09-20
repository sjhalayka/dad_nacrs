#include "table_data.h"


bool table_data::get_index(const string& column_name, size_t& index)
{
	index = 0;

	vector<string>::const_iterator ci = find(column_headers.begin(), column_headers.end(), column_name);

	if (ci != column_headers.end())
	{
		index = ci - column_headers.begin();
		return true;
	}

	// If we made it this far, there was no match at all
	return false;
}

void table_data::std_strtok(const string& s, const string& regex_s, vector<string>& tokens)
{
	tokens.clear();

	regex r(regex_s);

	sregex_token_iterator iter(s.begin(), s.end(), r, -1);
	sregex_token_iterator end;

	while (iter != end)
		tokens.push_back(*iter++);
}

bool table_data::get_data_buffer(const string& filename)
{
	filepath = "";

	column_headers.clear();
	data.clear();

	ifstream infile(filename, ifstream::ate | ifstream::binary);

	if (infile.fail())
	{
		cout << "Could not open file" << endl;
		return false;
	}

	size_t file_size = infile.tellg();

	infile.close();

	if (file_size == 0)
		return false;

	infile.open(filename, ifstream::binary);

	if (infile.fail())
	{
		cout << "Could not re-open file" << endl;
		return false;
	}

	filepath = filename;

	string s(file_size, ' ');

	infile.read(&s[0], file_size);
	infile.close();

	// Make sure that the last line ends in a newline
	if (s[s.size() - 1] != '\n')
	{
		s += '\n';
		file_size++;
	}

	vector<string> tokens;
	string temp_token;

	bool first_line = true;

	// For each character in the file
	for (size_t i = 0; i < file_size; i++)
	{
		if (s[i] == '\r')
		{
			// Skip over Windows carriage return
			continue;
		}
		else if (s[i] == ',')
		{
			tokens.push_back(temp_token);
			temp_token = "";
		}
		else if (s[i] == '\n')
		{
			tokens.push_back(temp_token);
			temp_token = "";

			// Process line here
			if (first_line)
			{
				column_headers = tokens;

				// Add columns
				for (size_t j = 0; j < indicators.size(); j++)
					column_headers.push_back(indicators[j].diagnosis_name);

				data.resize(column_headers.size());

				first_line = false;
			}
			else
			{
				const size_t input_column_count = column_headers.size() - indicators.size();

				// Touch up the data in case it's broken
				if (tokens.size() > input_column_count)
				{
					// Too many data, chop off the end
					tokens.resize(input_column_count);
				}
				else if (tokens.size() < input_column_count)
				{
					// Not enough data, pad with empty strings
					const size_t num_to_add = input_column_count - tokens.size();

					for (size_t i = 0; i < num_to_add; i++)
						tokens.push_back("");
				}

				// Initialize indicators with empty strings
				for (size_t j = 0; j < indicators.size(); j++)
					tokens.push_back("");

				for (size_t j = 0; j < column_headers.size(); j++)
					data[j].push_back(tokens[j]);
			}

			tokens.clear();
		}
		else
		{
			temp_token += s[i];
		}
	}

	return true;
}

bool table_data::get_diagnosis_indicators(void)
{
	diagnosis_indicator_indices.clear();
	diagnosis_indicator_names.clear();

	for (size_t i = 0; i < indicators.size(); i++)
	{
		const string name = indicators[i].diagnosis_name;

		size_t index = 0;

		if (false == get_index(name, index))
			return false;

		diagnosis_indicator_indices.push_back(index);
		diagnosis_indicator_names.push_back(name);
	}

	return true;
}

bool table_data::save_to_CSV_buffer(const string& filename)
{
	cout << "Saving to file" << endl;

	// Throw everything into a single string.
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

	cout << "Writing data" << endl;

	// Write string contents to file in one shot
	ofstream outfile(filename, ifstream::binary);
	outfile.write(s.c_str(), s.length());

	cout << "Done saving to file" << endl;

	return true;
}

bool table_data::load_from_CSV_buffer(const string& filename)
{
	cout << "Loading from file" << endl;

	if (false == get_data_buffer(filename))
		return false;

	if (false == get_diagnosis_indicators())
		return false;

	if (false == get_various_diag_codes())
		return false;

	// The loop from hell is as follows...

	// For each diagnosis indicator
	for(size_t v = 0; v < diagnosis_indicator_indices.size(); v++)
	{
		const size_t di_index = diagnosis_indicator_indices[v];

		// For each row
		for (size_t r = 0; r < get_row_count(); r++)
		{
			// Use the default value
			data[di_index][r] = "0";

			// For each diagnostic code
			for (size_t d = 0; d < diag_codes.size(); d++)
			{
				const size_t j = diag_codes[d];

				bool found = false;

				// For each indicator's codes
				for (size_t i = 0; i < indicators[v].diagnosis_codes.size(); i++)
				{
					// If found code match, then adjust the 
					// indicator's datum and go to the next row
					if (data[j][r] == indicators[v].diagnosis_codes[i])
					{
						data[di_index][r] = "1";
						found = true;
						break;
					}
				}

				if (found)
					break;
			}
		}
	}

	cout << "Done loading from file" << endl;

	return true;
}

size_t table_data::get_row_count(void)
{
	if (data.size() == 0)
		return 0;

	return data[0].size();
}

size_t table_data::get_count(const string& indicator_name)
{
	size_t indicator_index = 0;

	if (false == get_index(indicator_name, indicator_index))
		return 0;

	size_t count = 0;
	const size_t row_count = get_row_count();

	for (size_t i = 0; i < row_count; i++)
		if (data[indicator_index][i] == "1")
			count++;

	return count;
}

string table_data::get_filename(void)
{
	return filepath;
}