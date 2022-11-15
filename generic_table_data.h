// Guard against including this file more than once
#ifndef GENERIC_TABLE_DATA_H
#define GENERIC_TABLE_DATA_H



#include "table_data.h"




// Inherit from table_data class
class generic_table_data : public table_data
{
public:

	// Constructor
	generic_table_data(void)
	{

	}

	bool get_various_diag_codes(void)
	{
		return true;
	}
};



#endif
