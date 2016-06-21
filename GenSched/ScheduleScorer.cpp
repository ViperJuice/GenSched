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
	iPositionSwapScoreKey(FindMapKeyFromValue(L"Position-Swap_Score", availabilityData.mapNumberScorableType)),
	iScheduledWithDesiredWingmanScoreKey(FindMapKeyFromValue(L"Scheduled-With-Desired-Wingman_Score", availabilityData.mapNumberScorableType)),

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
	iDSGSceduledOnAvailableDay_Score(availabilityData.mapScorableNumToScore.find(iDSGScheduledOnDesiredDayScoreKey)->second),
	iTooManyGreyDays_Score(availabilityData.mapScorableNumToScore.find(iBucketTooManyGreyDayScoreKey)->second),
	iPositionSwap_Score(availabilityData.mapScorableNumToScore.find(iPositionSwapScoreKey)->second),
	iScheduledWithDesiredWingman_Score(availabilityData.mapScorableNumToScore.find(iScheduledWithDesiredWingmanScoreKey)->second)
{
	PopulateScoreFunctions(availabilityData, scheduleData);
}

std::vector<std::function<size_t(std::pair<size_t, size_t>*)>> ScheduleScorer::getFuncs()
{
	return funcs; 
}

void ScheduleScorer::PopulateScoreFunctions(AvailabilityData &availabilityData, ScheduleData &scheduleData)
{
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
					availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.UNAVAILABLE ||
					availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.CT_FLY_DAY;
				iDinnerAndMovie += scheduleData.iNumberOfSubPeriods[i] > 1; //increment D&M count
				iAlertCount[i] +=  
					(scheduleData.iNumberOfSubPeriods[i] > 1 && 
					(
						scheduleToScore[j + iDinnerAndMovie].first == i || //increment alert count on D&M day
						scheduleToScore[j + iDinnerAndMovie - 1].first == i ||
						scheduleToScore[j + iDinnerAndMovie].second == i || 
						scheduleToScore[j + iDinnerAndMovie - 1].second == i
					)) ||
					(
						(scheduleData.iNumberOfSubPeriods[i] <= 1 && //increment alert count on non-D&M day
						scheduleToScore[j + iDinnerAndMovie].first == i) ||
						(scheduleData.iNumberOfSubPeriods[i] <= 1 &&
						scheduleToScore[j + iDinnerAndMovie].second == i)
					);
			}
		}
		int iscore = 0;
		for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
		{
			//difference between requested and scheduled number of days
			int delta = availabilityData.ppIntPrefArray[i][iDesiredNumberOfDaysKey] - iAlertCount[i];
			//reduce penelty for difference if requested days weere less than bucket requirement adjusted fro leave and TDY
			delta += availabilityData.ppIntPrefArray[i][iDesiredNumberOfDaysKey] < ((availabilityData.mapNameNumToBucket.find(i)->second+1)/ iDaysAvailable[i]);
			iscore += std::abs(delta) * iDesNumOfAlertDays_Score;
		}
		return iscore;
	});

	//function to score prefered number of days in a row and max number days in a row
	funcs.push_back([&](std::pair<size_t, size_t>* scheduleToScore) ->size_t
	{
		int iscore = 0;
		for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
		{
			size_t iPreviouseDaysInARow = 0;
			size_t iDinnerAndMovie = 0;//tracks instances of dinner and a movie
			for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods-1;j++)
			{
				int iDaysInARow = 0;
				iDinnerAndMovie += scheduleData.iNumberOfSubPeriods[i] > 1; //increment for D&M
				iDaysInARow +=
					(scheduleToScore[j + iDinnerAndMovie].first == i || scheduleToScore[j + iDinnerAndMovie].second == i) &&
					(scheduleToScore[j + iDinnerAndMovie + 1].first == i || scheduleToScore[j + iDinnerAndMovie+1].second ==i);
				//assign score for days in a row near desired days in a row
				int iTmpValue = ((iDaysInARow == 0 || j + iDinnerAndMovie == scheduleData.iTotalNumberOfSubPeriods) * //check for end of consecutive scheduled priods and end of calender
					(iPreviouseDaysInARow > 0) * // check that the day previouse had member on schedule
					iPreviouseDaysInARow - availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey]); //find differnece between desired consecutive periods and actual consecutive periods
				iscore += std::abs(iTmpValue) * iPrefNumOfAlertDaysInRow_Score; //assigne score based on differece between actual and desired
				//penalize for exceeding max legal days
				iscore +=
					((iDaysInARow == 0 || j + iDinnerAndMovie == scheduleData.iTotalNumberOfSubPeriods) * iPreviouseDaysInARow > MAXDAYS) * iMAXDAYSPenalty;//TODO chnagee this
				//penalize for exeeding max prefered days, penelty is prefered days in a row penalty * 2
				iscore +=
					((iDaysInARow == 0 || j + iDinnerAndMovie == scheduleData.iTotalNumberOfSubPeriods) * //check for end of consecutive scheduled priods and end of calender
						iPreviouseDaysInARow > availabilityData.ppIntPrefArray[i][iMaxNumberOfDaysKey]) * //check if prefered maximum number of days in a row was exceeded
						((iDaysInARow == 0 || j + iDinnerAndMovie == scheduleData.iTotalNumberOfSubPeriods) * //check for end of consecutivly scheduled period
						iPreviouseDaysInARow - availabilityData.ppIntPrefArray[i][iMaxNumberOfDaysKey]) * //find difference between max and actual consecutively scheduled periods
						iMaxNumOfAlertDays_Score; // multiply by appropriate score
				iPreviouseDaysInARow = iDaysInARow;

			}
		}
		return iscore;
	});
	//Alert-Before-Gray-Day_Score,Dinner-and-Movie_Score,2-Supers_Score	2-SOFs_Score	Scheduled-On-Desired-Day_Score	Scheduled-On-UnDesired-Day_Score	D&M-On-Desired-Day_Score	Bucket-Scheduled-Difference_Score	Too-Many-Grey-Days_Score	DSG-Scheduled-On-Available-Day_Score
	//function to score on off on penelty
	funcs.push_back([&](std::pair<size_t, size_t>* scheduleToScore) ->size_t
	{
		int iscore = 0;
		for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
		{
			size_t iDinnerAndMovie = 0;//Tracks dinner & movie occurences
			size_t iOnOffOn = 0; //Tracks On-Off-On occurences;
			for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods - 1;j++)
			{
				if (scheduleToScore[j + iDinnerAndMovie + 1].first == i)
				{
					if (scheduleData.iNumberOfSubPeriods[j] > 1)//Check for Back to back D&M
					{
						//Check Brave 1 D&M to D&M
						if (scheduleData.iNumberOfSubPeriods[j + 1] > 1)//Check for Back to back D&M day
						{
							if ((scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 3].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
								(scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].second))
							{
								iOnOffOn++;
							}
						}
						iDinnerAndMovie++;
					}
				}

				if (scheduleToScore[j + iDinnerAndMovie + 1].second == i)
				{

					if (scheduleData.iNumberOfSubPeriods[j] > 1)//Check for Back to back D&M
					{
						if (scheduleData.iNumberOfSubPeriods[j + 1] > 1)//Check for Back to back D&M day
						{
							//Check Brave 2 D&M to D&M
							if ((scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 3].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
								(scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].second))
							{
								iOnOffOn++;
							}
						}
						iDinnerAndMovie++;
					}
				}
			}

			for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods - 2;j++)
			{
				//D&M
				if (scheduleData.iNumberOfSubPeriods[j] > 1) 
				{
					//D&M-D&M
					if (scheduleData.iNumberOfSubPeriods[j+1] > 1)
					{
						//handles D&M - D&M - D&M
						if (scheduleData.iNumberOfSubPeriods[j + 2] > 1)
						{
							if (scheduleToScore[j + iDinnerAndMovie + 1].first == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 5].first) ||
									(scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 5].second) &&
									((scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
									(scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].second) &&
										(scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 3].first) ||
										(scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 3].second)))
								{
									iOnOffOn++;
								}
							}
							if (scheduleToScore[j + iDinnerAndMovie + 1].second == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 5].first) ||
									(scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 5].second) &&
									((scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
									(scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].second) &&
										(scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 3].first) ||
										(scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 3].second)))
								{
									iOnOffOn++;
								}
							}
						}
						//handles D&M - D&M - N
						else
						{
							if (scheduleToScore[j + iDinnerAndMovie + 1].first == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 4].first) ||
									(scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 4].second) &&
									((scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
									(scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].second) &&
										(scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 3].first) ||
										(scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 3].second)))
								{
									iOnOffOn++;
								}
							}
							if (scheduleToScore[j + iDinnerAndMovie + 1].second == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 4].first) ||
									(scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 4].second) &&
									((scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
									(scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].second) &&
									(scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 3].first) ||
									(scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 3].second)))
								{
									iOnOffOn++;
								}
							}
						}
					}
					//handles D&M - N - D&M
					else if (scheduleData.iNumberOfSubPeriods[j + 2] > 1)
					{
						if (scheduleToScore[j + iDinnerAndMovie + 1].first == i)
						{
							if ((scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 4].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 4].second) &&
								((scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].second) &&
								(scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 3].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 3].second)))
							{
								iOnOffOn++;
							}
						}
						if (scheduleToScore[j + iDinnerAndMovie + 1].second == i)
						{
							if ((scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 4].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 4].second) &&
								((scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].second) &&
								(scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 3].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 3].second)))
							{
								iOnOffOn++;
							}
						}
					}
					//handles D&M - N - N
					else
					{
						if (scheduleToScore[j + iDinnerAndMovie + 1].first == i)
						{
							if ((scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 3].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
								((scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].second)))
							{
								iOnOffOn++;
							}
						}
						if (scheduleToScore[j + iDinnerAndMovie + 1].second == i)
						{
							if ((scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 3].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
								((scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
								(scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].second)))
							{
								iOnOffOn++;
							}
						}
					}
				}
				else
				//N-...
				{ 
					//N-D&M-....
					if (scheduleData.iNumberOfSubPeriods[j+1] > 1)
					{
						//N-D&M-D&M
						if (scheduleData.iNumberOfSubPeriods[j + 2] > 1)
						{
							if (scheduleToScore[j + iDinnerAndMovie].first == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 4].first) ||
									(scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 4].second) &&
									((scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].first) ||
									(scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].second) &&
									(scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
									(scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 2].second)))
								{
									iOnOffOn++;
								}
							}
							if (scheduleToScore[j + iDinnerAndMovie].second == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 4].first) ||
									(scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 4].second) &&
									((scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].first) ||
									(scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].second) &&
									(scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
									(scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 2].second)))
								{
									iOnOffOn++;
								}
							}
						}
						//N-D&M-N
						else
						{
							if (scheduleToScore[j + iDinnerAndMovie].first == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 3].first) ||
									(scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
									((scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].first) ||
									(scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].second) &&
									(scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
									(scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 2].second)))
								{
									iOnOffOn++;
								}
							}
							if (scheduleToScore[j + iDinnerAndMovie].first == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 3].first) ||
									(scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
									((scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].first) ||
									(scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].second) &&
									(scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 2].first) ||
									(scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 2].second)))
								{
									iOnOffOn++;
								}
							}
						}
					}

					//N-N...
					else
					{
						//N-N-D&M
						if (scheduleData.iNumberOfSubPeriods[j + 2] > 1)
						{
							if (scheduleToScore[j + iDinnerAndMovie].first == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 3].first) ||
									(scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
									((scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].first) ||
									(scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].second)))
								{
									iOnOffOn++;
								}
							}
							if (scheduleToScore[j + iDinnerAndMovie].second == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 3].first) ||
									(scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
									((scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].first) ||
									(scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].second)))
								{
									iOnOffOn++;
								}
							}
						}
						else
						{
							if (scheduleToScore[j + iDinnerAndMovie].first == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 2].first) ||
									(scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 2].second) &&
									((scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].first) ||
									(scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].second)))
								{
									iOnOffOn++;
								}
							}
							if (scheduleToScore[j + iDinnerAndMovie].second == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 2].first) ||
									(scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 2].second) &&
									((scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].first) ||
									(scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].second)))
								{
									iOnOffOn++;
								}
							}
						}
					}
				}
			}
			iscore += -1 * iOnOffOn * iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
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
