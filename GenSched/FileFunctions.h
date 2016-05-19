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
		static string getPath() {

			return sPath;
		}
		static string getFileName();
	private:
		static string sPath;
		static string sFileName;
		static bool bActionsCanceled;
};


#endif /* FILEFUNCTIONS_H_ */
