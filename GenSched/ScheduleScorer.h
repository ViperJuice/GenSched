/*
* ScheduleScore.h
*
*  Created on: June 19, 2016
*      Author: Jenner Torrence
*      Container Class to hold scoring functions for genetic scheduling algorithm
*/

#ifndef SCHEDULESCORER_H_
#define SCHEDULESCORER_H_
using namespace std;
class ScheduleScorer sealed
{
public:
	ScheduleScorer(AvailabilityData &availabilityData, ScheduleData &scheduleData);
	std::vector<std::function<size_t(std::pair<size_t, size_t>*)>> getFuncs();
private:
	std::vector<std::function<size_t(std::pair<size_t, size_t>*)>> funcs;
	void PopulateScoreFunctions(AvailabilityData &availabilityData, ScheduleData &scheduleData);
	size_t FindMapKeyFromValue(wstring wstrLookUp, std::map<size_t, wstring>& mapToLookIn);
	size_t alertCount=0;
	const size_t iDesiredNumberOfDaysKey;
	const size_t iPreferedNumberOfDaysInRowKey;
	const size_t iMaxNumberOfDaysKey;
	const size_t iOnOffOnPrefKey;
	const size_t iAlertBeforeGreyDayPrefKey;
	const size_t iDinnerAndMovie1stHalfPrefKey;
	const size_t iBrave1QualKey;
	const size_t iSOFKey;
	const size_t iSuperKey;
	const size_t iDSGKey;

	const size_t iDesiredNumberOfDaysScoreKey;
	const size_t iPreferedNumberOfDaysInRowScoreKey;
	const size_t iMaxNumberOfDaysScoreKey;
	const size_t iOnOffOnScoreKey;
	const size_t iAlertBeforeGreyDayScoreKey;
	const size_t iDinnerAndMovie1stHalfScoreKey;
	const size_t i2SOFSScoreKey;
	const size_t i2SupersScoreKey;
	const size_t iScheduledOnDesiredDayScoreKey;
	const size_t iScheduledOnUnDesiredDayScoreKey;
	const size_t iDinnerAndMovieOnDesiredDayScoreKey;
	const size_t iBucketScheduledDaysDifferenceScoreKey;
	const size_t iDSGScheduledOnDesiredDayScoreKey;
	const size_t iBucketTooManyGreyDayScoreKey;
	const size_t MAXDAYS = 7;

	const int iMAXDAYSPenalty = -1000;
	const int iDesNumOfAlertDays_Score;
	const int iPrefNumOfAlertDaysInRow_Score;
	const int iMaxNumOfAlertDays_Score;
	const int iOnOffOn_Score;
	const int iAlertBeforeDinnerAndMovie_Score;
	const int i2SOFs_Score;
	const int i2Supers_Score;
	const int iScheduledOnDesiredDayScore;
	const int iScheduledOnUnDesiredDayScore;
	const int iDinnerAndMovieOnDesiredDay_Score;
	const int iDifferenceFromBucket_Score;
	const int iTooManyGreyDays_Score;
	const int iDSGSceduledOnAvailableDayScore;

};

#endif /* SCHEDULESCORER_H_ */