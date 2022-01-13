#include "JsonFormaterValue.h"


JsonFormaterValue::JsonFormaterValue(rapidjson::Document& document_, rapidjson::Value& value_)
	: document(document_)
	, value(value_) {}

size_t JsonFormaterValue::Size() const
{
	return value.IsArray() ? value.Size() : 0;
}

JsonFormaterValue JsonFormaterValue::operator[](unsigned index) const
{
	if (!value.IsArray())
	{
		value.SetArray();
	}

	while (index >= value.Size())
	{
		value.PushBack(rapidjson::Value(), document.GetAllocator());
	}

	return JsonFormaterValue(document, value[index]);
}

void JsonFormaterValue::operator=(bool x)
{
	value = x;
}

void JsonFormaterValue::operator=(int x)
{
	value = x;
}

void JsonFormaterValue::operator=(unsigned x)
{
	value = x;
}

void JsonFormaterValue::operator=(long long x)
{
	value = x;
}

void JsonFormaterValue::operator=(unsigned long long x)
{
	value = x;
}

void JsonFormaterValue::operator=(float x)
{
	value = x;
}

void JsonFormaterValue::operator=(double x)
{
	value = x;
}

void JsonFormaterValue::operator=(const char* x)
{
	value.SetString(x, document.GetAllocator());
}

JsonFormaterValue::operator bool() const
{
	return value.IsBool() ? value.GetBool() : false;
}

JsonFormaterValue::operator int() const
{
	return value.IsInt() ? value.GetInt() : 0;
}

JsonFormaterValue::operator unsigned() const
{
	return value.IsUint() ? value.GetUint() : 0;
}

JsonFormaterValue::operator long long() const
{
	return value.IsInt64() ? value.GetInt64() : 0;
}

JsonFormaterValue::operator unsigned long long() const
{
	return value.IsUint64() ? value.GetUint64() : 0;
}

JsonFormaterValue::operator float() const
{
	return value.IsDouble() ? value.GetFloat() : 0;
}

JsonFormaterValue::operator double() const
{
	return value.IsDouble() ? value.GetDouble() : 0;
}

JsonFormaterValue::operator std::string() const
{
	return value.IsString() ? value.GetString() : "";
}
