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
using namespace Windows::Storage::Streams;
using namespace concurrency;
using namespace Windows::Devices::Enumeration;



FileFunctions::FileFunctions():sPath(""), sFileName(""), bActionCanceled(false)
{

}

FileFunctions::~FileFunctions()
{

}


void FileFunctions::pathAndFileNamePicker() {
	FileOpenPicker^ picker = ref new FileOpenPicker();
	picker->FileTypeFilter->Append(".csv");
	auto fileTask = create_task(picker->PickSingleFileAsync());
	fileTask.then([this](StorageFile^ file) {
		return create_task(file->OpenSequentialReadAsync());
	})
	.then([this](IInputStream^ stm) {
		Platform::String^ strData = stm->ToString();
	});
}


string FileFunctions::getFileName() {
	return sFileName;
}

string ** FileFunctions::getCSVData()
{
	pathAndFileNamePicker();
	return nullptr;
}

bool FileFunctions::getActionCanceled()
{
	return bActionCanceled;
}

string FileFunctions::getPath() {
	return sPath;
}