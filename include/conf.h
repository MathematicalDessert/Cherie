/*
 * File Name: conf.h
 * Author(s): P. Kamara
 *
 * configuration file.
 */

#pragma once
#include <string>

//#define CHERIE_UNICODE

namespace cherie
{
	namespace types
	{
#ifdef CHERIE_UNICODE
        using string = std::wstring;
        using istream = std::wistream;
        using stringstream = std::wstringstream;
        using che_char = wchar_t;
#define CHE_STR(x) L#x
#else
        using string = std::string;
        using istream = std::istream;
        using stringstream = std::stringstream;
        using che_char = char;
#define CHE_STR(x) x
		
        using integer = long long;
        using floating_point = long double;
	}
#endif
}