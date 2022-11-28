// Guard against including this file more than once
#ifndef TABLE_DATA_H
#define TABLE_DATA_H



// Standard library console input/output
#include <iostream>
using std::cout;
using std::endl;

// File input/output
#include <fstream>
using std::ifstream;
using std::ofstream;

// Stringstream input/output
// This is for converting strings to integers (etc),
// and for converting integers (etc) back to strings
#include <sstream>
using std::istringstream;
using std::ostringstream;

// Vector (a kind of array) using multiple types of data
// We use this in lieu of new[]/delete[]
#include <vector>
using std::vector;

// String (a kind of array) using 8-bit chars
// We use this in lieu of new[]/delete[]
#include <string>
using std::string;
using std::getline;
using std::to_string;

#include <format>
using std::format;

// Regular expressions
// We use this to tokenize a string
#include <regex>
using std::regex;
using std::sregex_token_iterator;

// High-precision timing
#include <chrono>



#include "diagnosis_indicators.h"



class generic_table_data;


// Base class
class table_data
{
protected:

	// Keep the data inaccessible from outside the class
	vector<string> column_headers;
	vector<vector<string>> data;
	
	vector<string> diagnosis_indicator_names;
	vector<size_t> diagnosis_indicator_indices;
	vector<diagnosis_indicator> indicators;
	vector<size_t> diag_codes;

	string filepath = "";

public:

	template <class T>
	friend void merge(const T& left, const T& right, generic_table_data& out);

	bool get_index(const string& column_name, size_t& index) const;
	void std_strtok(const string& s, const string& regex_s, vector<string>& tokens);
	bool get_data_buffer(const string& filename);
	bool get_diagnosis_indicators(void);
	bool get_matches(void);
	bool add_column(const string& column_name, const string& initializer_value);
	bool rename_column(const string& column_name, const string& new_column_name);
	bool calc_age(const string& column_name_a, const string& column_name_b);
	bool delete_column(const string& column_name);
	bool replace(const string& column_name, const string& find_value, const string& replace_value);
	bool print_column(const string& column_name);
	void print_indicators(void);
	bool save_to_CSV_buffer(const string& filename);
	bool load_from_CSV_buffer(const string& filename);
	size_t get_row_count(void);
	size_t get_count(const string& indicator_name);
	string get_filename(void);

	// This function *must* be implemented by inheriting classes
	// Making it equal to 0 means that one cannot instantiate the
	// base class table_data
	virtual bool get_various_diag_codes(void) = 0;

};



#endif


