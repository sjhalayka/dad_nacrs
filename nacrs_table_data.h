// Guard against including this file more than once
#ifndef NACRS_TABLE_DATA_H
#define NACRS_TABLE_DATA_H



#include "table_data.h"



// Inherit from table_data class
class nacrs_table_data : public table_data
{
public:

	// Constructor
	nacrs_table_data(const vector<diagnosis_indicator>& src_indicators);

	// Function that is dataset specific
	bool get_various_column_indices(void);
};


#endif

