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
			cout << "dropping " << ci->first << endl;
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


template <class T>
void split(T& in, generic_table_data &out_dadnacrs, generic_table_data& out_npduis)
{
	out_dadnacrs.column_headers = in.column_headers;
	out_npduis.column_headers = in.column_headers;

	out_dadnacrs.data.resize(out_dadnacrs.column_headers.size());
	out_npduis.data.resize(out_npduis.column_headers.size());

	size_t index = 0;
	string s = "drug_code";
	in.get_index(s, index);

	for (size_t i = 0; i < in.get_row_count(); i++)
	{
		if (in.data[index][i] == "")
		{
			for (size_t j = 0; j < in.data.size(); j++)
				out_dadnacrs.data[j].push_back(in.data[j][i]);
		}
		else
		{
			for (size_t j = 0; j < in.data.size(); j++)
				out_npduis.data[j].push_back(in.data[j][i]);
		}
	}
}


void sort_by_mbun(vector<npduis_row>& vn)
{
	map<string, vector<npduis_row>> mv;

	for (size_t i = 0; i < vn.size(); i++)
		mv[vn[i].mbun].push_back(vn[i]);

	vn.clear();

	for (map<string, vector<npduis_row>>::iterator i = mv.begin(); i != mv.end(); i++)
		sort(i->second.begin(), i->second.end());

	for (map<string, vector<npduis_row>>::iterator i = mv.begin(); i != mv.end(); i++)
		for (size_t j = 0; j < i->second.size(); j++)
			vn.push_back(i->second[j]);

	mv.clear();
}



#endif


