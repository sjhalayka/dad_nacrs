#include "table_data.h"


bool table_data::get_index(const string& column_name, size_t& index) const
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

bool table_data::get_matches(void)
{
	// For each diagnosis indicator
	for (size_t q = 0; q < diagnosis_indicator_indices.size(); q++)
	{
		// For each row
		for (size_t r = 0; r < get_row_count(); r++)
		{
			// Use the default value
			data[diagnosis_indicator_indices[q]][r] = "0";

			// For each diagnostic code
			// For each indicator's diagnostic codes
			// If found code match, then adjust the indicator's datum
			for (size_t d = 0; d < diag_codes.size(); d++)
				for (size_t i = 0; i < indicators[q].diagnosis_codes.size(); i++)
					if (data[diag_codes[d]][r] == indicators[q].diagnosis_codes[i])
						data[diagnosis_indicator_indices[q]][r] = "1";
		}
	}

	return true;
}

bool table_data::add_column(const string& column_name, const string& initializer_value)
{
	size_t index = 0;

	// If already exists, abort
	if (get_index(column_name, index))
		return false;

	column_headers.push_back(column_name);

	vector<string> d(get_row_count(), initializer_value);
	data.push_back(d);

	return true;
}

bool table_data::rename_column(const string& column_name, const string& new_column_name)
{
	size_t index = 0;

	if (false == get_index(column_name, index))
		return false;

	column_headers[index] = new_column_name;

	return true;
}

bool table_data::calc_age(const string& column_name_a, const string& column_name_b)
{
	add_column("age", "");

	size_t column_a_index = 0;
	size_t column_b_index = 0;
	size_t age_index = 0;

	if (false == get_index(column_name_a, column_a_index))
	{
//		cout << "could not find " << column_name_a << endl;
		return false;
	}

	if (false == get_index(column_name_b, column_b_index))
	{
//		cout << "could not find " << column_name_b << endl;
		return false;
	}

	if (false == get_index("age", age_index))
	{
//		cout << "could not find age" << endl;
		return false;
	}




	for (size_t i = 0; i < get_row_count(); i++)
	{
		string column_b_string = data[column_b_index][i];
		unsigned int column_b_int = 0;

		istringstream iss(column_b_string);
		iss >> column_b_int;

		if (data[column_a_index][i].length() >= 2)
		{
			string admin_reg_year_string = data[column_a_index][i].substr(data[column_a_index][i].size() - 2, 2);
			unsigned int admin_reg_year_int = 0;

			iss.clear();
			iss.str(admin_reg_year_string);
			iss >> admin_reg_year_int;

			string current_year_string = format("{:%Y}", std::chrono::system_clock::now());
			current_year_string = current_year_string.substr(current_year_string.size() - 2, 2); // e.g. 20 for 2020
			
			size_t current_year_int = 0;

			iss.clear();
			iss.str(current_year_string);
			iss >> current_year_int;

			if (admin_reg_year_int > current_year_int)
				admin_reg_year_int = 1900 + admin_reg_year_int;
			else
				admin_reg_year_int = 2000 + admin_reg_year_int;

			data[age_index][i] = to_string(admin_reg_year_int - column_b_int);
		}
		else
		{
			data[age_index][i] = "NULL"; // this happens when the input is garbage
		}
	}

	return true;
}


bool table_data::print_column(const string& column_name)
{
	size_t index = 0;

	if (false == get_index(column_name, index))
		return false;

	for (size_t i = 0; i < get_row_count(); i++)
		cout << data[index][i] << endl;

	return true;
}


bool table_data::delete_column(const string& column_name)
{
	size_t index = 0;

	if (false == get_index(column_name, index))
		return false;

	column_headers.erase(column_headers.begin() + index);
	data.erase(data.begin() + index);

	return true;
}

bool table_data::clear_memory(void)
{
	column_headers.clear();
	data.clear();

	return true;
}

bool table_data::replace(const string& column_name, const string& find_value, const string& replace_value)
{
	size_t index = 0;

	// If does not exist, abort
	if (false == get_index(column_name, index))
		return false;

	for (size_t i = 0; i < data[index].size(); i++)
		if (data[index][i] == find_value)
			data[index][i] = replace_value;

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

//	cout << row_count << endl;

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
	//cout << "Loading from file" << endl;

	if (false == get_data_buffer(filename))
		return false;

	if (false == get_diagnosis_indicators())
		return false;

	if (false == get_various_diag_codes())
		return false;

	if (false == get_matches())
		return false;

	//cout << "Done loading from file" << endl;

	return true;
}

