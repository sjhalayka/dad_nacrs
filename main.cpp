#include "dad_table_data.h"
#include "nacrs_table_data.h"



int main(void)
{
	// Set up indicators
	//
	// Here we make use of the push_back
	// function, which adds an item to the end
	// of the vector
	vector<diagnosis_indicator> indicators;
	diagnosis_indicator d;

	d.diagnosis_name = "Neutropenia_Indicator";
	d.diagnosis_codes = { "D700" };
	indicators.push_back(d);

	d.diagnosis_name = "Myocarditis_Indicator";
	d.diagnosis_codes = { "I401", "I408", "I409", "I41", "I514" };
	indicators.push_back(d);

	d.diagnosis_name = "Cardiomyopathy_Indicator";
	d.diagnosis_codes = { "I420", "I421", "I423" "I424", "I425", "I427", "I428", "I429" };
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
	d.diagnosis_codes = { "X60", "X61", "X62", "X63", "X64", "X65", "X66", "X67", "X68", "X69" "X70", "X71", "X72", "X73", "X74", "X75", "X76", "X77", "X78", "X79", "X80", "X81", "X82", "X83", "X84" };
	indicators.push_back(d);



	// Handle DAD data
	dad_table_data dtd(indicators);

	if (false == dtd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/dad_cohorts_08_18.csv"))
		return -1;

	cout << dtd.get_filename() << endl;
	cout << "Neutropenia count:    " << dtd.get_count("Neutropenia_Indicator") << endl;
	cout << "Myocarditis count:    " << dtd.get_count("Myocarditis_Indicator") << endl;
	cout << "Cardiomyopathy count: " << dtd.get_count("Cardiomyopathy_Indicator") << endl;
	cout << "Schizophrenia count: " << dtd.get_count("Schizophrenia_Indicator") << endl;
	cout << "Schizoaffective count: " << dtd.get_count("Schizoaffective_Indicator") << endl;
	cout << "Bipolar count: " << dtd.get_count("Bipolar_Indicator") << endl;
	cout << "Other non-organic psychosis count: " << dtd.get_count("Other_non-organic_psychosis_Indicator") << endl;
	cout << "Unspecified organic psychosis count: " << dtd.get_count("Unspecified_organic_psychosis_Indicator") << endl;
	cout << "Self-harm: " << dtd.get_count("Self-harm_Indicator") << endl;
	cout << endl;

	if (false == dtd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/dad_cohorts_08_18_with_indicators.csv"))
		return -1;

	if (false == dtd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/dad_post_08_18.csv"))
		return -1;

	cout << dtd.get_filename() << endl;
	cout << "Neutropenia count:    " << dtd.get_count("Neutropenia_Indicator") << endl;
	cout << "Myocarditis count:    " << dtd.get_count("Myocarditis_Indicator") << endl;
	cout << "Cardiomyopathy count: " << dtd.get_count("Cardiomyopathy_Indicator") << endl;
	cout << "Schizophrenia count: " << dtd.get_count("Schizophrenia_Indicator") << endl;
	cout << "Schizoaffective count: " << dtd.get_count("Schizoaffective_Indicator") << endl;
	cout << "Bipolar count: " << dtd.get_count("Bipolar_Indicator") << endl;
	cout << "Other non-organic psychosis count: " << dtd.get_count("Other_non-organic_psychosis_Indicator") << endl;
	cout << "Unspecified organic psychosis count: " << dtd.get_count("Unspecified_organic_psychosis_Indicator") << endl;
	cout << "Self-harm: " << dtd.get_count("Self-harm_Indicator") << endl;
	cout << endl;


	if (false == dtd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/dad_post_08_18_with_indicators.csv"))
		return -1;



	// Handle NACRS data
	nacrs_table_data ntd(indicators);

	if (false == ntd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/nacrs_cohorts_08_18.csv"))
		return -1;

	cout << ntd.get_filename() << endl;
	cout << "Neutropenia count:    " << ntd.get_count("Neutropenia_Indicator") << endl;
	cout << "Myocarditis count:    " << ntd.get_count("Myocarditis_Indicator") << endl;
	cout << "Cardiomyopathy count: " << ntd.get_count("Cardiomyopathy_Indicator") << endl;
	cout << "Schizophrenia count: " << ntd.get_count("Schizophrenia_Indicator") << endl;
	cout << "Schizoaffective count: " << ntd.get_count("Schizoaffective_Indicator") << endl;
	cout << "Bipolar count: " << ntd.get_count("Bipolar_Indicator") << endl;
	cout << "Other non-organic psychosis count: " << ntd.get_count("Other_non-organic_psychosis_Indicator") << endl;
	cout << "Unspecified organic psychosis count: " << ntd.get_count("Unspecified_organic_psychosis_Indicator") << endl;
	cout << "Self-harm: " << ntd.get_count("Self-harm_Indicator") << endl;
	cout << endl;


	if(false == ntd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/nacrs_cohorts_08_18_with_indicators.csv"))
		return -1;

	if (false == ntd.load_from_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/nacrs_post_08_18.csv"))
		return -1;

	cout << ntd.get_filename() << endl;
	cout << "Neutropenia count:    " << ntd.get_count("Neutropenia_Indicator") << endl;
	cout << "Myocarditis count:    " << ntd.get_count("Myocarditis_Indicator") << endl;
	cout << "Cardiomyopathy count: " << ntd.get_count("Cardiomyopathy_Indicator") << endl;
	cout << "Schizophrenia count: " << ntd.get_count("Schizophrenia_Indicator") << endl;
	cout << "Schizoaffective count: " << ntd.get_count("Schizoaffective_Indicator") << endl;
	cout << "Bipolar count: " << ntd.get_count("Bipolar_Indicator") << endl;
	cout << "Other non-organic psychosis count: " << ntd.get_count("Other_non-organic_psychosis_Indicator") << endl;
	cout << "Unspecified organic psychosis count: " << ntd.get_count("Unspecified_organic_psychosis_Indicator") << endl;
	cout << "Self-harm: " << ntd.get_count("Self-harm_Indicator") << endl;
	cout << endl;

	if (false == ntd.save_to_CSV_buffer("Z:/Smartphone_2/Shawn/Indicators/nacrs_post_08_18_with_indicators.csv"))
		return -1;



	// It's all good
	return 0;
}