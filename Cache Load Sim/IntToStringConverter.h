#pragma once

using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Platform;

namespace Cache_Load_Sim
{
	public ref class IntToStringConverter sealed : IValueConverter{
	public:
		virtual Object^ Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language);
		virtual Object^ ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language);

		IntToStringConverter();

	private:
	};
}