// Guard against including this file more than once
#ifndef DAD_TABLE_DATA_H
#define DAD_TABLE_DATA_H


#include "table_data.h"


// Inherit from table_data class
class dad_table_data : public table_data
{
public:

	// Function that is dataset specific
	bool get_various_column_indices(void);
};


#endif