size_t table_data::get_row_count(void)
{
	if (data.size() == 0)
		return 0;

	return data[0].size();
}

void table_data::resize(const size_t max_row_count)
{
	for (size_t i = 0; i < data.size(); i++)
		data[i].resize(max_row_count);
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

void table_data::print_indicators(void)
{
	cout << get_filename() << endl;

	for (size_t i = 0; i < indicators.size(); i++)
		cout << indicators[i].diagnosis_name << ": " << get_count(indicators[i].diagnosis_name) << endl;

	cout << endl;
}

bool table_data::sort_columns(const vector<string> &in_column_names)
{
	// Do some consistency checking...
	if (in_column_names.size() != column_headers.size())
	{
//		cout << "Wrong number of columns. Aborting sort" << endl;
		return false;
	}

	map<string, size_t> src_counts;
		
	for (size_t i = 0; i < in_column_names.size(); i++)
		src_counts[in_column_names[i]]++;

	if (src_counts.size() != in_column_names.size())
	{
//		cout << "Duplicate input column found. Aborting sort" << endl;
		return false;
	}

	map<string, size_t> dest_counts;

	for (size_t i = 0; i < column_headers.size(); i++)
		dest_counts[column_headers[i]]++;

	if (dest_counts.size() != column_headers.size())
	{
//		cout << "Duplicate output column found. Aborting sort" << endl;
		return false;
	}

	if (src_counts != dest_counts)
	{
//		cout << "Column headers not identical. Aborting sort" << endl;
		return false;
	}

	// Consistency checks are finished. Do sort
	for (size_t i = 0; i < in_column_names.size(); i++)
	{
		size_t index = 0;

		if (false == get_index(in_column_names[i], index))
			return false;

		column_headers[i].swap(column_headers[index]);
		data[i].swap(data[index]);
	}

	return true;
}

void table_data::unify_column_names_case(void)
{
	for (size_t i = 0; i < column_headers.size(); i++)
	{
		string h = column_headers[i];

		// This column header is blank, nothing to do here
		if (h.size() == 0)
			continue;

		// Convert to all lower case
		transform(h.begin(), h.end(), h.begin(), tolower);

		// Set first letter to be upper case
		h[0] = toupper(h[0]);

		column_headers[i] = h;
	}
}



bool date_equals(const tm& left, const tm& right)
{
	if (right.tm_year == left.tm_year && right.tm_mon == left.tm_mon && right.tm_mday == left.tm_mday)
		return true;
	else
		return false;
}



bool date_less_than(const tm& left, const tm& right)
{
	if (right.tm_year + 1900 > left.tm_year + 1900)
		return true;
	else if (right.tm_year + 1900 < left.tm_year + 1900)
		return false;

	if (right.tm_mon > left.tm_mon)
		return true;
	else if (right.tm_mon < left.tm_mon)
		return false;

	if (right.tm_mday > left.tm_mday)
		return true;
	else if (right.tm_mday < left.tm_mday)
		return false;

	return false;
}



void add_days_to_date(const string& sa, int num_days_to_add, string& out)
{
	tm ta = {};

	istringstream iss(sa);
	iss >> get_time(&ta, "%d%b%Y");

	ta.tm_mday += num_days_to_add;
	mktime(&ta);

	ostringstream oss;
	oss << put_time(&ta, "%d%b%Y") << endl;
	out = oss.str();

	//cout << sa << " " << oss.str() << endl;
}


bool is_date_between_two_dates(tm& ta, tm& tb, tm& t_curr)
{
	//cout << put_time(&ta, "%c") << endl;
	//cout << put_time(&tb, "%c") << endl;
	//cout << put_time(&t_curr, "%c") << endl;

	vector<tm> vtm;
	vtm.push_back(ta);
	vtm.push_back(tb);
	vtm.push_back(t_curr);

	sort(vtm.begin(), vtm.end(), date_less_than);

	// If t_curr is in the middle, return true
	if (date_equals(vtm[1], t_curr))
		return true;

	return false;
}

// for example: 31-Apr-07
bool is_date_between_two_dates(const string& sa, const string& sb, const string& s_curr)
{
	tm ta = {}, tb = {}, t_curr = {};

	istringstream iss(sa);
	iss >> get_time(&ta, "%d%b%Y");

	iss.clear();
	iss.str(sb);
	iss >> get_time(&tb, "%d%b%Y");

	iss.clear();
	iss.str(s_curr);
	iss >> get_time(&t_curr, "%d%b%Y");

	return is_date_between_two_dates(ta, tb, t_curr);
}


