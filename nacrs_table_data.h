#pragma once


#include "table_data.h"


class nacrs_table_data : public table_data
{
public:

	// Function that is dataset specific
	bool get_various_column_indices(void);
};


