//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace GenSched;
using namespace std;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Devices::Enumeration;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;


// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}


void GenSched::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	mainSchedulingSingleton = MainSchedulingSingleton::Instance();
	mainSchedulingSingleton->RunSchedulingProcess();
}

void GenSched::MainPage::button_CompletBuild_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	mainSchedulingSingleton->ForceCompleteSchedulingProcess();
}

void GenSched::MainPage::App_Suspending(Object ^ sender, SuspendingEventArgs ^ e)
{
	auto deferral = e->SuspendingOperation->GetDeferral();
}

