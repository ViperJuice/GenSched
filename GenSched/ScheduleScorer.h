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
	ScheduleScorer(AvailabilityData &availabilityData, ScheduleData &scheduleData, std::vector<ScheduleScoreData> &vctScheduleScoreData);
	std::vector<std::function<size_t(std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember)>> getFuncs();
	void SetFinalScheduleFlag(bool isScoringFinalSchedules);
private:
	bool bFinalSchedules;
	AvailabilityData availabilityData;
	ScheduleData scheduleData;
	std::vector<ScheduleScoreData>* vctScheduleScoreData;
	std::vector<std::function<size_t(std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember)>> funcs;
	void PopulateScoreFunctions();
	size_t FindMapKeyFromValue(wstring wstrLookUp, std::map<size_t, wstring>& mapToLookIn);
	std::mutex mtxScheduleSpecific;
	std::mutex mtxDateSpecific;
	std::mutex mtxNameSpecific;
	std::mutex mtxDateAndNameSpecific;
	size_t alertCount=0;
	const size_t iDesiredNumberOfDaysKey;
	const size_t iPreferedNumberOfDaysInRowKey;
	const size_t iMaxNumberOfDaysKey;
	const size_t iOnOffOnPrefKey;
	const size_t iAlertBeforeGreyDayPrefKey;
	const size_t iAlertOnGreyDayPrefKey;
	const size_t iDinnerAndMovie1stHalfPrefKey;
	const size_t iBrave1QualKey;
	const size_t iSOFKey;
	const size_t iSuperKey;
	const size_t iDSGKey;

	const size_t iLessThanDesiredNumberOfDaysScoreKey;
	const size_t iGreaterThanDesiredNumberOfDaysScoreKey;
	const size_t iDSGGreaterThanDesiredNumberOfDaysScoreKey;
	const size_t iLessThanPreferedNumberOfDaysInRowScoreKey;
	const size_t iGreaterThanPreferedNumberOfDaysInRowScoreKey;
	const size_t iMaxNumberOfDaysScoreKey;
	const size_t iOnOffOnScoreKey;
	const size_t iAlertBeforeGreyDayScoreKey;
	const size_t iAlertOnGreyDayScoreKey;
	const size_t iDinnerAndMovie1stHalfScoreKey;
	const size_t i2SOFSScoreKey;
	const size_t i2SupersScoreKey;
	const size_t iScheduledOnDesiredDayScoreKey;
	const size_t iScheduledOnUnDesiredDayScoreKey;
	const size_t iDinnerAndMovieOnDesiredDayScoreKey;
	const size_t iBucketScheduledDaysDifferenceScoreKey;
	const size_t iDSGScheduledOnDesiredDayScoreKey;
	const size_t iBucketTooManyGreyDayScoreKey;
	const size_t iScheduledWithDesiredWingmanScoreKey;
	const size_t iPositionSwapScoreKey;
	const size_t MAXDAYS = 7;

	const int iMAXDAYS_Penalty = -1000; //TODO make iMAXDAYSPenalty selectable
	const int iDinnerAndMovieNotRequested_Penalty = -200; //TODO make iDinnerAndMovieNotRequested selectable
	const int iLessThanDesNumOfAlertDays_Score;
	const int iGreaterThanDesNumOfAlertDays_Score;
	const int iDSGGreaterThanDesNumOfAlertDays_Score;
	const int iGreaterThanPrefNumOfAlertDaysInRow_Score;
	const int iLessThanPrefNumOfAlertDaysInRow_Score;
	const int iMaxNumOfAlertDays_Score;
	const int iOnOffOn_Score;
	const int iAlertBeforeGreyDay_Score;
	const int iAlertOnGreyDay_Score=0;
	const int iDinnerAndMovie1stHalf_Score;
	const int i2SOFs_Score;
	const int i2Supers_Score;
	const int iScheduledOnDesiredDay_Score;
	const int iScheduledOnUnDesiredDay_Score;
	const int iDinnerAndMovieOnDesiredDay_Score;
	const int iDifferenceFromBucket_Score;
	const int iTooManyGreyDays_Score;
	const int iDSGScheduledOnAvailableDay_Score;
	const int iPositionSwap_Score;
	const int iScheduledWithDesiredWingman_Score;

};

#endif /* SCHEDULESCORER_H_ */