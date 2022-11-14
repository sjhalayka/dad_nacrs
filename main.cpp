#include "dad_table_data.h"
#include "nacrs_table_data.h"


template <class T>
void merge( const T &left, const T& right, dad_table_data &out)
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
			cout << "merging " << ci->first << endl;

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

/*
void merge(const nacrs_table_data& left, const dad_table_data& right, dad_table_data& out)
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
			cout << "merging " << ci->first << endl;

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




void merge(const dad_table_data& left, const nacrs_table_data& right, dad_table_data& out)
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
			cout << "merging " << ci->first << endl;

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
*/








int main(void)
{
	// Set up indicators
	vector<diagnosis_indicator> indicators;
	diagnosis_indicator d;

	d.diagnosis_name = "Neutropenia_Indicator";
	d.diagnosis_codes = { "D700" };
	indicators.push_back(d);

	d.diagnosis_name = "Myocarditis_Indicator";
	d.diagnosis_codes = { "I401", "I408", "I409", "I41", "I514" };
	indicators.push_back(d);

	d.diagnosis_name = "Cardiomyopathy_Indicator";
	d.diagnosis_codes = { "I420", "I421", "I422", "I423", "I424", "I425", "I427", "I428", "I429" };
	indicators.push_back(d);

	d.diagnosis_name = "Schizophrenia_Indicator";
	d.diagnosis_codes = { "F200", "F201", "F202", "F203", "F204", "F205", "F206", "F208", "F209" };
	indicators.push_back(d);

	d.diagnosis_name = "Schizoaffective_Indicator";
	d.diagnosis_codes = { "F250", "F251", "F258", "F259" };
	indicators.push_back(d);
	
	d.diagnosis_name = "Bipolar_Indicator";
	d.diagnosis_codes = { "F310", "F311", "F312", "F313", "F314", "F315", "F316", "F317", "F318", "F319" };
	indicators.push_back(d);

	d.diagnosis_name = "Other_non-organic_psychosis_Indicator";
	d.diagnosis_codes = { "F28" };
	indicators.push_back(d);

	d.diagnosis_name = "Unspecified_organic_psychosis_Indicator";
	d.diagnosis_codes = { "F29" };
	indicators.push_back(d);
	
	d.diagnosis_name = "Self-harm_Indicator";
	d.diagnosis_codes = { "X60", "X61", "X62", "X63", "X64", "X65", "X66", "X67", "X68", "X69", "X70", "X71", "X72", "X73", "X74", "X75", "X76", "X77", "X78", "X79", "X80", "X81", "X82", "X83", "X84" };
	indicators.push_back(d);


	// Handle DAD data
	dad_table_data dtd0(indicators);
	dad_table_data dtd1(indicators);

	if (false == dtd0.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/dad_cohorts_08_18.csv"))
		return -1;

	if (false == dtd1.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/dad_post_08_18.csv"))
		return -1;

	dad_table_data dtd_out(indicators);

	merge<dad_table_data>(dtd0, dtd1, dtd_out);




	//if (false == out.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/aggregate.csv"))
	//{
	//	cout << "Could not save CSV file" << endl;
	//	return -1;
	//}

	//cout << "done" << endl;

	//return 0;






	// Handle NACRS data
	nacrs_table_data ntd0(indicators);
	nacrs_table_data ntd1(indicators);

	if (false == ntd0.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/nacrs_cohorts_08_18.csv"))
		return -1;

	if (false == ntd1.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/nacrs_post_08_18.csv"))
		return -1;

	dad_table_data ntd_out(indicators);

	merge<nacrs_table_data>(ntd0, ntd1, ntd_out);

	dad_table_data all(indicators);

	merge<dad_table_data>(dtd_out, ntd_out, all);


	if (false == all.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/aggregate.csv"))
		return -1;


	// It's all good
	return 0;
}


