//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "Cache.h"

using namespace Platform::Collections;

namespace Cache_Load_Sim
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		Vector<DataChunk^>^ MainMemory;
		Cache^ ProcessorCache;

		void LoadAddressButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ShowErrorDialogBox(Platform::Object^ sender, Platform::String^ message);
		void CommandInvokedHandler(Windows::UI::Popups::IUICommand^ command);
		void ResetSimulationButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
