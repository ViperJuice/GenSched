//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;


namespace GenSched
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		MainSchedulingSingleton* mainSchedulingSingleton;
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void button_CompletBuild_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void MainPage::App_Suspending(Object^ sender, SuspendingEventArgs^ e);
	};
}
