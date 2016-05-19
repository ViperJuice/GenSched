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
		bool getActionCanceled();
		void getPathandFileName();
	private:
		string sPath;
		string sFileName;
		bool bActionCanceled;
};


#endif /* FILEFUNCTIONS_H_ */
