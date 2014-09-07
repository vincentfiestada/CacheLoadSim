#pragma once

#include "DataChunk.h"

#define NOTHING -1
#define Direct 0
#define Set 1
#define Full 2
#define FIFO 0
#define LRU 1
#define LFU 2

using namespace Windows::UI::Xaml::Data;
using namespace Windows::Foundation::Collections;

namespace Cache_Load_Sim
{
	[BindableAttribute]
	public ref class Cache sealed : INotifyPropertyChanged
	{
	public:
		Cache(int size, int map, int replace, Platform::Object^ RAMTable);

		property int Size
		{
			void set(int size)
			{
				_size = size;
			}
			int get()
			{
				return _size;
			}
		}
		property Platform::String^ MappingAlgorithm
		{
			Platform::String^ get()
			{
				switch (_mapping)
				{
				case Direct:
					return ref new Platform::String(L"Direct Mapping Algorithm");
					break;
				case Full:
					return ref new Platform::String(L"Fully Associative Mapping Algorithm");
					break;
				case Set:
					return ref new Platform::String(L"Set Associative Mapping Algorithm");
					break;
				default:
					return ref new Platform::String(L"Unknown Mapping Algorithm [Beware of iminent exceptions]");
				}
			}
		}
		property Platform::String^ ReplacementAlgorithm
		{
			Platform::String^ get()
			{
				switch (_replacement)
				{
				case FIFO:
					return ref new Platform::String(L"First-in-First-Out");
					break;
				case LRU:
					return ref new Platform::String(L"Least Recently Used is replaced");
					break;
				case LFU:
					return ref new Platform::String(L"Least Frequently Used is replaced");
					break;
				default:
					return ref new Platform::String(L"Random Slot is replaced");
				}
			}
		}
		property int TotalHits
		{
			void set(int x)
			{
				_totalHits = x;
				PropertyChanged(this, ref new PropertyChangedEventArgs(ref new String(L"TotalHits")));
			}
			int get() { return _totalHits; }
		}
		property int TotalMisses
		{
			void set(int x)
			{
				_totalMisses = x;
				PropertyChanged(this, ref new PropertyChangedEventArgs(ref new String(L"TotalMisses")));
			}
			int get() { return _totalMisses; }
		}
		property int TotalHitTime
		{
			void set(int x)
			{
				_totalHitTime = x;
				PropertyChanged(this, ref new PropertyChangedEventArgs(ref new String(L"TotalHitTime")));
			}
			int get() { return _totalHitTime; }
		}
		property int TotalMissPenalty
		{
			void set(int x)
			{
				_totalMissPenalty = x;
				PropertyChanged(this, ref new PropertyChangedEventArgs(ref new String(L"TotalMissPenalty")));
			}
			int get() { return _totalMissPenalty; }
		}
		property Platform::Object^ Slots
		{
			Platform::Object^ get()
			{
				return _entries;
			}
		}
		void Load(int address);

		virtual event PropertyChangedEventHandler^ PropertyChanged;

	private:
		int _size;
		int _mappingIndexTemplate;
		Platform::Collections::Vector<DataChunk^>^ _RAMTable;
		Platform::Collections::Vector<DataChunk^>^ _entries;
		int _lastFIFOTracker;
		int _mapping;
		int _replacement;
		// To estimate performance, monitor hit rate through total number of Hits
		//      , miss rate through total number of misses
		//		, total hit time [ includes time it takes to find the data chunk; still counted if it is a miss ]
		//		, total miss penalty [ includes time it takes to find which slot to replace or load into ]
		int _totalHitTime;
		int _totalHits;
		int _totalMissPenalty;
		int _totalMisses;
		
	};
}