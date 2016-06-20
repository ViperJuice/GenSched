#include "pch.h"
#include "ScheduleScorer.h"


ScheduleScorer::ScheduleScorer(AvailabilityData &availabilityData, ScheduleData &scheduleData) :
	iDesiredNumberOfDaysKey(FindMapKeyFromValue(L"Des#", availabilityData.mapNumberPrefType)),
	iPreferedNumberOfDaysInRowKey(FindMapKeyFromValue(L"Pref#", availabilityData.mapNumberPrefType)),
	iMaxNumberOfDaysKey(FindMapKeyFromValue(L"Max#", availabilityData.mapNumberPrefType)),
	iOnOffOnPrefKey(FindMapKeyFromValue(L"O-O-O", availabilityData.mapNumberPrefType)),
	iAlertBeforeGreyDayPrefKey(FindMapKeyFromValue(L"Alert Before Grey Day", availabilityData.mapNumberPrefType)),
	iDinnerAndMovie1stHalfPrefKey(FindMapKeyFromValue(L"D&M", availabilityData.mapNumberPrefType)),
	iDesiredNumberOfDaysScoreKey(FindMapKeyFromValue(L"Des#_Score", availabilityData.mapNumberScorableType)),
	iPreferedNumberOfDaysInRowScoreKey(FindMapKeyFromValue(L"Pref#_Score", availabilityData.mapNumberScorableType)),
	iMaxNumberOfDaysScoreKey(FindMapKeyFromValue(L"Max#_Score", availabilityData.mapNumberScorableType)),
	iOnOffOnScoreKey(FindMapKeyFromValue(L"O-O-O_Score", availabilityData.mapNumberScorableType)),
	iAlertBeforeGreyDayScoreKey(FindMapKeyFromValue(L"Alert Before Grey Day_Score", availabilityData.mapNumberScorableType)),
	iDinnerAndMovie1stHalfScoreKey(FindMapKeyFromValue(L"D&M_Score", availabilityData.mapNumberScorableType)),
	i2SOFSScoreKey(FindMapKeyFromValue(L"2-SOFs_Score", availabilityData.mapNumberScorableType)),
	i2SupersScoreKey(FindMapKeyFromValue(L"2-Supers_Score", availabilityData.mapNumberScorableType)),
	iScheduledOnDesiredDayScoreKey(FindMapKeyFromValue(L"Scheduled-On-Desired-Day_Score", availabilityData.mapNumberScorableType)),
	iScheduledOnUnDesiredDayScoreKey(FindMapKeyFromValue(L"Scheduled-On-UnDesired-Day_Score", availabilityData.mapNumberScorableType)),
	iDinnerAndMovieOnDesiredDayScoreKey(FindMapKeyFromValue(L"D&M-On-Desired-Day_Score", availabilityData.mapNumberScorableType)),
	iBucketScheduledDaysDifferenceScoreKey(FindMapKeyFromValue(L"Bucket-Scheduled-Difference_Score", availabilityData.mapNumberScorableType)),
	iDSGScheduledOnDesiredDayScoreKey(FindMapKeyFromValue(L"DSG-Scheduled-On-Available-Day_Score", availabilityData.mapNumberScorableType)),
	iBucketTooManyGreyDayScoreKey(FindMapKeyFromValue(L"Too-Many-Grey-Days_Score", availabilityData.mapNumberScorableType)),
	iBrave1QualKey(FindMapKeyFromValue(L"Brave1", availabilityData.mapNumberQualType)),
	iSOFKey(FindMapKeyFromValue(L"SOF", availabilityData.mapNumberQualType)),
	iSuperKey(FindMapKeyFromValue(L"Super", availabilityData.mapNumberQualType)),
	iDSGKey(FindMapKeyFromValue(L"DSG", availabilityData.mapNumberQualType)),
	iDesNumOfAlertDays_Score(availabilityData.mapScorableNumToScore.find(iDesiredNumberOfDaysScoreKey)->second),
	iPrefNumOfAlertDaysInRow_Score(availabilityData.mapScorableNumToScore.find(iPreferedNumberOfDaysInRowScoreKey)->second),
	iMaxNumOfAlertDays_Score(availabilityData.mapScorableNumToScore.find(iMaxNumberOfDaysScoreKey)->second),
	iOnOffOn_Score(availabilityData.mapScorableNumToScore.find(iOnOffOnScoreKey)->second),
	iAlertBeforeDinnerAndMovie_Score(availabilityData.mapScorableNumToScore.find(iAlertBeforeGreyDayScoreKey)->second),
	i2SOFs_Score(availabilityData.mapScorableNumToScore.find(i2SOFSScoreKey)->second),
	i2Supers_Score(availabilityData.mapScorableNumToScore.find(i2SupersScoreKey)->second),
	iScheduledOnDesiredDayScore(availabilityData.mapScorableNumToScore.find(iScheduledOnDesiredDayScoreKey)->second),
	iScheduledOnUnDesiredDayScore(availabilityData.mapScorableNumToScore.find(iScheduledOnUnDesiredDayScoreKey)->second),
	iDinnerAndMovieOnDesiredDay_Score(availabilityData.mapScorableNumToScore.find(iDinnerAndMovieOnDesiredDayScoreKey)->second),
	iDifferenceFromBucket_Score(availabilityData.mapScorableNumToScore.find(iBucketScheduledDaysDifferenceScoreKey)->second),
	iDSGSceduledOnAvailableDayScore(availabilityData.mapScorableNumToScore.find(iDSGScheduledOnDesiredDayScoreKey)->second),
	iTooManyGreyDays_Score(availabilityData.mapScorableNumToScore.find(iBucketTooManyGreyDayScoreKey)->second)
{
	PopulateScoreFunctions(availabilityData, scheduleData);
}

