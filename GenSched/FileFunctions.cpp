/*
 * FileFunctions.cpp
 *
 *  Created on: May 9, 2016
 *      Author: Jenner
 */

#include "FileFunctions.h"
#include "pch.h"
#include <string>
#include <iostream>
#include <stdio.h>

using namespace Windows::Storage;
using namespace std;
using namespace Windows::Storage::Pickers;
using namespace concurrency;
using namespace Windows::Devices::Enumeration;



FileFunctions::FileFunctions():sPath(""), sFileName(""), bActionCanceled(false)
{

}

FileFunctions::~FileFunctions()
{

}
	void FileFunctions::getPathandFileName() {

		FileOpenPicker^ picker = ref new FileOpenPicker();
		picker->FileTypeFilter->Append(".csv");
		auto fileTask = create_task(picker->PickSingleFileAsync());
		fileTask.then([&](StorageFile^ file) {
			wstring wsPath(file->Path->Data());
			wstring wsFileName(file->Name->Data());
			string str(wsPath.begin(), wsPath.end());
			sPath = str;
			str = string(wsFileName.begin(), wsFileName.end());
			sFileName = str;
		})
		.then([&](task<void> t)
		{

			try
			{
				t.wait();
			}
			catch (concurrency::task_canceled e)
			{
				bActionCanceled = true;
			}
			catch (Platform::COMException^ e)
			{
				//Example output: The system cannot find the specified file.
				OutputDebugString(e->Message->Data());
			}

		});
	}

	string FileFunctions::getFileName() {
		return sFileName;
	}

	bool FileFunctions::getActionCanceled()
	{
		return bActionCanceled;
	}

	string FileFunctions::getPath() {
		return sPath;
	}