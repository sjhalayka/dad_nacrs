#pragma once


#include "table_data.h"


class dad_table_data : public table_data
{
public:

	// Function that is dataset specific
	bool get_various_column_indices(void);
};

