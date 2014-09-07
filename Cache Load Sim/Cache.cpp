#include "pch.h"
#include "Cache.h"

using namespace Cache_Load_Sim;

Cache::Cache(int size, int map, int replace, Platform::Object^ RAMTable)
{
	// size		   = number of cache entries
	// map         = mapping algorithm to use
	// replace     = replacement algorithm to use
	Size = size;
	_entries = ref new Platform::Collections::Vector<DataChunk^>;
	for (int i = 0; i < size; i++)
	{
		_entries->Append(ref new DataChunk(NOTHING, 0));
	}
	_RAMTable = (Platform::Collections::Vector<DataChunk^>^)RAMTable;
	_mapping = map;
	_replacement = replace;
	_lastFIFOTracker = 0;
	_totalHits = 0;
	_totalMisses = 0;
	_totalHitTime = 0;
	_totalMissPenalty = 0;
	// get the number of bitshifts to use when getting the index for direct and set assoc mapping
	// 1) Get an index Template, of there are (in binary) 000 - 111 slots, you need 3 bits as index
	//    If we take those three bits from the rightmost part of the address, we can just use
	//    a bitwise  AND (&) with the address and the number of slots - 1
	int bitcount = 0;
	_mappingIndexTemplate = 0;
	if (_mapping == Direct)
	{
		bitcount = floor(log2(size));
		for (int i = 0; i < bitcount; i++)
		{
			_mappingIndexTemplate <<= 1;
			_mappingIndexTemplate += 1;
		}
		
	}
	else if (_mapping == Set)
	{
		// Number of bits to use for set assoc mapping is always log(base:2 of n) - 1
		bitcount = floor(log2(size)) - 1;
		for (int i = 0; i < bitcount; i++)
		{
			_mappingIndexTemplate <<= 1;
			_mappingIndexTemplate += 1;
		}
	}
}

void Cache::Load(int address)
{
	// We can treat all three mapping algorithms as set associative
	//		Set Associative maps addresses to 2 consecutive cache entries
	//		Direct mapping maps addresses to 1 cache entry
	//		Fully Associative maps addresses to all cache entries
	unsigned int mapStart = 0;
	unsigned int mapEnd = 0;
	int tempAddress = address;

	switch (_mapping)
	{
	case Direct:
		tempAddress &= _mappingIndexTemplate;
		mapStart = tempAddress;
		mapEnd = mapStart;
		break;
	case Set:
		if (_mappingIndexTemplate == 0)
		{
			tempAddress &= 1;
			mapStart = tempAddress;
			mapEnd = mapStart;
		}
		else
		{
			tempAddress &= _mappingIndexTemplate;
			mapStart = tempAddress * 2;
			mapEnd = mapStart + 1;
		}
		break;
	case Full:
		mapStart = 0;
		mapEnd = _size - 1;
		break;
	default:
		throw ref new Platform::FailureException(L"Mapping Algorithm is not recognized");
	}

	// Now that we know the start and end of the map, look for the data chunk w/ same address in the cache
	for (unsigned int i = mapStart; i <= mapEnd; i++)
	{
		Cache::TotalHitTime += 1; // each time we look at a slot, hit time increases
		if (_entries->GetAt(i)->Address == address)
		{
			Cache::TotalHits += 1;
			if (_replacement == LRU || _replacement == LFU)
			{
				_entries->GetAt(i)->AccessFrequency++;
			}
			return; // data found; return
		}
	}

	// Load function did not return, so the data is not in the cache yet
	//      First, increment total miss count...and then
	//		we need to load it from Main Memory
	Cache::TotalMisses += 1;

	// Find an empty slot in the 'map'
	for (unsigned int i = mapStart; i <= mapEnd; i++)
	{
		if (i < 0 || i > _entries->Size - 1) continue;
		// each time we look for an empty slot, increment total miss penalty
		Cache::TotalMissPenalty += 1;
		if (_entries->GetAt(i)->Address == NOTHING) // If address is the constant NOTHING, the slot is empty
		{
			// Now we can load the data chunk from RAM
			_entries->SetAt(i, _RAMTable->GetAt(address));
			if (_replacement == LRU || _replacement == LFU)
			{
				_lastFIFOTracker += 1;
				_entries->GetAt(i)->AccessFrequency = _lastFIFOTracker;
				_entries->GetAt(i)->AccessFrequency++;
			}
			else if (_replacement == FIFO)
			{
				_entries->GetAt(i)->AccessFrequency = _lastFIFOTracker + 1;
				_lastFIFOTracker++;
			}
			return; // Empty slot found; data chunk loaded; return from function
		}
	}

	// No empty slot was found, so we need to choose a slot in the map to replace

	int min = -1;
	int indexOfSlotToReplace = -1;

	// Whatever replacement algorithm is used, just replace the slot which has the lowest access frequency
	//		If using FIFO, the frequency of each new loaded data chunk is 1 + the highest previous frequency
	//			And it is reset when the chunk is unloaded
	//		If LRU, the frequency is incremented each time a chunk is used
	//			And it is reset when the chunk is unloaded
	//		If LFU, the frequency is incremented each time a chunk is used
	//			But it is never reset
	//		Note: LRU & LFU still incorporate the lastFIFOTracker so that there is no bias on positioning

	for (unsigned int i = mapStart; i <= mapEnd; i++)
	{
		if (_entries->GetAt(i)->AccessFrequency < min || min == -1)
		{
			min = _entries->GetAt(i)->AccessFrequency;
			indexOfSlotToReplace = i;
		}
	}

	// If FIFO or LRU, reset access frequency counter
	if (_replacement == FIFO || _replacement == LRU)
	{
		_entries->GetAt(indexOfSlotToReplace)->AccessFrequency = 0;
	}
	_entries->SetAt(indexOfSlotToReplace, _RAMTable->GetAt(address));
	// Now take care of the newly loaded chunk's frequency counter
	if (_replacement == LRU || _replacement == LFU)
	{
		_lastFIFOTracker += 1;
		_entries->GetAt(indexOfSlotToReplace)->AccessFrequency = _lastFIFOTracker;
		_entries->GetAt(indexOfSlotToReplace)->AccessFrequency += 1;
	}
	else if (_replacement == FIFO)
	{
		_lastFIFOTracker += 1;
		_entries->GetAt(indexOfSlotToReplace)->AccessFrequency = _lastFIFOTracker;
	}
	
	return;

}