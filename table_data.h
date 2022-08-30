#pragma once


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <chrono>
using namespace std;


// Base class
class table_data
{
protected:

	// Keep the data inaccessible from outside the class
	vector<string> column_headers;
	vector<vector<string>> data;
	size_t neutropenia_index = 0;
	vector<size_t> diag_codes;

	// The following are helper functions that are inaccessible
	// from outside of the class

	// This function *must* be implemented by inheriting classes
	// Making it equal to 0 means that one cannot instantiate the
	// base class table_data
	virtual bool get_various_column_indices(void) = 0;

	size_t get_index(const string& column_name);
	bool get_data_line_by_line(const string& filename);
	void std_strtok(const string& s, const string& regex_s, vector<string>& tokens);

	bool save_to_CSV_buffer(const string& filename);
	bool load_from_CSV_line_by_line(const string& filename);

public:

	// These are accessible from outside the class
	bool save_to_CSV(const string& filename, bool use_buffer = true);
	bool load_from_CSV(const string& filename, bool use_buffer = true);
	size_t get_row_count(void);
	size_t get_D700_count(void);
};


