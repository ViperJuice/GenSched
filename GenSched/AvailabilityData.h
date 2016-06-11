#ifndef AVAILABILITYDATA_H_
#define AVAILABILITYDATA_H_
//Struct to contain availability data
struct AvailabilityData
{
	std::map<size_t, std::wstring> mapNumberName;
	std::map<size_t, std::wstring> mapNumberScorableType;
	std::map<size_t, std::wstring> mapNumberQualType;
	std::map<size_t, std::wstring> mapNumberPrefType;
	std::map<size_t, size_t> mapScorableNumToScore;
	std::map<size_t, size_t> mapNameNumToBucket;
	std::map<size_t, std::size_t> mapPrefNumToScorableNum; // associate preferences to scorable types where applicable


	enum weekdays { Sun, Mon, Tue, Wed, Thu, Fri, Sat };
	enum dayTypes { F, W, G };
	enum availabilityType
		{UNAVAILABLE = -5, LEAVE = -4, TDY = -3, PLEASE_NO = -2, RATHER_NOT = -1, DONT_CARE = 0, ALERT_IS_FINE = 1,
		ALERT_PLEASE = 2, DINNER_AND_MOVIE = 3};
	int** ppIntDateDayDayTypeArray;
	int** ppIntAvailabilityTypeArray;
	int** ppIntWingmanPrefArray;
	bool** ppBoolQualArray;
	int** ppIntPrefArray;
	int year;
	std::wstring month;
};
#endif /* AVAILABILITYDATA_H_ */