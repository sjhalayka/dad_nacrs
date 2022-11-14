// Guard against including this file more than once
#ifndef DAD_TABLE_DATA_H
#define DAD_TABLE_DATA_H



#include "table_data.h"
#include "nacrs_table_data.h"

#include <map>
#include <unordered_map>
#include <string>
using namespace std;


// Inherit from table_data class
class dad_table_data : public table_data
{
public:

	// Constructor
	dad_table_data(const vector<diagnosis_indicator>& src_indicators);

	// Function that is dataset specific
	bool get_various_diag_codes(void);


};



#endif
