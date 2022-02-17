#pragma once

//TODO Improve using this https://github.com/systelab/cpp-rapidjson-json-adapter/blob/master/src/RapidJSONAdapter/JSONValue.h

#include "rapidjson/document.h"

#include <string>

namespace Hachiko
{
    class JsonFormatterValue
    {
    public:
        JsonFormatterValue(rapidjson::Document& document, rapidjson::Value& value);

        // Size of the array. Returns 0 if the value is not an array.
        [[nodiscard]] size_t Size() const;

        // Object/array access (Creates new members if they don't exist)
        template<typename T>
        JsonFormatterValue operator[](T* key) const;
        JsonFormatterValue operator[](unsigned index) const;

        // Assignment operators to modify the value
        void operator=(bool x) const;
        void operator=(int x) const;
        void operator=(unsigned x) const;
        void operator=(long long x) const;
        void operator=(unsigned long long x) const;
        void operator=(float x) const;
        void operator=(double x) const;
        void operator=(const char* x) const;

        // Conversion operators for easy access
        operator bool() const;
        operator int() const;
        operator unsigned() const;
        operator long long() const;
        operator unsigned long long() const;
        operator float() const;
        operator double() const;
        operator std::string() const;

    private:
        rapidjson::Document& document;
        rapidjson::Value& value;
    };

    template<typename T>
    JsonFormatterValue JsonFormatterValue::operator[](T* key) const
    {
        if (!value.IsObject())
        {
            value.SetObject();
        }

        if (!value.HasMember(key))
        {
            value.AddMember(rapidjson::StringRef(key), rapidjson::Value(), document.GetAllocator());
        }

        return JsonFormatterValue(document, value[key]);
    }
}
