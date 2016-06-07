#ifndef AVAILABILITYDATA_H_
#define AVAILABILITYDATA_H_
//Struct to contain availability data
struct AvailabilityData
{
	std::map<unsigned int, std::wstring> mapNumberName;
	std::map<unsigned int, std::wstring> mapNumberScorableType;
	std::map<unsigned int, std::wstring> mapNumberQualType;
	std::map<unsigned int, unsigned int> mapScorableNumToScore;
	std::map<unsigned int, unsigned int> mapNameNumToBucket;

	enum weekdays { Sun, Mon, Tue, Wed, Thu, Fri, Sat };
	enum dayTypes { F, W, G };
	enum availabilityType
		{UNAVAILABLE = -5, LEAVE = -4, TDY = -3, PLEASE_NO = -2, RATHER_NOT = -1, DONT_CARE = 0, ALERT_IS_FINE = 1,
		ALERT_PLEASE = 2, DINNER_AND_MOVIE = 3};
	int** ppIntDateDayDayTypeArray;
	int** ppIntAvailabilityTypeArray;
	int** ppIntWingmanPrefArray;
	int** ppIntQualArray;
	int** ppPrefArray;
	int year;
	std::wstring month;
};
#endif /* AVAILABILITYDATA_H_ */