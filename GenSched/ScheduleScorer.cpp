#include "pch.h"
#include "ScheduleScorer.h"

ScheduleScorer::ScheduleScorer(AvailabilityData &availabilityData, ScheduleData &scheduleData, std::vector<ScheduleScoreData> &vctScheduleScoreData) :
	bFinalSchedules(false),
	iDesiredNumberOfDaysKey(FindMapKeyFromValue(L"Des#", availabilityData.mapNumberPrefType)),
	iPreferedNumberOfDaysInRowKey(FindMapKeyFromValue(L"Pref#", availabilityData.mapNumberPrefType)),
	iMaxNumberOfDaysKey(FindMapKeyFromValue(L"Max#", availabilityData.mapNumberPrefType)),
	iOnOffOnPrefKey(FindMapKeyFromValue(L"O-O-O", availabilityData.mapNumberPrefType)),
	iAlertBeforeGreyDayPrefKey(FindMapKeyFromValue(L"Alert Before Grey Day", availabilityData.mapNumberPrefType)),
	iAlertOnGreyDayPrefKey(FindMapKeyFromValue(L"Alert On Grey Day", availabilityData.mapNumberPrefType)),
	iDinnerAndMovie1stHalfPrefKey(FindMapKeyFromValue(L"D&M", availabilityData.mapNumberPrefType)),
	iGreaterThanDesiredNumberOfDaysScoreKey(FindMapKeyFromValue(L">Des#_Score", availabilityData.mapNumberScorableType)),
	iLessThanDesiredNumberOfDaysScoreKey(FindMapKeyFromValue(L"<Des#_Score", availabilityData.mapNumberScorableType)),
	iDSGGreaterThanDesiredNumberOfDaysScoreKey(FindMapKeyFromValue(L"DSG>Des#_Score", availabilityData.mapNumberScorableType)),
	iGreaterThanPreferedNumberOfDaysInRowScoreKey(FindMapKeyFromValue(L">Pref#_Score", availabilityData.mapNumberScorableType)),
	iLessThanPreferedNumberOfDaysInRowScoreKey(FindMapKeyFromValue(L"<Pref#_Score", availabilityData.mapNumberScorableType)),
	iMaxNumberOfDaysScoreKey(FindMapKeyFromValue(L"Max#_Score", availabilityData.mapNumberScorableType)),
	iOnOffOnScoreKey(FindMapKeyFromValue(L"O-O-O_Score", availabilityData.mapNumberScorableType)),
	iAlertBeforeGreyDayScoreKey(FindMapKeyFromValue(L"Alert Before Grey Day_Score", availabilityData.mapNumberScorableType)),
	iAlertOnGreyDayScoreKey(FindMapKeyFromValue(L"Alert On Grey Day_Score", availabilityData.mapNumberScorableType)),
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

	iGreaterThanDesNumOfAlertDays_Score(availabilityData.mapScorableNumToScore.find(iGreaterThanDesiredNumberOfDaysScoreKey)->second),
	iLessThanDesNumOfAlertDays_Score(availabilityData.mapScorableNumToScore.find(iLessThanDesiredNumberOfDaysScoreKey)->second),
	iDSGGreaterThanDesNumOfAlertDays_Score(availabilityData.mapScorableNumToScore.find(iDSGGreaterThanDesiredNumberOfDaysScoreKey)->second),
	iGreaterThanPrefNumOfAlertDaysInRow_Score(availabilityData.mapScorableNumToScore.find(iGreaterThanPreferedNumberOfDaysInRowScoreKey)->second),
	iLessThanPrefNumOfAlertDaysInRow_Score(availabilityData.mapScorableNumToScore.find(iLessThanPreferedNumberOfDaysInRowScoreKey)->second),
	iMaxNumOfAlertDays_Score(availabilityData.mapScorableNumToScore.find(iMaxNumberOfDaysScoreKey)->second),
	iOnOffOn_Score(availabilityData.mapScorableNumToScore.find(iOnOffOnScoreKey)->second),
	iAlertBeforeGreyDay_Score(availabilityData.mapScorableNumToScore.find(iAlertBeforeGreyDayScoreKey)->second),
	iAlertOnGreyDay_Score(availabilityData.mapScorableNumToScore.find(iAlertOnGreyDayScoreKey)->second),
	iDinnerAndMovie1stHalf_Score(availabilityData.mapScorableNumToScore.find(iDinnerAndMovie1stHalfScoreKey)->second),
	i2SOFs_Score(availabilityData.mapScorableNumToScore.find(i2SOFSScoreKey)->second),
	i2Supers_Score(availabilityData.mapScorableNumToScore.find(i2SupersScoreKey)->second),
	iScheduledOnDesiredDay_Score(availabilityData.mapScorableNumToScore.find(iScheduledOnDesiredDayScoreKey)->second),
	iScheduledOnUnDesiredDay_Score(availabilityData.mapScorableNumToScore.find(iScheduledOnUnDesiredDayScoreKey)->second),
	iDinnerAndMovieOnDesiredDay_Score(availabilityData.mapScorableNumToScore.find(iDinnerAndMovieOnDesiredDayScoreKey)->second),
	iDifferenceFromBucket_Score(availabilityData.mapScorableNumToScore.find(iBucketScheduledDaysDifferenceScoreKey)->second),
	iDSGScheduledOnAvailableDay_Score(availabilityData.mapScorableNumToScore.find(iDSGScheduledOnDesiredDayScoreKey)->second),
	iTooManyGreyDays_Score(availabilityData.mapScorableNumToScore.find(iBucketTooManyGreyDayScoreKey)->second),
	iPositionSwap_Score(availabilityData.mapScorableNumToScore.find(iPositionSwapScoreKey)->second),
	iScheduledWithDesiredWingman_Score(availabilityData.mapScorableNumToScore.find(iScheduledWithDesiredWingmanScoreKey)->second),
	availabilityData(availabilityData),
	scheduleData(scheduleData),
	vctScheduleScoreData(&vctScheduleScoreData)
{
	PopulateScoreFunctions();
}
void ScheduleScorer::SetFinalScheduleFlag(bool bIsScoringFinalSchedules)
{
	bFinalSchedules = bIsScoringFinalSchedules;
}

std::vector<std::function<size_t(std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember)>> ScheduleScorer::getFuncs()
{
	return funcs; 
}


