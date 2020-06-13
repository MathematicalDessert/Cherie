/*
 * File Name: token.h
 * Author(s): P. Kamara
 *
 * Token object and associated enums.
 */

#pragma once
#include <unordered_map>
#include <variant>

#undef EOF // included from stdio.h, fucking stupid.

namespace cherie::compiler
{
    enum class token_type
    {
    	NONE,
		EOF,
    	IDENTIFIER,
        LITERAL,
    	/* Keywords */
    	WHILE, IF,
    	AND, OR, NOT,
    	/* Symbols */
    	ASSIGN, ADD_ASSIGN, SUBTRACT_ASSIGN, MULTIPLY_ASSIGN, DIVIDE_ASSIGN,
    	EQUALS, ADD, SUBTRACT, MULTIPLY, DIVIDE,
    	INCREMENT, DECREMENT,

        OPEN_PARENTHESIS, CLOSE_PARENTHESIS,
        OPEN_BRACE, CLOSE_BRACE,
        OPEN_BRACKET, CLOSE_BRACKET,

        COMMA, COLON, SEMICOLON,
    };

    const std::unordered_map<std::wstring, token_type> keyword_map = {
        { L"while", token_type::WHILE },
        { L"if", token_type::IF },
        { L"and", token_type::AND },
        { L"or", token_type::OR },
        { L"not", token_type::NOT },
    };

	typedef std::variant<std::monostate, long long, long double, wchar_t, std::wstring> token;
}
