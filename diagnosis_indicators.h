// Guard against including this file more than once
#ifndef DIAGNOSIS_INDICATORS_H
#define DIAGNOSIS_INDICATORS_H



#include <string>
using std::string;

#include <vector>
using std::vector;



class diagnosis_indicator
{
public:

	string diagnosis_name;
	vector<string> diagnosis_codes;
};



#endif

