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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();

	// Initialize
	MainMemory = ref new Platform::Collections::Vector<DataChunk^>;
	ProcessorCache = ref new Cache(4, Full, LRU, 4,  MainMemory);

	// >>>>>>>>>>>>>>>>>> IMPORTANT NOTE! <<<<<<<<<<<<<<<<<
	//	You need to create a better interface instead of just hardcoding
	//		the load instructions and allow the simulation to be viewed step by step

	int dataToPut = 0x0000;
	// Populate Main Memory
	for (int i = 0; i < 16; i++)
	{
		MainMemory->Append(ref new DataChunk(i, dataToPut));
		dataToPut += 0x1111;
	}
	// Execute Instructions
	ProcessorCache->Load(10);
	ProcessorCache->Load(11);
	ProcessorCache->Load(12);
	ProcessorCache->Load(13);
	ProcessorCache->Load(10);
	ProcessorCache->Load(10);
	ProcessorCache->Load(10);
	ProcessorCache->Load(14);
	ProcessorCache->Load(15);

	// Display Simulation Results
	simulationStats->Text += ref new String(L"Simulated Settings:\n\n");
	simulationStats->Text += ProcessorCache->Size.ToString() + ref new String(L" Cache Entries\n");
	simulationStats->Text += ProcessorCache->MappingAlgorithm + ref new String(L" is used\n");
	simulationStats->Text += ProcessorCache->ReplacementAlgorithm + ref new String(L"\n");
	totalHitsResult->Text += ref new String(L"Total Hits: ");
	totalHitsResult->Text += ProcessorCache->TotalHits.ToString();
	totalMissesResult->Text += ref new String(L"Total Misses: ");
	totalMissesResult->Text += ProcessorCache->TotalMisses.ToString();
	totalHitTimeResult->Text += ref new String(L"Total Hit Time: ");
	totalHitTimeResult->Text += ProcessorCache->TotalHitTime.ToString();
	totalMissPenaltyResult->Text += ref new String(L"Total Miss Penalty: ");
	totalMissPenaltyResult->Text += ProcessorCache->TotalMissPenalty.ToString();
	finalState->Text += ref new String(L"Final State:\n");
	for (int i = 0; i < ProcessorCache->Size; i++)
	{
		finalState->Text += ref new String(L"\n") + i.ToString() + ref new String(L"  >  ");
		int addressToShow = ProcessorCache->getAt(i)->Address;
		if (addressToShow != -1)
		{
			finalState->Text += addressToShow.ToString();
		}
		else
		{
			finalState->Text += ref new String(L"[EMPTY]");
		}
	}
}