std::vector<std::function<size_t(std::pair<size_t, size_t>*)>> ScheduleScorer::getFuncs()
{
	return funcs; 
}

void ScheduleScorer::PopulateScoreFunctions(AvailabilityData &availabilityData, ScheduleData &scheduleData)
{
	//Des-Num-Days_Score,Pref-Num-Days-In-Row_Score,Max-Num-Days-In-Row_Score On-Off-On_Score,Alert-Before-Gray-Day_Score,Dinner-and-Movie_Score,2-Supers_Score	2-SOFs_Score	Scheduled-On-Desired-Day_Score	Scheduled-On-UnDesired-Day_Score	D&M-On-Desired-Day_Score	Bucket-Scheduled-Difference_Score	Too-Many-Grey-Days_Score	DSG-Scheduled-On-Available-Day_Score
	//Function to score desired number of days vs scheduled number of days
	funcs.push_back([&](std::pair<size_t,size_t>* scheduleToScore) ->size_t
	{
		size_t* iAlertCount = new size_t[availabilityData.iNumberOfAvailabilityDataRows]();
		size_t* iDaysAvailable = new size_t[availabilityData.iNumberOfAvailabilityDataRows]();
		for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
		{
			size_t iDinnerAndMovie = 0;//tracks instances of dinner and a movie
			for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods;j++)
			{
				iDaysAvailable[i] +=
					availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.ALERT_PLEASE ||
					availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.ALERT_IS_FINE ||
					availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.DONT_CARE ||
					availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.RATHER_NOT ||
					availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.PLEASE_NO ||
					availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.UNAVAILABLE;
				iDinnerAndMovie += scheduleData.iNumberOfSubPeriods[i] > 1;
				iAlertCount[i] += 
					(scheduleData.iNumberOfSubPeriods[i] > 1 && 
					(
						scheduleToScore[j + iDinnerAndMovie].first == i ||
						scheduleToScore[j + iDinnerAndMovie - 1].first == i
					)) ||
					(
						scheduleData.iNumberOfSubPeriods[i] <= 1 &&
						scheduleToScore[j + iDinnerAndMovie].first == i
					);

				iAlertCount[i] += scheduleToScore[j + iDinnerAndMovie].second == i;
			}
		}
		size_t iscore = 0;

		for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
		{
			//difference between requested and scheduled number of days
			int delta = availabilityData.ppIntPrefArray[i][iDesiredNumberOfDaysKey] - iAlertCount[i];
			//reduce penelty for diference if requested days weere less than bucket requirement adjusted fro leave and TDY
			delta += availabilityData.ppIntPrefArray[i][iDesiredNumberOfDaysKey] < ((availabilityData.mapNameNumToBucket.find(i)->second+1)/ iDaysAvailable[i]);
			iscore += delta * availabilityData.ppIntPrefArray[i][iDesiredNumberOfDaysKey];
		}
		return iscore;
	});

	//Pref-Num-Days-In-Row_Score,Max-Num-Days-In-Row_Score On-Off-On_Score,Alert-Before-Gray-Day_Score,Dinner-and-Movie_Score,2-Supers_Score	2-SOFs_Score	Scheduled-On-Desired-Day_Score	Scheduled-On-UnDesired-Day_Score	D&M-On-Desired-Day_Score	Bucket-Scheduled-Difference_Score	Too-Many-Grey-Days_Score	DSG-Scheduled-On-Available-Day_Score
	//function to score prefered number of days in a row and max number days in a row
	funcs.push_back([&](std::pair<size_t, size_t>* scheduleToScore) ->size_t
	{
		
		size_t iscore = 0;

		for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
		{
			size_t iPreviouseDaysInARow = 0;
			size_t iDinnerAndMovie = 0;//tracks instances of dinner and a movie
			for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods;j++)
			{
				int iDaysInARow = 0;
				iDinnerAndMovie += scheduleData.iNumberOfSubPeriods[i] > 1;
				iDaysInARow +=
					(scheduleToScore[j + iDinnerAndMovie].first == i ||scheduleToScore[j + iDinnerAndMovie].second == i) &&
					(
						scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 1].first || 
						scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 1].second ||
						scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 1].first || 
						scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 1].second
					);
				//assign score for days in a row near desired days in a row
				int iTmpValue = ((iDaysInARow == 0 || j + iDinnerAndMovie == scheduleData.iTotalNumberOfSubPeriods) *
					iPreviouseDaysInARow - availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey]);
				iscore += std::abs(iTmpValue) * availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey];
				//penalize for exceeding max legal days
				iscore +=
					((iDaysInARow == 0 || j + iDinnerAndMovie == scheduleData.iTotalNumberOfSubPeriods) * iPreviouseDaysInARow > MAXDAYS) * iMAXDAYSPenalty;//TODO chnagee this
				//penalize for exeeding max prefered days, penelty is prefered days in a row penalty * 2
				iscore +=
					((iDaysInARow == 0 || j + iDinnerAndMovie == scheduleData.iTotalNumberOfSubPeriods) *
						iPreviouseDaysInARow > availabilityData.ppIntPrefArray[i][iMaxNumberOfDaysKey]) *
						((iDaysInARow == 0 || j + iDinnerAndMovie == scheduleData.iTotalNumberOfSubPeriods) *
							iPreviouseDaysInARow - availabilityData.ppIntPrefArray[i][iMaxNumberOfDaysKey]) *
							availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey] * 2;
				iPreviouseDaysInARow = iDaysInARow;

			}
		}
		return iscore;
	});
}

size_t ScheduleScorer::FindMapKeyFromValue(wstring wstrLookUp, std::map<size_t, wstring> &mapToLookIn)
{
	const wstring nameToFind = wstrLookUp;
	auto findResult = std::find_if(std::begin(mapToLookIn), std::end(mapToLookIn), [&](const std::pair<int, wstring> &pair)
	{
		return pair.second == nameToFind;
	});

	size_t foundKey = -1; // You might want to initialise this to a value you know is invalid in your map
	if (findResult != std::end(mapToLookIn))
	{
		foundKey = findResult->first;
	}
	return foundKey;
}
