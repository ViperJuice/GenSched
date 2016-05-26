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
#include <thread>

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

void FileFunctions::fileSelectionRetriever() {
	Platform::String^ fileSelectionResult = create_task(fileSelectionComplete).get();
}
void FileFunctions::setPathAndFileName() {
	thread t1(&FileFunctions::fileSelectionRetriever, this);
	t1.join();
}

void FileFunctions::pathAndFileNamePicker() {
	FileOpenPicker^ picker = ref new FileOpenPicker();
	picker->FileTypeFilter->Append(".csv");
	auto fileTask = create_task(picker->PickSingleFileAsync());
	fileTask.then([this](StorageFile^ file) {
		wstring wsPath(file->Path->Data());
		wstring wsFileName(file->Name->Data());
		string str(wsPath.begin(), wsPath.end());
		sPath = str;
		str = string(wsFileName.begin(), wsFileName.end());
		sFileName = str;
	})
	.then([this](task<Platform::String^> t)
	{
		/*try
		{
			fileSelectionComplete.set(t.get());
		}
		catch (Platform::Exception^ exception)
		{
			fileSelectionComplete.set_exception(exception);
		}*/
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