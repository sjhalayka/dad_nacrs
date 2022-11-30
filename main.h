#ifndef MAIN_H
#define MAIN_H



#include "dad_table_data.h"
#include "nacrs_table_data.h"
#include "generic_table_data.h"



template <class T>
void merge(const T& left, const T& right, generic_table_data& out)
{
	out.column_headers.clear();
	out.data.clear();

	unordered_map<string, size_t> header_count;

	for (size_t i = 0; i < left.column_headers.size(); i++)
		header_count[left.column_headers[i]]++;

	for (size_t i = 0; i < right.column_headers.size(); i++)
		header_count[right.column_headers[i]]++;

	for (auto ci = header_count.begin(); ci != header_count.end(); ci++)
	{
		size_t index_left = 0;
		size_t index_right = 0;

		if (false == left.get_index(ci->first, index_left) || false == right.get_index(ci->first, index_right))
		{
			//cout << "dropping " << ci->first << endl;
		}
		else
		{
			//cout << "merging " << ci->first << endl;

			vector<string> empty_vec;

			out.column_headers.push_back(ci->first);
			out.data.push_back(empty_vec);

			for (size_t i = 0; i < left.data[index_left].size(); i++)
				out.data[out.data.size() - 1].push_back(left.data[index_left][i]);

			for (size_t i = 0; i < right.data[index_right].size(); i++)
				out.data[out.data.size() - 1].push_back(right.data[index_right][i]);
		}
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
	if (right.tm_year > left.tm_year)
		return true;
	else if (right.tm_year < left.tm_year)
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

	if (date_equals(vtm[1], t_curr))
		return true;

	return false;
}

// for example: 31-Apr-07
bool is_date_between_two_dates(const string& sa, const string& sb, const string& s_curr)
{
	tm ta = {}, tb = {}, t_curr = {};

	istringstream iss(sa);
	iss >> get_time(&ta, "%d-%b-%y");

	iss.clear();
	iss.str(sb);
	iss >> get_time(&tb, "%d-%b-%y");

	iss.clear();
	iss.str(s_curr);
	iss >> get_time(&t_curr, "%d-%b-%y");

	return is_date_between_two_dates(ta, tb, t_curr);
}



#endif


