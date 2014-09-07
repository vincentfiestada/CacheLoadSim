//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace Cache_Load_Sim;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Popups;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();

	// Initialize
	MainMemory = ref new Vector<DataChunk^>;
	ProcessorCache = ref new Cache(4, Direct, LRU, MainMemory);

	int dataToPut = 0x00;
	// Populate Main Memory
	for (int i = 0; i < 16; i++)
	{
		MainMemory->Append(ref new DataChunk(i, dataToPut));
		dataToPut += 0x11;
	}
	//// Execute Instructions
	//ProcessorCache->Load(10);
	//ProcessorCache->Load(11);
	//ProcessorCache->Load(12);
	//ProcessorCache->Load(13);
	//ProcessorCache->Load(10);
	//ProcessorCache->Load(10);
	//ProcessorCache->Load(10);
	//ProcessorCache->Load(14);
	//ProcessorCache->Load(15);

	//// Display Simulation Results
	//simulationStats->Text += ref new String(L"Simulated Settings:\n\n");
	//simulationStats->Text += ProcessorCache->Size.ToString() + ref new String(L" Cache Entries\n");
	//simulationStats->Text += ProcessorCache->MappingAlgorithm + ref new String(L" is used\n");
	//simulationStats->Text += ProcessorCache->ReplacementAlgorithm + ref new String(L"\n");
	//totalHitsResult->Text += ref new String(L"Total Hits: ");
	//totalHitsResult->Text += ProcessorCache->TotalHits.ToString();
	//totalMissesResult->Text += ref new String(L"Total Misses: ");
	//totalMissesResult->Text += ProcessorCache->TotalMisses.ToString();
	//totalHitTimeResult->Text += ref new String(L"Total Hit Time: ");
	//totalHitTimeResult->Text += ProcessorCache->TotalHitTime.ToString();
	//totalMissPenaltyResult->Text += ref new String(L"Total Miss Penalty: ");
	//totalMissPenaltyResult->Text += ProcessorCache->TotalMissPenalty.ToString();
	//finalState->Text += ref new String(L"Final State:\n");
	//for (int i = 0; i < ProcessorCache->Size; i++)
	//{
	//	finalState->Text += ref new String(L"\n") + i.ToString() + ref new String(L"  >  ");
	//	int addressToShow = ProcessorCache->getAt(i)->Address;
	//	if (addressToShow != -1)
	//	{
	//		finalState->Text += addressToShow.ToString();
	//	}
	//	else
	//	{
	//		finalState->Text += ref new String(L"[EMPTY]");
	//	}
	//}

	MainMemoryContentsDisplay->DataContext = MainMemory;
	CacheSlotsDisplay->DataContext = ProcessorCache->Slots;
	StatCounters->DataContext = ProcessorCache;
	SimulationInfo->DataContext = ProcessorCache;
}

void Cache_Load_Sim::MainPage::LoadAddressButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	unsigned int address = _wtoi(AddressToLoadTextBox->Text->Data());

	// Check for out of range address
	if (address < 0 || address >= MainMemory->Size)
	{
		ShowErrorDialogBox(this, ref new String(L"Memory Address out of range. Choose an address from 0 to ") + (MainMemory->Size - 1).ToString() + ref new String(L"."));
		return;
	}

	ProcessorCache->Load(address);
}

void Cache_Load_Sim::MainPage::ShowErrorDialogBox(Platform::Object^ sender, Platform::String^ message)
{
	// Create the message dialog and set its content
	MessageDialog^ msg = ref new MessageDialog(message);

	// Add commands and set their callbacks; both commands use the same callback function instead of inline event handlers
	UICommand^ dismissCommand = ref new UICommand("Okay", ref new UICommandInvokedHandler(this, &MainPage::CommandInvokedHandler));

	// Add the commands to the dialog
	msg->Commands->Append(dismissCommand);

	// Set the command that will be invoked by default
	msg->DefaultCommandIndex = 0;

	// Set the command to be invoked when escape is pressed
	msg->CancelCommandIndex = 0;

	// Show the message dialog
	msg->ShowAsync();
}

/// <summary>
/// Callback function for the invocation of the dialog commands.
/// </summary>
/// <param name="command">The command that was invoked.</param>
void MainPage::CommandInvokedHandler(Windows::UI::Popups::IUICommand^ command)
{
	// Nothing to do, really
}

/// <summary>
/// Reset the simulation with the given settings
/// </summary>
void Cache_Load_Sim::MainPage::ResetSimulationButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// collect settings
	int cachesize = _wtoi(ResetCacheSizeTextBox->Text->Data());
	// must be either 2, 4, or 8
	if (cachesize != 2 && cachesize != 4 && cachesize != 8)
	{
		ShowErrorDialogBox(this, ref new String(L"Could not reset simulation.\nNumber of cache entries must be either 2, 4, or 8. Try again."));
		return;
	}
	// >>>>>>>>>>>>>>>> IMPORTANT !!! <<<<<<<<<<<<<<
	// Assuming that the comboboxes' indices match up with the constants defined in 'Cache.h':
	// >>>>>>>>>>>>>>>> IMPORTANT !!! <<<<<<<<<<<<<<

	int mapalgo = ResetMappingAlgorithmComboBox->SelectedIndex;
	int repalgo = ResetReplacementAlgorithmComboBox->SelectedIndex;

	// Remove data context references
	MainMemoryContentsDisplay->DataContext = nullptr;
	CacheSlotsDisplay->DataContext = nullptr;
	StatCounters->DataContext = nullptr;
	SimulationInfo->DataContext = nullptr;

	// destroy old main memory and cache explicity (Although runtime should take care of this)
	delete MainMemory;
	delete ProcessorCache;

	// Create new ones
	MainMemory = ref new Vector<DataChunk^>;
	ProcessorCache = ref new Cache(cachesize, mapalgo, repalgo, MainMemory);

	// Repopulate Main Memory
	int dataToPut = 0x00;
	// Populate Main Memory
	// If the cache has n entries, main memory has n^2 addresses
	int mainmemorysize = cachesize*cachesize;
	for (int i = 0; i < mainmemorysize; i++)
	{
		MainMemory->Append(ref new DataChunk(i, dataToPut));
		dataToPut += 0x11;
	}

	// Reinstate Data Context references
	CacheSlotsDisplay->DataContext = ProcessorCache;
	// We need to explicity redeclare the itemsSource; otherwise, the listview shows up empty
	CacheSlotsDisplay->ItemsSource = ProcessorCache->Slots;
	MainMemoryContentsDisplay->DataContext = MainMemory;
	MainMemoryContentsDisplay->ItemsSource = MainMemory;
	StatCounters->DataContext = ProcessorCache;
	SimulationInfo->DataContext = ProcessorCache;
}
