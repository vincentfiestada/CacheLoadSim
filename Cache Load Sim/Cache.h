#pragma once

#include "DataChunk.h"

#define NOTHING -1
#define Direct 0
#define Full 1
#define Set 2
#define FIFO 0
#define LRU 1
#define LFU 2

namespace Cache_Load_Sim
{
	public ref class Cache sealed
	{
	public:
		Cache(int size, int map, int replace, int addressBits, Platform::Object^ RAMTable);

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
			int get() { return _totalHits; }
		}
		property int TotalMisses
		{
			int get() { return _totalMisses; }
		}
		property int TotalHitTime
		{
			int get() { return _totalHitTime; }
		}
		property int TotalMissPenalty
		{
			int get() { return _totalMissPenalty; }
		}

		DataChunk^ getAt(int index);
		void Load(int address);

	private:
		int _size;
		int _mappingIndexBitShift;
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