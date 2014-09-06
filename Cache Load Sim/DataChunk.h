#pragma once
using namespace Platform;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Data;

namespace Cache_Load_Sim
{
	// TODO: Implement DataChanged
	public ref class DataChunk sealed : INotifyPropertyChanged
	{
	public:
		DataChunk(int address, int data)
		{
			Address = address;
			Data = data;
			AccessFrequency = 0;
		}

		property int Address
		{
			void set(int address)
			{
				_address = address;
				PropertyChanged(this, ref new PropertyChangedEventArgs(ref new String(L"Address")));
			}
			int get()
			{
				return _address;
			}
		}
		property int Data
		{
			void set(int data)
			{
				_data = data;
				PropertyChanged(this, ref new PropertyChangedEventArgs(ref new String(L"Data")));
			}
			int get()
			{
				return _data;
			}
		}
		property int AccessFrequency
		{
			void set(int freq)
			{
				_freq = freq;
				PropertyChanged(this, ref new PropertyChangedEventArgs(ref new String(L"AccessFrequency")));
			}
			int get()
			{
				return _freq;
			}
		}

		virtual event PropertyChangedEventHandler^ PropertyChanged;

	private:
		int _address;
		int _data;
		int _freq;
	};
}