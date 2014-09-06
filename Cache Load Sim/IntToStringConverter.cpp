#include "pch.h"
#include "IntToStringConverter.h"

using namespace Cache_Load_Sim;
using namespace Windows::UI;

Object^ IntToStringConverter::Convert(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	return value->ToString();
}

Object^ IntToStringConverter::ConvertBack(Object^ value, TypeName targetType, Object^ parameter, String^ language)
{
	throw ref new NotImplementedException();
}