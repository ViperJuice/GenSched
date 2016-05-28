/*
 * FileFunctions.h
 *
 *  Created on: May 9, 2016
 *      Author: Jenner
 */

#ifndef FILEFUNCTIONS_H_
#define FILEFUNCTIONS_H_
using namespace std;


class FileFunctions {
	public:
		FileFunctions();
		~FileFunctions();
		string getPath();
		string getFileName();
		string** getCSVData();
		bool getActionCanceled();
		
	private:
		string sPath;
		string sFileName;
		bool bActionCanceled;
		concurrency::task_completion_event<Platform::String^> fileSelectionComplete;
		void pathAndFileNamePicker();
};


#endif /* FILEFUNCTIONS_H_ */
