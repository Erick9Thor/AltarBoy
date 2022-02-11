#include "JsonFormatterValue.h"

Hachiko::JsonFormatterValue::JsonFormatterValue(rapidjson::Document& document_, rapidjson::Value& value_) :
    document(document_),
    value(value_) {}

size_t Hachiko::JsonFormatterValue::Size() const
{
    return value.IsArray() ? value.Size() : 0;
}

Hachiko::JsonFormatterValue Hachiko::JsonFormatterValue::operator[](unsigned index) const
{
    if (!value.IsArray())
    {
        value.SetArray();
    }

    while (index >= value.Size())
    {
        value.PushBack(rapidjson::Value(), document.GetAllocator());
    }

    return JsonFormatterValue(document, value[index]);
}

void Hachiko::JsonFormatterValue::operator=(bool x) const
{
    value = x;
}

void Hachiko::JsonFormatterValue::operator=(int x) const
{
    value = x;
}

void Hachiko::JsonFormatterValue::operator=(unsigned x) const
{
    value = x;
}

void Hachiko::JsonFormatterValue::operator=(long long x) const
{
    value = x;
}

void Hachiko::JsonFormatterValue::operator=(unsigned long long x) const
{
    value = x;
}

void Hachiko::JsonFormatterValue::operator=(float x) const
{
    value = x;
}

void Hachiko::JsonFormatterValue::operator=(double x) const
{
    value = x;
}

void Hachiko::JsonFormatterValue::operator=(const char* x) const
{
    value.SetString(x, document.GetAllocator());
}

Hachiko::JsonFormatterValue::operator bool() const
{
    return value.IsBool() ? value.GetBool() : false;
}

Hachiko::JsonFormatterValue::operator int() const
{
    return value.IsInt() ? value.GetInt() : 0;
}

Hachiko::JsonFormatterValue::operator unsigned() const
{
    return value.IsUint() ? value.GetUint() : 0;
}

Hachiko::JsonFormatterValue::operator long long() const
{
    return value.IsInt64() ? value.GetInt64() : 0;
}

Hachiko::JsonFormatterValue::operator unsigned long long() const
{
    return value.IsUint64() ? value.GetUint64() : 0;
}

Hachiko::JsonFormatterValue::operator float() const
{
    return value.IsDouble() ? value.GetFloat() : 0;
}

Hachiko::JsonFormatterValue::operator double() const
{
    return value.IsDouble() ? value.GetDouble() : 0;
}

Hachiko::JsonFormatterValue::operator std::string() const
{
    return value.IsString() ? value.GetString() : "";
}
