/*
 * File Name: exceptions.h
 * Author(s): P. Kamara
 *
 * Cherie-related exceptions.
 */

#pragma once

#include <exception>
#include <string>
#include <cstdio>

namespace cherie
{
	struct lexer_exception
		: std::exception
	{
		explicit lexer_exception(const std::string& what)
			: std::exception(what.c_str()) {}
	};

	struct parser_exception
		: std::exception
	{
		explicit parser_exception(const std::string& what)
			: std::exception(what.c_str()) {}
	};

	template <typename... Args>
	void lexer_error(std::string format, Args... args)
	{
		char str[_MAX_PATH];
		sprintf_s(str, format.c_str(), args...);
		throw lexer_exception(str);
	}
}