void ScheduleScorer::PopulateScoreFunctions()
{
	//Function to score desired number of days vs scheduled number of days
	funcs.push_back([&](std::vector<std::pair<size_t,size_t>> scheduleToScore, size_t iPopulationMember) ->size_t
	{
		try
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
					iDinnerAndMovie += scheduleData.iNumberOfSubPeriods[j] > 1; //increment D&M count
					iAlertCount[i] +=
						(scheduleData.iNumberOfSubPeriods[j] > 1 &&
						(
							scheduleToScore[j + iDinnerAndMovie].first == i || //increment alert count on D&M day
							scheduleToScore[j + iDinnerAndMovie - 1].first == i ||
							scheduleToScore[j + iDinnerAndMovie].second == i ||
							scheduleToScore[j + iDinnerAndMovie - 1].second == i
							)) ||
							(
						(scheduleData.iNumberOfSubPeriods[j] <= 1 && //increment alert count on non-D&M day
							scheduleToScore[j + iDinnerAndMovie].first == i) ||
							(scheduleData.iNumberOfSubPeriods[j] <= 1 &&
								scheduleToScore[j + iDinnerAndMovie].second == i)
							);
				}
			}
			int iscore = 0;
			for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
			{
				//difference between requested and scheduled number of days
				int iDesiredDelta = iAlertCount[i] - availabilityData.ppIntPrefArray[i][iDesiredNumberOfDaysKey];
				//Update ScheduleScoreData if final schedule
				if(bFinalSchedules && iDesiredDelta > 0)
				{
					std::pair<std::pair<size_t, std::wstring>, int> pairDataToAdd;
					pairDataToAdd.first.first = i;
					pairDataToAdd.first.second = L"More alerts than desired by " + std::to_wstring(iDesiredDelta);
					pairDataToAdd.second = iDesiredDelta*iGreaterThanDesNumOfAlertDays_Score;
					mtxNameSpecific.lock();
					vctScheduleScoreData->at(iPopulationMember).vctNameSpecific.push_back(pairDataToAdd);
					mtxNameSpecific.unlock();
				}
				else if (bFinalSchedules && iDesiredDelta < 0)
				{
					std::pair<std::pair<size_t, std::wstring>, int> pairDataToAdd;
					pairDataToAdd.first.first = i;
					pairDataToAdd.first.second = L"Less alerts than desired by " + std::to_wstring(iDesiredDelta * -1);
					pairDataToAdd.second = std::abs(iDesiredDelta)*iLessThanDesNumOfAlertDays_Score;
					mtxNameSpecific.lock();
					vctScheduleScoreData->at(iPopulationMember).vctNameSpecific.push_back(pairDataToAdd);
					mtxNameSpecific.unlock();
				}
				//Difference between bucket requrirment and days scheduled adjusted for leave and TDY
				int iBucketDelta = iAlertCount[i] - (availabilityData.mapNameNumToBucket.find(i)->second + 1) * iDaysAvailable[i] / scheduleData.iNumberOfAvailabilityPeriods;
				if (bFinalSchedules && iBucketDelta > 0)
				{
					std::pair<std::pair<size_t, std::wstring>, int> pairDataToAdd;
					pairDataToAdd.first.first = i;
					pairDataToAdd.first.second = L"More alerts than bucket by " + std::to_wstring(iBucketDelta);
					pairDataToAdd.second = iBucketDelta*iDifferenceFromBucket_Score;
					mtxNameSpecific.lock();
					vctScheduleScoreData->at(iPopulationMember).vctNameSpecific.push_back(pairDataToAdd);
					mtxNameSpecific.unlock();
				}
				else if (bFinalSchedules && iBucketDelta < 0)
				{
					std::pair<std::pair<size_t, std::wstring>, int> pairDataToAdd;
					pairDataToAdd.first.first = i;
					pairDataToAdd.first.second = L"Less alerts than bucket by " + std::to_wstring(iBucketDelta * -1);
					pairDataToAdd.second = -1*iBucketDelta*iDifferenceFromBucket_Score;
					mtxNameSpecific.lock();
					vctScheduleScoreData->at(iPopulationMember).vctNameSpecific.push_back(pairDataToAdd);
					mtxNameSpecific.unlock();
				}
				if (iDesiredDelta > 0)
				{
					iscore += iDesiredDelta * iGreaterThanDesNumOfAlertDays_Score;
				}
				else if (iDesiredDelta < 0)
				{
					iscore += -1 * iDesiredDelta * iGreaterThanDesNumOfAlertDays_Score;
				}
				if (iBucketDelta > 0)
				{
					iscore += iDesiredDelta * iGreaterThanDesNumOfAlertDays_Score;
				}
				else if (iBucketDelta < 0)
				{
					iscore += -1 * iDesiredDelta * iDifferenceFromBucket_Score;
				}
			}
			delete iAlertCount;
			delete iDaysAvailable;
			return iscore;
		}
		catch (std::exception const & e)
		{
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown Exception";
			OutputDebugString(msg->Data());
		}
	});

	//function to score prefered number of days in a row and max number days in a row
	funcs.push_back([&](std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember) ->size_t
	{
		try
		{
			int iscore = 0;
			for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
			{
				int iDaysInARow = 0;

				size_t iDinnerAndMovie = 0;//tracks instances of dinner and a movie
				for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods;j++)
				{
					bool bScheduledToday = false;
					bool bScheduledYesterday = false;
					size_t iTempDaysInARow = 0;
					if (scheduleToScore[j + iDinnerAndMovie].first == i || scheduleToScore[j + iDinnerAndMovie].second == i)
					{
						bScheduledToday = true;
					}
					else if (scheduleData.iNumberOfSubPeriods[j] > 1 && (scheduleToScore[j + iDinnerAndMovie+1].first == i || scheduleToScore[j + iDinnerAndMovie+1].second == i))
					{
						bScheduledToday = true;
					}
					else
					{
						bScheduledToday = false;
					}
					if ((j + iDinnerAndMovie) > 0 && (scheduleToScore[j + iDinnerAndMovie - 1].first == i || scheduleToScore[j + iDinnerAndMovie - 1].second == i))
					{
						bScheduledYesterday = true;//doesn't count if on AM yesterday but not PM
					}
					else
					{
						bScheduledYesterday = false;
					}
					if (bScheduledToday)
					{
						iDaysInARow++;
					}
					if (bScheduledYesterday && (!bScheduledToday || j == availabilityData.iNumberOfAvailabilityPeriods-1))
					{
						size_t iSubtraction = 1;
						if (j== availabilityData.iNumberOfAvailabilityPeriods - 1 && bScheduledToday) { iSubtraction = 0; }
						//Penalize for difference between desired and scheduled days in a row
						if (iDaysInARow - availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey] > 0)
						{
							iscore += (iDaysInARow - availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey] )* iGreaterThanPrefNumOfAlertDaysInRow_Score;
						}
						else if (iDaysInARow - availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey] < 0)
						{
							iscore += -1* (iDaysInARow - availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey]) * iLessThanPrefNumOfAlertDaysInRow_Score;
						}

						if (bFinalSchedules && (iDaysInARow - availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey]) > 0)
						{
							std::pair<std::pair<size_t, size_t>, std::pair<std::wstring, int>> pairDataToAdd;
							pairDataToAdd.first.first = j+iDinnerAndMovie - iSubtraction;
							pairDataToAdd.first.second = i;
							pairDataToAdd.second.first = L"Days in a row greater than desired by "
								+ std::to_wstring(iDaysInARow - availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey]);
							pairDataToAdd.second.second = (iDaysInARow - availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey]) 
								* iGreaterThanPrefNumOfAlertDaysInRow_Score;
							mtxDateAndNameSpecific.lock();
							vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
							mtxDateAndNameSpecific.unlock();
						}
						else if (bFinalSchedules && (iDaysInARow - availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey]) < 0)
						{
							std::pair<std::pair<size_t, size_t>, std::pair<std::wstring, int>> pairDataToAdd;
							pairDataToAdd.first.first = j + iDinnerAndMovie - iSubtraction;
							pairDataToAdd.first.second = i;
							pairDataToAdd.second.first = L"Days in a row less than desired by "
								+ std::to_wstring(-1*(iDaysInARow - availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey]));
							pairDataToAdd.second.second = - 1* (iDaysInARow - availabilityData.ppIntPrefArray[i][iPreferedNumberOfDaysInRowKey])
								* iLessThanPrefNumOfAlertDaysInRow_Score;
							mtxDateAndNameSpecific.lock();
							vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
							mtxDateAndNameSpecific.unlock();
						}
						if (iDaysInARow - availabilityData.ppIntPrefArray[i][iMaxNumberOfDaysKey] > 0)//Penalize for exceeding max desired days in a row
						{
							iscore += (iDaysInARow - availabilityData.ppIntPrefArray[i][iMaxNumberOfDaysKey]* iMaxNumOfAlertDays_Score);
							if (bFinalSchedules && (iDaysInARow - availabilityData.ppIntPrefArray[i][iMaxNumberOfDaysKey]) * iMaxNumOfAlertDays_Score != 0)
							{
								std::pair<std::pair<size_t, size_t>, std::pair<std::wstring, int>> pairDataToAdd;
								pairDataToAdd.first.first = j + iDinnerAndMovie - iSubtraction;
								pairDataToAdd.first.second = i;
								pairDataToAdd.second.first = L"Days in a row greater than personal max by "
									+ std::to_wstring(iDaysInARow - availabilityData.ppIntPrefArray[i][iMaxNumberOfDaysKey]);
								pairDataToAdd.second.second = (iDaysInARow - availabilityData.ppIntPrefArray[i][iMaxNumberOfDaysKey])
									* iMaxNumOfAlertDays_Score;
								mtxDateAndNameSpecific.lock();
								vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
								mtxDateAndNameSpecific.unlock();
							}
						}
						if (iDaysInARow > MAXDAYS)//Penalize for exceeding max days in a row
						{
							iscore += (iDaysInARow - MAXDAYS) * iMAXDAYS_Penalty;
							if (bFinalSchedules && (iDaysInARow - MAXDAYS) * iMAXDAYS_Penalty != 0)
							{
								std::pair<std::pair<size_t, size_t>, std::pair<std::wstring, int>> pairDataToAdd;
								pairDataToAdd.first.first = j + iDinnerAndMovie - iSubtraction;
								pairDataToAdd.first.second = i;
								pairDataToAdd.second.first = L"Days in a row greater than allowed max by "
									+ std::to_wstring(iDaysInARow - MAXDAYS);
								pairDataToAdd.second.second = iDaysInARow - MAXDAYS
									* iMAXDAYS_Penalty;
								mtxDateAndNameSpecific.lock();
								vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
								mtxDateAndNameSpecific.unlock();
							}
						}
					}
					if (!bScheduledToday)
					{
						iDaysInARow = 0;
					}
					iDinnerAndMovie += scheduleData.iNumberOfSubPeriods[j] > 1; //increment for D&M
				}
			}
			return iscore;
		}
		catch (std::exception const & e)
		{
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown Exception";
			OutputDebugString(msg->Data());
		}

	});
	//function to score on-off-on penalty
	funcs.push_back([&](std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember) ->size_t
	{
		try {
			int iscore = 0;
			std::pair<std::pair<size_t, size_t>, std::pair<std::wstring, int>> pairDataToAdd;
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
								if ((scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 3].first ||
									scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
									scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].first &&
									scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].second)
								{
									iOnOffOn++;
									if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey]!=0)
									{
										pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
										pairDataToAdd.first.second = i;
										pairDataToAdd.second.first = L"On-Off-On";
										pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
										mtxDateAndNameSpecific.lock();
										vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
										mtxDateAndNameSpecific.unlock();
									}
								}
							}

						}
					}

					if (scheduleToScore[j + iDinnerAndMovie + 1].second == i)
					{

						if (scheduleData.iNumberOfSubPeriods[j] > 1)//Check for D&M
						{
							if (scheduleData.iNumberOfSubPeriods[j + 1] > 1)//Check for Back to back D&M day
							{
								//Check Brave 2 D&M to D&M
								if ((scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 3].first ||
									scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
									scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].first &&
									scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].second)
								{
									iOnOffOn++;
									if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
									{
										pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
										pairDataToAdd.first.second = i;
										pairDataToAdd.second.first = L"On-Off-On";
										pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
										mtxDateAndNameSpecific.lock();
										vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
										mtxDateAndNameSpecific.unlock();
									}
								}
							}

						}
					}
					if (scheduleData.iNumberOfSubPeriods[j] > 1)
					{
						iDinnerAndMovie++;
					}
				}
				iDinnerAndMovie = 0;//reset D&M counter
				for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods - 2;j++)
				{
					//D&M
					if (scheduleData.iNumberOfSubPeriods[j] > 1)
					{
						//D&M-D&M
						if (scheduleData.iNumberOfSubPeriods[j + 1] > 1)
						{
							//handles D&M - D&M - D&M
							if (scheduleData.iNumberOfSubPeriods[j + 2] > 1)
							{
								if (scheduleToScore[j + iDinnerAndMovie + 1].first == i)
								{
									if ((scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 5].first ||
										scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 5].second) &&
										scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].first &&
										scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].second &&
										scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 3].first &&
										scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 3].second)
									{
										iOnOffOn++;
										if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
										{
											pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
											pairDataToAdd.first.second = i;
											pairDataToAdd.second.first = L"On-Off-On";
											pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
											mtxDateAndNameSpecific.lock();
											vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
											mtxDateAndNameSpecific.unlock();
										}
									}
								}
								if (scheduleToScore[j + iDinnerAndMovie + 1].second == i)
								{
									if ((scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 5].first ||
										scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 5].second) &&
										scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].first &&
										scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].second &&
										scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 3].first &&
										scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 3].second)
									{
										iOnOffOn++;
										if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
										{
											pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
											pairDataToAdd.first.second = i;
											pairDataToAdd.second.first = L"On-Off-On";
											pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
											mtxDateAndNameSpecific.lock();
											vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
											mtxDateAndNameSpecific.unlock();
										}
									}
								}
							}
							//handles D&M - D&M - N
							else
							{
								if (scheduleToScore[j + iDinnerAndMovie + 1].first == i)
								{
									if ((scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 4].first ||
										scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 4].second) &&
										scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].first &&
										scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].second &&
										scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 3].first &&
										scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 3].second)
									{
										iOnOffOn++;
										if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
										{
											pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
											pairDataToAdd.first.second = i;
											pairDataToAdd.second.first = L"On-Off-On";
											pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
											mtxDateAndNameSpecific.lock();
											vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
											mtxDateAndNameSpecific.unlock();
										}
									}
								}
								if (scheduleToScore[j + iDinnerAndMovie + 1].second == i)
								{
									if ((scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 4].first ||
										scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 4].second) &&
										scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].first &&
										scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].second &&
										scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 3].first &&
										scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 3].second)
									{
										iOnOffOn++;
										if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
										{
											pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
											pairDataToAdd.first.second = i;
											pairDataToAdd.second.first = L"On-Off-On";
											pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
											mtxDateAndNameSpecific.lock();
											vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
											mtxDateAndNameSpecific.unlock();
										}
									}
								}
							}
						}
						//handles D&M - N - D&M
						else if (scheduleData.iNumberOfSubPeriods[j + 2] > 1)
						{
							if (scheduleToScore[j + iDinnerAndMovie + 1].first == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 4].first ||
									scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 4].second) &&
									scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].first &&
									scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].second &&
									scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 3].first &&
									scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 3].second)
								{
									iOnOffOn++;
									if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
									{
										pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
										pairDataToAdd.first.second = i;
										pairDataToAdd.second.first = L"On-Off-On";
										pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
										mtxDateAndNameSpecific.lock();
										vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
										mtxDateAndNameSpecific.unlock();
									}
								}
							}
							if (scheduleToScore[j + iDinnerAndMovie + 1].second == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 4].first ||
									scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 4].second) &&
									scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].first &&
									scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].second &&
									scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 3].first &&
									scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 3].second)
								{
									iOnOffOn++;
									if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
									{
										pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
										pairDataToAdd.first.second = i;
										pairDataToAdd.second.first = L"On-Off-On";
										pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
										mtxDateAndNameSpecific.lock();
										vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
										mtxDateAndNameSpecific.unlock();
									}
								}
							}
						}
						//handles D&M - N - N
						else
						{
							if (scheduleToScore[j + iDinnerAndMovie + 1].first == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 3].first ||
									scheduleToScore[j + iDinnerAndMovie + 1].first == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
									scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].first &&
									scheduleToScore[j + iDinnerAndMovie + 1].first != scheduleToScore[j + iDinnerAndMovie + 2].second)
								{
									iOnOffOn++;
									if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
									{
										pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
										pairDataToAdd.first.second = i;
										pairDataToAdd.second.first = L"On-Off-On";
										pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
										mtxDateAndNameSpecific.lock();
										vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
										mtxDateAndNameSpecific.unlock();
									}
								}
							}
							if (scheduleToScore[j + iDinnerAndMovie + 1].second == i)
							{
								if ((scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 3].first ||
									scheduleToScore[j + iDinnerAndMovie + 1].second == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
									scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].first &&
									scheduleToScore[j + iDinnerAndMovie + 1].second != scheduleToScore[j + iDinnerAndMovie + 2].second)
								{
									iOnOffOn++;
									if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
									{
										pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
										pairDataToAdd.first.second = i;
										pairDataToAdd.second.first = L"On-Off-On";
										pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
										mtxDateAndNameSpecific.lock();
										vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
										mtxDateAndNameSpecific.unlock();
									}
								}
							}
						}
						iDinnerAndMovie++;
					}
					else
						//N-...
					{
						//N-D&M-....
						if (scheduleData.iNumberOfSubPeriods[j + 1] > 1)
						{
							//N-D&M-D&M
							if (scheduleData.iNumberOfSubPeriods[j + 2] > 1)
							{
								if (scheduleToScore[j + iDinnerAndMovie].first == i)
								{
									if ((scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 4].first ||
										scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 4].second) &&
										scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].first &&
										scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].second &&
										scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 2].first &&
										scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 2].second)
									{
										iOnOffOn++;
										if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
										{
											pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
											pairDataToAdd.first.second = i;
											pairDataToAdd.second.first = L"On-Off-On";
											pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
											mtxDateAndNameSpecific.lock();
											vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
											mtxDateAndNameSpecific.unlock();
										}
									}
								}
								if (scheduleToScore[j + iDinnerAndMovie].second == i)
								{
									if ((scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 4].first ||
										scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 4].second) &&
										scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].first &&
										scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].second &&
										scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 2].first &&
										scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 2].second)
									{
										iOnOffOn++;
										if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
										{
											pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
											pairDataToAdd.first.second = i;
											pairDataToAdd.second.first = L"On-Off-On";
											pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
											mtxDateAndNameSpecific.lock();
											vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
											mtxDateAndNameSpecific.unlock();
										}
									}
								}
							}
							//N-D&M-N
							else
							{
								if (scheduleToScore[j + iDinnerAndMovie].first == i)
								{
									if ((scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 3].first ||
										scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
										scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].first &&
										scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].second &&
										scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 2].first &&
										scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 2].second)
									{
										iOnOffOn++;
										if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
										{
											pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
											pairDataToAdd.first.second = i;
											pairDataToAdd.second.first = L"On-Off-On";
											pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
											mtxDateAndNameSpecific.lock();
											vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
											mtxDateAndNameSpecific.unlock();
										}
									}
								}
								if (scheduleToScore[j + iDinnerAndMovie].first == i)
								{
									if ((scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 3].first ||
										scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
										scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].first &&
										scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].second &&
										scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 2].first &&
										scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 2].second)
									{
										iOnOffOn++;
										if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
										{
											pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
											pairDataToAdd.first.second = i;
											pairDataToAdd.second.first = L"On-Off-On";
											pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
											mtxDateAndNameSpecific.lock();
											vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
											mtxDateAndNameSpecific.unlock();
										}
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
									if ((scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 3].first ||
										scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
										scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].first &&
										scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].second)
									{
										iOnOffOn++;
										if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
										{
											pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
											pairDataToAdd.first.second = i;
											pairDataToAdd.second.first = L"On-Off-On";
											pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
											mtxDateAndNameSpecific.lock();
											vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
											mtxDateAndNameSpecific.unlock();
										}
									}
								}
								if (scheduleToScore[j + iDinnerAndMovie].second == i)
								{
									if ((scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 3].first ||
										scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 3].second) &&
										scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].first &&
										scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].second)
									{
										iOnOffOn++;
										if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
										{
											pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
											pairDataToAdd.first.second = i;
											pairDataToAdd.second.first = L"On-Off-On";
											pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
											mtxDateAndNameSpecific.lock();
											vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
											mtxDateAndNameSpecific.unlock();
										}
									}
								}
							}
							else
							{
								if (scheduleToScore[j + iDinnerAndMovie].first == i)
								{
									if ((scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 2].first ||
										scheduleToScore[j + iDinnerAndMovie].first == scheduleToScore[j + iDinnerAndMovie + 2].second) &&
										scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].first &&
										scheduleToScore[j + iDinnerAndMovie].first != scheduleToScore[j + iDinnerAndMovie + 1].second)
									{
										iOnOffOn++;
										if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
										{
											pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
											pairDataToAdd.first.second = i;
											pairDataToAdd.second.first = L"On-Off-On";
											pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
											mtxDateAndNameSpecific.lock();
											vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
											mtxDateAndNameSpecific.unlock();
										}
									}
								}
								if (scheduleToScore[j + iDinnerAndMovie].second == i)
								{
									if ((scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 2].first ||
										scheduleToScore[j + iDinnerAndMovie].second == scheduleToScore[j + iDinnerAndMovie + 2].second) &&
										scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].first &&
										scheduleToScore[j + iDinnerAndMovie].second != scheduleToScore[j + iDinnerAndMovie + 1].second)
									{
										iOnOffOn++;
										if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey] != 0)
										{
											pairDataToAdd.first.first = j + iDinnerAndMovie + 2;
											pairDataToAdd.first.second = i;
											pairDataToAdd.second.first = L"On-Off-On";
											pairDataToAdd.second.second = iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
											mtxDateAndNameSpecific.lock();
											vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
											mtxDateAndNameSpecific.unlock();
										}
									}
								}
							}
						}
					}
				}
				iscore += iOnOffOn * iOnOffOn_Score * availabilityData.ppIntPrefArray[i][iOnOffOnPrefKey];
			}
			return iscore;
		}
		catch (std::exception const & e)
		{
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown Exception";
			OutputDebugString(msg->Data());
		}
	});
	
	//function to score alert before grey-day
	funcs.push_back([&](std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember) ->size_t
	{
		try
		{
			int iscore = 0;
			for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
			{
				size_t iDinnerAndMovie = 0;//tracks instances of dinner and a movie
				size_t iAlertsBeforGreyDays = 0;
				for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods - 1;j++)
				{
					if (scheduleData.iNumberOfSubPeriods[j] > 1)
					{
						iDinnerAndMovie++;//increment D&M Count
					}
					if ((scheduleToScore[j + iDinnerAndMovie].first == i || scheduleToScore[j + iDinnerAndMovie].second == i) &&
						availabilityData.ppIntDateDayDayTypeArray[1][j + 1] == availabilityData.GREY_DAY &&
						(scheduleToScore[j + iDinnerAndMovie + 1].first != i && scheduleToScore[j + iDinnerAndMovie + 1].second != i))
					{
			
						iAlertsBeforGreyDays++;
						if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iAlertBeforeGreyDayPrefKey]!=0)
						{
							std::pair<std::pair<size_t, size_t>, std::pair<std::wstring, int>> pairDataToAdd;
							pairDataToAdd.first.first = j + iDinnerAndMovie;
							pairDataToAdd.first.second = i;
							pairDataToAdd.second.first = L"Alert Before Grey Day";
							pairDataToAdd.second.second = iAlertBeforeGreyDay_Score * availabilityData.ppIntPrefArray[i][iAlertBeforeGreyDayPrefKey];
							mtxDateAndNameSpecific.lock();
							vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
							mtxDateAndNameSpecific.unlock();
						}
					}
				}
				iscore += iAlertsBeforGreyDays * iAlertBeforeGreyDay_Score * availabilityData.ppIntPrefArray[i][iAlertBeforeGreyDayPrefKey];//Score Alert before grey days
			}
			return iscore;
		}
		catch (std::exception const & e)
		{
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown Exception";
			OutputDebugString(msg->Data());
		}
	});
	//function to score alert on grey-day
	funcs.push_back([&](std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember) ->size_t
	{
		try
		{
			int iscore = 0;
			std::pair<std::pair<size_t, size_t>, std::pair<std::wstring, int>> pairDataToAdd;
			for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
			{
				size_t iDinnerAndMovie = 0;//tracks instances of dinner and a movie
				size_t iGreyDayCount = 0;
				for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods;j++)
				{
					if (scheduleData.iNumberOfSubPeriods[j] > 1)
					{
						iDinnerAndMovie++;//increment D&M Count
						if ((scheduleToScore[j + iDinnerAndMovie].first == i || scheduleToScore[j + iDinnerAndMovie].second == i ||
							scheduleToScore[j + iDinnerAndMovie - 1].first == i || scheduleToScore[j + iDinnerAndMovie - 1].second == i) &&
							availabilityData.ppIntDateDayDayTypeArray[1][j] == availabilityData.GREY_DAY)
						{
							iGreyDayCount++;
							if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iAlertOnGreyDayPrefKey] != 0)
							{
								pairDataToAdd.first.first = j + iDinnerAndMovie;
								pairDataToAdd.first.second = i;
								pairDataToAdd.second.first = L"Alert on grey day";
								pairDataToAdd.second.second = iAlertOnGreyDay_Score * availabilityData.ppIntPrefArray[i][iAlertOnGreyDayPrefKey];
								mtxDateAndNameSpecific.lock();
								vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
								mtxDateAndNameSpecific.unlock();
							}
						}
					}
					else
					{
						if ((scheduleToScore[j + iDinnerAndMovie].first == i || scheduleToScore[j + iDinnerAndMovie].second == i) &&
							availabilityData.ppIntDateDayDayTypeArray[1][j] == availabilityData.GREY_DAY)
						{
							iGreyDayCount++;
							if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iAlertOnGreyDayPrefKey] != 0)
							{
								pairDataToAdd.first.first = j + iDinnerAndMovie;
								pairDataToAdd.first.second = i;
								pairDataToAdd.second.first = L"Alert on grey day";
								pairDataToAdd.second.second = iAlertOnGreyDay_Score * availabilityData.ppIntPrefArray[i][iAlertOnGreyDayPrefKey];
								mtxDateAndNameSpecific.lock();
								vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
								mtxDateAndNameSpecific.unlock();
							}
						}
					}
				}
				iscore += iGreyDayCount * iAlertOnGreyDay_Score * availabilityData.ppIntPrefArray[i][iAlertOnGreyDayPrefKey];//Score Alert on grey days
			}
			return iscore;
		}
		catch (std::exception const & e)
		{
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown Exception";
			OutputDebugString(msg->Data());
		}
	});
	//function to score Dinner & Movie 1st Half
	funcs.push_back([&](std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember) ->size_t
	{
		try
		{
			int iscore = 0;
			std::pair<std::pair<size_t, size_t>, std::pair<std::wstring, int>> pairDataToAdd;
			for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
			{
				size_t iDinnerAndMovie = 0;//tracks instances of dinner and a movie
				size_t iDinnerAndMovie1stHalf = 0;
				for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods - 1;j++)
				{
					if (scheduleData.iNumberOfSubPeriods[j] > 1)
					{
						if ((scheduleToScore[j + iDinnerAndMovie].first == i || scheduleToScore[j + iDinnerAndMovie].second == i) &&
							(scheduleToScore[j + iDinnerAndMovie + 1].first != i && scheduleToScore[j + iDinnerAndMovie + 1].second != i))
						{
							iDinnerAndMovie1stHalf++;
							if (bFinalSchedules && availabilityData.ppIntPrefArray[i][iDinnerAndMovie1stHalfPrefKey] != 0)
							{
								pairDataToAdd.first.first = j + iDinnerAndMovie;
								pairDataToAdd.first.second = i;
								pairDataToAdd.second.first = L"Alert on grey day";
								pairDataToAdd.second.second = iDinnerAndMovie1stHalf_Score 
									* availabilityData.ppIntPrefArray[i][iDinnerAndMovie1stHalfPrefKey];
								mtxDateAndNameSpecific.lock();
								vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
								mtxDateAndNameSpecific.unlock();
							}
						}
						iDinnerAndMovie++;//increment D&M Count
					}

				}
				iscore += iDinnerAndMovie1stHalf * iDinnerAndMovie1stHalf_Score * availabilityData.ppIntPrefArray[i][iDinnerAndMovie1stHalfPrefKey];
			}
			return iscore;
		}
		catch (std::exception const & e)
		{
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown Exception";
			OutputDebugString(msg->Data());
		}
	});
	//function to score Dinner & Movie on desired & undesired day
	funcs.push_back([&](std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember) ->size_t
	{
		try
		{
			int iscore = 0;
			for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
			{
				size_t iDinnerAndMovie = 0;//tracks instances of dinner and a movie
				size_t iDesiredDinnerAndMovieCount = 0;////tracks instances of dinner and a movie on desired day
				size_t iUnDesiredDinnerAndMovieCount = 0;////tracks instances of dinner and a movie on desired day
				std::pair<std::pair<size_t, size_t>, std::pair<std::wstring, int>> pairDataToAdd;
				for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods;j++)
				{
					if (scheduleData.iNumberOfSubPeriods[j] > 1)
					{

						iDinnerAndMovie++;//increment D&M Count
						if ((scheduleToScore[j + iDinnerAndMovie].first == i || scheduleToScore[j + iDinnerAndMovie].second == i) &&
							(scheduleToScore[j + iDinnerAndMovie - 1].first != i && scheduleToScore[j + iDinnerAndMovie - 1].second != i) &&
							availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.DINNER_AND_MOVIE)
						{
							iDesiredDinnerAndMovieCount++;
							if (bFinalSchedules && iDinnerAndMovieOnDesiredDay_Score != 0)
							{
								pairDataToAdd.first.first = j + iDinnerAndMovie;
								pairDataToAdd.first.second = i;
								pairDataToAdd.second.first = L"Desired D&M";
								pairDataToAdd.second.second = iDinnerAndMovieOnDesiredDay_Score;
								mtxDateAndNameSpecific.lock();
								vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
								mtxDateAndNameSpecific.unlock();
							}
						}
						if ((scheduleToScore[j + iDinnerAndMovie].first == i || scheduleToScore[j + iDinnerAndMovie].second == i) &&
							(scheduleToScore[j + iDinnerAndMovie - 1].first != i && scheduleToScore[j + iDinnerAndMovie - 1].second != i) &&
							availabilityData.ppIntAvailabilityTypeArray[i][j] != availabilityData.DINNER_AND_MOVIE)
						{
							iUnDesiredDinnerAndMovieCount++;
							if (bFinalSchedules && iDinnerAndMovieNotRequested_Penalty != 0)
							{
								pairDataToAdd.first.first = j + iDinnerAndMovie;
								pairDataToAdd.first.second = i;
								pairDataToAdd.second.first = L"Undesired D&M";
								pairDataToAdd.second.second = iDinnerAndMovieNotRequested_Penalty;
								mtxDateAndNameSpecific.lock();
								vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
								mtxDateAndNameSpecific.unlock();
							}
						}
					}
				}
				iscore += iDesiredDinnerAndMovieCount * iDinnerAndMovieOnDesiredDay_Score 
					+ iUnDesiredDinnerAndMovieCount * iDinnerAndMovieNotRequested_Penalty;
			}
			return iscore;
		}
		catch (std::exception const & e)
		{
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown Exception";
			OutputDebugString(msg->Data());
		}
	});
	//function to score scheduled on desired day or undesired day
	funcs.push_back([&](std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember) ->size_t
	{
		try
		{
			int iscore = 0;
			std::pair<std::pair<size_t, size_t>, std::pair<std::wstring, int>> pairDataToAdd;
			for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
			{
				size_t iDinnerAndMovie = 0;//tracks instances of dinner and a movie
				size_t iDesiredAlerts = 0;////tracks instances alert on desired day
				size_t iUnDesiredAlerts = 0;////tracks instances of alert on un-desired day

				for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods;j++)
				{
					int iDesiredAlertsScore = 0;////tracks instances of alert on desired day score 
					int iUnDesiredAlertsScore = 0;////tracks instances of alert on un-desired day score
					//Find desired alerts
					if ((scheduleToScore[j + iDinnerAndMovie].first == i || scheduleToScore[j + iDinnerAndMovie].second == i) &&
						(availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.ALERT_PLEASE ||
						availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.ALERT_IS_FINE ||
						availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.DONT_CARE))
					{
						if (availabilityData.ppBoolQualArray[i][iDSGKey] == true)
						{
							iDesiredAlertsScore += availabilityData.ppIntAvailabilityTypeArray[i][j] * iDSGScheduledOnAvailableDay_Score;
							if (bFinalSchedules && availabilityData.ppIntAvailabilityTypeArray[i][j] * iDSGScheduledOnAvailableDay_Score !=0)
							{
								pairDataToAdd.first.first = j + iDinnerAndMovie;
								pairDataToAdd.first.second = i;
								if (availabilityData.ppIntAvailabilityTypeArray[i][j]==availabilityData.ALERT_PLEASE)
								{
									pairDataToAdd.second.first = L"DSG Scheduled on \"Alert Please\" day";
								}
								else if (availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.ALERT_IS_FINE)
								{
									pairDataToAdd.second.first = L"DSG Scheduled on \"Alert Is Fine\" day";
								}
								else if (availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.DONT_CARE)
								{
									pairDataToAdd.second.first = L"DSG Scheduled on \"Don't Care\" day";
								}
								pairDataToAdd.second.second = availabilityData.ppIntAvailabilityTypeArray[i][j] * iDSGScheduledOnAvailableDay_Score;
								mtxDateAndNameSpecific.lock();
								vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
								mtxDateAndNameSpecific.unlock();
							}
						}
						else
						{
							iDesiredAlertsScore += availabilityData.ppIntAvailabilityTypeArray[i][j] * iScheduledOnDesiredDay_Score;
							if (bFinalSchedules && availabilityData.ppIntAvailabilityTypeArray[i][j] * iDSGScheduledOnAvailableDay_Score != 0)
							{
								pairDataToAdd.first.first = j + iDinnerAndMovie;
								pairDataToAdd.first.second = i;
								if (availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.ALERT_PLEASE)
								{
									pairDataToAdd.second.first = L"Scheduled on \"Alert Please\" day.";
								}
								else if (availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.ALERT_IS_FINE)
								{
									pairDataToAdd.second.first = L"Scheduled on \"Alert Is Fine\" day.";
								}
								else if (availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.DONT_CARE)
								{
									pairDataToAdd.second.first = L"Scheduled on \"Don't Care\" day.";
								}
								pairDataToAdd.second.second = availabilityData.ppIntAvailabilityTypeArray[i][j] * iScheduledOnDesiredDay_Score;
								mtxDateAndNameSpecific.lock();
								vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
								mtxDateAndNameSpecific.unlock();
							}
						}
						iDesiredAlerts++;
					}
					//Find un-desired alerts
					else if ((scheduleToScore[j + iDinnerAndMovie].first == i || scheduleToScore[j + iDinnerAndMovie].second == i) &&
						(availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.RATHER_NOT ||
						availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.PLEASE_NO))
					{
						iUnDesiredAlertsScore += availabilityData.ppIntAvailabilityTypeArray[i][j] * iScheduledOnUnDesiredDay_Score;
						if (bFinalSchedules && availabilityData.ppIntAvailabilityTypeArray[i][j] * iDSGScheduledOnAvailableDay_Score != 0)
						{
							pairDataToAdd.first.first = j + iDinnerAndMovie;
							pairDataToAdd.first.second = i;
							if (availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.RATHER_NOT)
							{
								pairDataToAdd.second.first = L"Scheduled on \"Rather Not\" day.";
							}
							else if (availabilityData.ppIntAvailabilityTypeArray[i][j] == availabilityData.PLEASE_NO)
							{
								pairDataToAdd.second.first = L"Scheduled on \"Please No\" day.";
							}
							pairDataToAdd.second.second = -1 * availabilityData.ppIntAvailabilityTypeArray[i][j] * iScheduledOnUnDesiredDay_Score;
							mtxDateAndNameSpecific.lock();
							vctScheduleScoreData->at(iPopulationMember).vctDateAndNameSpecific.push_back(pairDataToAdd);
							mtxDateAndNameSpecific.unlock();
						}
						iUnDesiredAlerts++;
					}
					if (scheduleData.iNumberOfSubPeriods[j] > 1)
					{
						iDinnerAndMovie++;//increment D&M Count
					}
					iscore += iDesiredAlertsScore - iUnDesiredAlertsScore;
				}
			}
			return iscore;
		}
		catch (std::exception const & e)
		{
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown Exception";
			OutputDebugString(msg->Data());
		}
	});
	//function to score 2 supers or SOFs scheduled at once
	funcs.push_back([&](std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember) ->size_t
	{
		try
		{
			int iscore = 0;
			std::pair<std::size_t, std::pair<std::wstring, int>> pairDataToAdd;
			size_t iDinnerAndMovie = 0;//tracks instances of dinner and a movie
			size_t i2SupersCount = 0;////tracks instances of dinner and a movie on desired day
			size_t i2SOFsCount = 0;////tracks instances of dinner and a movie on desired day
			for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityPeriods;i++)
			{
				if (availabilityData.ppBoolQualArray[scheduleToScore[i + iDinnerAndMovie].first][iSOFKey] == true &&
					availabilityData.ppBoolQualArray[scheduleToScore[i + iDinnerAndMovie].second][iSOFKey] == true)
				{
					i2SOFsCount++;
					if (bFinalSchedules && i2SOFs_Score !=0)
					{
						pairDataToAdd.first = i + iDinnerAndMovie;
						pairDataToAdd.second.first = L"2 SOFs scheduled on same day";
						pairDataToAdd.second.second = i2SOFs_Score;
						mtxDateSpecific.lock();
						vctScheduleScoreData->at(iPopulationMember).vctDateSpecific.push_back(pairDataToAdd);
						mtxDateSpecific.unlock();
					}
				}
				if (availabilityData.ppBoolQualArray[scheduleToScore[i + iDinnerAndMovie].first][iSuperKey] == true &&
					availabilityData.ppBoolQualArray[scheduleToScore[i + iDinnerAndMovie].second][iSuperKey] == true)
				{
					i2SupersCount++;
					if (bFinalSchedules && i2Supers_Score != 0)
					{
						pairDataToAdd.first = i + iDinnerAndMovie;
						pairDataToAdd.second.first = L"2 Supervisors scheduled on same day";
						pairDataToAdd.second.second = i2Supers_Score;
						mtxDateSpecific.lock();
						vctScheduleScoreData->at(iPopulationMember).vctDateSpecific.push_back(pairDataToAdd);
						mtxDateSpecific.unlock();
					}
				}
				if (scheduleData.iNumberOfSubPeriods[i] > 1)
				{
					iDinnerAndMovie++;//increment D&M Count
				}
			}
			iscore += i2SupersCount * i2Supers_Score + i2SOFsCount * i2SOFs_Score;
			return iscore;
		}
		catch (std::exception const & e)
		{
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown Exception";
			OutputDebugString(msg->Data());
		}
	});
	//function to score position swaps
	funcs.push_back([&](std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember) ->size_t
	{
		try
		{
			int iscore = 0;
			std::pair<std::size_t, std::pair<std::wstring, int>> pairDataToAdd;
			size_t iDinnerAndMovie = 0;//tracks instances of dinner and a movie
			size_t iSwapCount = 0;
			for (size_t i = 0;i < scheduleData.iTotalNumberOfSubPeriods - 1;i++)
			{
				if (scheduleToScore[i].first == scheduleToScore[i + 1].second)
				{
					iSwapCount++;
					if (bFinalSchedules && iPositionSwap_Score != 0)
					{
						pairDataToAdd.first = i + iDinnerAndMovie + 1;
						pairDataToAdd.second.first = L"Brave1 to Brave2 swap";
						pairDataToAdd.second.second = iPositionSwap_Score;
						mtxDateSpecific.lock();
						vctScheduleScoreData->at(iPopulationMember).vctDateSpecific.push_back(pairDataToAdd);
						mtxDateSpecific.unlock();
					}
				}
				if (scheduleToScore[i].second == scheduleToScore[i + 1].first)
				{
					iSwapCount++;
					if (bFinalSchedules && iPositionSwap_Score != 0)
					{
						pairDataToAdd.first = i + iDinnerAndMovie + 1;
						pairDataToAdd.second.first = L"Brave2 to Brave1 swap";
						pairDataToAdd.second.second = iPositionSwap_Score;
						mtxDateSpecific.lock();
						vctScheduleScoreData->at(iPopulationMember).vctDateSpecific.push_back(pairDataToAdd);
						mtxDateSpecific.unlock();
					}
				}
			}
			iscore += iSwapCount * iPositionSwap_Score;
			return iscore;
		}
		catch (std::exception const & e)
		{
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown Exception";
			OutputDebugString(msg->Data());
		}
	});
	//Function to score matched desired wingman
	funcs.push_back([&](std::vector<std::pair<size_t, size_t>> scheduleToScore, size_t iPopulationMember) ->size_t
	{
		try
		{
			int iscore = 0;
			std::pair<std::size_t, std::pair<std::wstring, int>> pairDataToAdd;
			for (size_t i = 0;i < availabilityData.iNumberOfAvailabilityDataRows;i++)
			{
				size_t iDinnerAndMovie = 0;//tracks instances of dinner and a movie
				size_t iMatchedWingmanCount = 0;////tracks instances of dinner and a movie on desired day
				for (size_t j = 0;j < availabilityData.iNumberOfAvailabilityPeriods;j++)
				{
					if (availabilityData.ppIntWingmanPrefArray[i][j] != -1)
					{
						//Find wingman matches
						if (scheduleToScore[j + iDinnerAndMovie].first == i && scheduleToScore[j + iDinnerAndMovie].second == availabilityData.ppIntWingmanPrefArray[i][j])
						{
							iMatchedWingmanCount++;
							if (bFinalSchedules && iScheduledWithDesiredWingman_Score != 0)
							{
								pairDataToAdd.first = j + iDinnerAndMovie;
								pairDataToAdd.second.first = L"Requested wingman match";
								pairDataToAdd.second.second = iScheduledWithDesiredWingman_Score;
								mtxDateSpecific.lock();
								vctScheduleScoreData->at(iPopulationMember).vctDateSpecific.push_back(pairDataToAdd);
								mtxDateSpecific.unlock();
							}
						}
						else if (scheduleToScore[j + iDinnerAndMovie].second == i && scheduleToScore[j + iDinnerAndMovie].first == availabilityData.ppIntWingmanPrefArray[i][j])
						{
							iMatchedWingmanCount++;
							if (bFinalSchedules && iScheduledWithDesiredWingman_Score != 0)
							{
								pairDataToAdd.first = j + iDinnerAndMovie;
								pairDataToAdd.second.first = L"Requested wingman match";
								pairDataToAdd.second.second = iScheduledWithDesiredWingman_Score;
								mtxDateSpecific.lock();
								vctScheduleScoreData->at(iPopulationMember).vctDateSpecific.push_back(pairDataToAdd);
								mtxDateSpecific.unlock();
							}
						}
					}
					if (scheduleData.iNumberOfSubPeriods[j] > 1)
					{
						iDinnerAndMovie++;//increment D&M Count
						if (availabilityData.ppIntWingmanPrefArray[i][j] != -1)
						{
							//Find wingman matches
							if (scheduleToScore[j + iDinnerAndMovie].first == i && scheduleToScore[j + iDinnerAndMovie].second == availabilityData.ppIntWingmanPrefArray[i][j])
							{
								iMatchedWingmanCount++;
								if (bFinalSchedules && iScheduledWithDesiredWingman_Score != 0)
								{
									pairDataToAdd.first = j + iDinnerAndMovie;
									pairDataToAdd.second.first = L"Requested wingman match";
									pairDataToAdd.second.second = iScheduledWithDesiredWingman_Score;
									mtxDateSpecific.lock();
									vctScheduleScoreData->at(iPopulationMember).vctDateSpecific.push_back(pairDataToAdd);
									mtxDateSpecific.unlock();
								}
							}
							else if (scheduleToScore[j + iDinnerAndMovie].second == i && scheduleToScore[j + iDinnerAndMovie].first == availabilityData.ppIntWingmanPrefArray[i][j])
							{
								iMatchedWingmanCount++;
								if (bFinalSchedules && iScheduledWithDesiredWingman_Score != 0)
								{
									pairDataToAdd.first = j + iDinnerAndMovie;
									pairDataToAdd.second.first = L"Requested wingman match";
									pairDataToAdd.second.second = iScheduledWithDesiredWingman_Score;
									mtxDateSpecific.lock();
									vctScheduleScoreData->at(iPopulationMember).vctDateSpecific.push_back(pairDataToAdd);
									mtxDateSpecific.unlock();
								}
							}
						}
					}
				}
				iscore += iMatchedWingmanCount*iScheduledWithDesiredWingman_Score;
			}
			return iscore;
		}
		catch (std::exception const & e)
		{
			Platform::String^ msg("Standard Error = ");
			OutputDebugString(msg->Data());
		}
		catch (...)
		{
			Platform::String^ msg = "Unkown Exception";
			OutputDebugString(msg->Data());
		}
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
