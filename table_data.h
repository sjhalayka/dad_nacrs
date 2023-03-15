// Guard against including this file more than once
#ifndef TABLE_DATA_H
#define TABLE_DATA_H



// Standard library console input/output
#include <iostream>
using std::cout;
using std::endl;

#include <map>
using std::map;

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

#include <list>
using std::list;


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

#include <ctime>
using std::get_time;
using std::put_time;

#include "diagnosis_indicators.h"


// Forward declare that there will be a class called generic_table_data,
// which will be inherited from the table_data class
class generic_table_data;




bool date_equals(const tm& left, const tm& right);

bool date_less_than(const tm& left, const tm& right);

void add_days_to_date(const string& sa, int num_days_to_add, string& out);

bool is_date_between_two_dates(tm& ta, tm& tb, tm& t_curr);

bool is_date_between_two_dates(const string& sa, const string& sb, const string& s_curr);



class npduis_row
{
public:
	bool operator<(const npduis_row& right) const
	{
		long unsigned int mbun_int = 0;
		long unsigned int right_mbun_int = 0;

		istringstream iss(mbun);
		iss >> mbun_int;

		iss.clear();
		iss.str(right.mbun);
		iss >> right_mbun_int;

		if (right_mbun_int > mbun_int)
			return true;
		else if (right_mbun_int < mbun_int)
			return false;


		if (right.drug_code > drug_code)
			return true;
		else if (right.drug_code < drug_code)
			return false;



		tm ta = {}, tb = {};

		//cout << episode_beg_dt << " " << right.episode_beg_dt << endl;


		iss.clear();
		iss.str(episode_beg_dt);
		iss >> get_time(&ta, "%d%b%Y");

		iss.clear();
		iss.str(right.episode_beg_dt);
		iss >> get_time(&tb, "%d%b%Y");

		if (date_less_than(ta, tb))
			return true;
		else if (date_less_than(tb, ta))
			return false;



		return false;
	}

	string mbun;
	string province;
	string birth_yr;
	string age;
	string female;
	string rural_unkn;
	string fiscal_year;
	string source_dad;
	string schizoph;
	string schizaff;
	string bipolar;
	string psychosis_org;
	string psychosis_non;
	string self_harm;
	string myocarditis;
	string cardiomyopathy;
	string neutropenia;
	string episode_beg_dt;
	string episode_end_dt;
	string drug_code;
	string drug_desc;
	string drug_desc_grp;
	string drug_set;
};




// Base class
class table_data
{
public:

	// Keep the data inaccessible from outside the class
	vector<string> column_headers;
	vector<vector<string>> data;
	
	vector<string> diagnosis_indicator_names;
	vector<size_t> diagnosis_indicator_indices;
	vector<diagnosis_indicator> indicators;
	vector<size_t> diag_codes;

	string filepath = "";

public:

	// Forward declaring that there will be a friend function that can access
	// the class' protected members
	template <class T>
	friend void merge(const T& left, const T& right, generic_table_data& out);

	template <class T>
	friend void split(T& in, generic_table_data& out_dadnacrs, generic_table_data& out_npduis);

	friend void sort_by_mbun(vector<npduis_row>& vn);


	bool sort_columns(const vector<string> &in_column_names);
	bool get_index(const string& column_name, size_t& index) const;
	void std_strtok(const string& s, const string& regex_s, vector<string>& tokens);
	bool get_data_buffer(const string& filename);
	bool get_diagnosis_indicators(void);
	bool get_matches(void);
	bool add_column(const string& column_name, const string& initializer_value);
	bool rename_column(const string& column_name, const string& new_column_name);
	bool calc_age(const string& column_name_a, const string& column_name_b);
	bool delete_column(const string& column_name);
	bool clear_rows(void);
	bool clear_memory(void);
	bool replace(const string& column_name, const string& find_value, const string& replace_value);
	bool print_column(const string& column_name);
	void print_indicators(void);
	bool save_to_CSV_buffer(const string& filename);
	bool load_from_CSV_buffer(const string& filename);
	size_t get_row_count(void);

	npduis_row get_npduis_row(const size_t row_index)
	{
		npduis_row nr;

		if (row_index >= get_row_count())
			return nr;

		nr.mbun = data[0][row_index];
		nr.province = data[1][row_index];
		nr.birth_yr = data[2][row_index];
		nr.age = data[3][row_index];
		nr.female = data[4][row_index];
		nr.rural_unkn = data[5][row_index];
		nr.fiscal_year = data[6][row_index];
		nr.source_dad = data[7][row_index];
		nr.schizoph = data[8][row_index];
		nr.schizaff = data[9][row_index];
		nr.bipolar = data[10][row_index];
		nr.psychosis_org = data[11][row_index];
		nr.psychosis_non = data[12][row_index];
		nr.self_harm = data[13][row_index];
		nr.myocarditis = data[14][row_index];
		nr.cardiomyopathy = data[15][row_index];
		nr.neutropenia = data[16][row_index];
		nr.episode_beg_dt = data[17][row_index];
		nr.episode_end_dt = data[18][row_index];
		nr.drug_code = data[19][row_index];
		nr.drug_desc = data[20][row_index];
		nr.drug_desc_grp = data[21][row_index];
		nr.drug_set = data[22][row_index];

		return nr;
	}

	void add_npduis_row(npduis_row nr)
	{
		data[0].push_back(nr.mbun);
		data[1].push_back(nr.province);
		data[2].push_back(nr.birth_yr);
		data[3].push_back(nr.age);
		data[4].push_back(nr.female);
		data[5].push_back(nr.rural_unkn);
		data[6].push_back(nr.fiscal_year);
		data[7].push_back(nr.source_dad);
		data[8].push_back(nr.schizoph);
		data[9].push_back(nr.schizaff);
		data[10].push_back(nr.bipolar);
		data[11].push_back(nr.psychosis_org);
		data[12].push_back(nr.psychosis_non);
		data[13].push_back(nr.self_harm);
		data[14].push_back(nr.myocarditis);
		data[15].push_back(nr.cardiomyopathy);
		data[16].push_back(nr.neutropenia);
		data[17].push_back(nr.episode_beg_dt);
		data[18].push_back(nr.episode_end_dt);
		data[19].push_back(nr.drug_code);
		data[20].push_back(nr.drug_desc);
		data[21].push_back(nr.drug_desc_grp);
		data[22].push_back(nr.drug_set);

	}






	void delete_row(size_t row_index)
	{
		if (row_index < get_row_count())
			for (size_t i = 0; i < data.size(); i++)
				data[i].erase(data[i].begin() + row_index);
	}

	void resize(const size_t max_row_count);
	size_t get_count(const string& indicator_name);
	string get_filename(void);
	void unify_column_names_case(void);

	// This function *must* be implemented by inheriting classes
	// Making it equal to 0 means that one cannot instantiate the
	// base class table_data
	virtual bool get_various_diag_codes(void) = 0;

};






#endif


