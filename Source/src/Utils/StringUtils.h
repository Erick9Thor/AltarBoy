#pragma once
#include <string>

namespace Hachiko
{
    // copied from https://cpp-optimizations.netlify.app/strings_concatenation/
    class StringUtils final
    {
    private:
        static size_t StrSize(const char* str)
        {
            return strlen(str);
        }

        static size_t StrSize(const std::string& str)
        {
            return str.size();
        }

        template<class Head, class... Tail>
        static size_t StrSize(const Head& head, const Tail&... tail)
        {
            return StrSize(head) + StrSize(tail...);
        }

        template<class Head>
        static void StrAppend(std::string& out, const Head& head)
        {
            out += head;
        }

        template<class Head, class... Args>
        static void StrAppend(std::string& out, const Head& head, const Args&... args)
        {
            out += head;
            StrAppend(out, args...);
        }

    public:
        template<class... Args>
        static std::string Concat(const Args&... args)
        {
            size_t tot_size = StrSize(args...);
            std::string out;
            out.reserve(tot_size);

            StrAppend(out, args...);
            return out;
        }
    };
}
