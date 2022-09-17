#include "table_data.h"


bool table_data::get_index(const string& column_name, size_t &index)
{
	index = 0;

	for (vector<string>::const_iterator ci = column_headers.begin(); ci != column_headers.end(); ci++)
	{
		if (*ci == column_name)
			return true;
		else
			index++;
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
	{
		tokens.push_back(*iter);
		iter++;
	}
}

bool table_data::get_data_buffer(const string& filename)
{
	filepath = "";

	//cout << "Getting file size" << endl;

	//std::chrono::high_resolution_clock::time_point start_time, end_time;
	//start_time = std::chrono::high_resolution_clock::now();

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

	//cout << "Reopening file" << endl;

	infile.open(filename, ifstream::binary);

	if (infile.fail())
	{
		cout << "Could not re-open file" << endl;
		return false;
	}

	filepath = filename;

	//cout << "Allocating memory" << endl;
	string s(file_size, ' ');

	//cout << "Reading file" << endl;
	infile.read(&s[0], file_size);
	infile.close();

	// Make sure that the last line ends in a newline
	if (s[s.size() - 1] != '\n')
	{
		s += '\n';
		file_size += 1;
	}

	vector<string> tokens;
	string temp_token;

	bool first_line = true;

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
				column_headers.push_back("Neutropenia_Indicator");
				column_headers.push_back("Myocarditis_Indicator");
				column_headers.push_back("Cardiomyopathy_Indicator");

				data.resize(column_headers.size());

				first_line = false;
			}
			else
			{
				vector<string> data_cells = tokens;

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

				// Initialize indicators with blank data
				data_cells.push_back("");
				data_cells.push_back("");
				data_cells.push_back("");

				for (size_t j = 0; j < column_headers.size(); j++)
					data[j].push_back(data_cells[j]);
			}

			tokens.clear();
		}
		else
		{
			temp_token += s[i];
		}
	}

	//end_time = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<float, std::milli> elapsed = end_time - start_time;
	//cout << elapsed.count() / 1000.0f << endl;

	return true;
}

bool table_data::save_to_CSV_buffer(const string& filename)
{
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

	// Write string contents to file in one shot
	ofstream outfile(filename, ifstream::binary);
	outfile.write(s.c_str(), s.length());

	return true;
}


bool table_data::load_from_CSV_buffer(const string& filename)
{
	if (false == get_data_buffer(filename))
		return false;

	if (false == get_various_column_indices())
		return false;

	const size_t row_count = get_row_count();

	// Search for neutropenia code(s), 
	// to populate the Neutropenia indicator
	for (size_t i = 0; i < row_count; i++)
	{
		data[neutropenia_index][i] = "0";

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

	// Search for myocarditis code(s), 
	// to populate the myocarditis indicator
	for (size_t i = 0; i < row_count; i++)
	{
		data[myocarditis_index][i] = "0";

		for (size_t j = 0; j < diag_codes.size(); j++)
		{
			const size_t index = diag_codes[j];

			// If found code, then adjust the 
			// indicator and go to next row
			if (data[index][i] == "I401" || 
				data[index][i] == "I408" ||
				data[index][i] == "I409" ||
				data[index][i] == "I41"  ||
				data[index][i] == "I514")
			{
				data[myocarditis_index][i] = "1";
				break;
			}
		}
	}

	// Search for cardiomyopathy code(s), 
	// to populate the cardiomyopathy indicator
	for (size_t i = 0; i < row_count; i++)
	{
		data[cardiomyopathy_index][i] = "0";

		for (size_t j = 0; j < diag_codes.size(); j++)
		{
			const size_t index = diag_codes[j];

			// If found code, then adjust the 
			// indicator and go to next row
			if (data[index][i] == "I420" ||
				data[index][i] == "I421" ||
				data[index][i] == "I422" ||
				data[index][i] == "I423" ||
				data[index][i] == "I424" ||
				data[index][i] == "I425" ||
				data[index][i] == "I427" ||
				data[index][i] == "I428" ||
				data[index][i] == "I429")
			{
				data[cardiomyopathy_index][i] = "1";
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

size_t table_data::get_neutropenia_count(void)
{
	const size_t row_count = get_row_count();

	size_t count = 0;

	for (size_t i = 0; i < row_count; i++)
	{
		if (data[neutropenia_index][i] == "1")
			count++;
	}

	return count;
}

size_t table_data::get_myocarditis_count(void)
{
	const size_t row_count = get_row_count();

	size_t count = 0;

	for (size_t i = 0; i < row_count; i++)
	{
		if (data[myocarditis_index][i] == "1")
			count++;
	}

	return count;
}

size_t table_data::get_cardiomyopathy_count(void)
{
	const size_t row_count = get_row_count();

	size_t count = 0;

	for (size_t i = 0; i < row_count; i++)
	{
		if (data[cardiomyopathy_index][i] == "1")
			count++;
	}

	return count;
}

string table_data::get_filename(void)
{
	return filepath;
}