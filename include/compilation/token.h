/*
 * File Name: token.h
 * Author(s): P. Kamara
 *
 * Token object and associated enums.
 */

#pragma once
#include <unordered_map>
#include <variant>
#include <array>

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
    	LET, CONST,
    	FUNCTION, WHILE, IF,
    	AND, OR, NOT,
    	/* Symbols */
    	ASSIGN, ADD_ASSIGN, SUBTRACT_ASSIGN, MULTIPLY_ASSIGN, DIVIDE_ASSIGN,
    	EQUALS, ADD, SUBTRACT, MULTIPLY, DIVIDE,
    	INCREMENT, DECREMENT,

        OPEN_PARENTHESIS, CLOSE_PARENTHESIS,
        OPEN_BRACE, CLOSE_BRACE,
        OPEN_BRACKET, CLOSE_BRACKET,

        QUESTION_MARK, EXCLAIMATION_MARK,
        COMMA, COLON, SEMICOLON, DOT,
    	ARROW,
    	COUNT,
    };

	const std::array<const char*, static_cast<size_t>(token_type::COUNT)> token_strings = {
		"None",
		"EOF",
		"Identifier",
		"Literal",
		"Let",
		"Const",
		"Function",
		"While",
		"If",
		"And",
		"Or",
		"Not",
		"=",
		"+=",
		"-=",
		"*=",
		"/=",
		"==",
		"+",
		"-",
		"*",
		"/",
		"++",
		"--",
		"(",
		")",
		"{",
		"}",
		"(",
		")",
		"?",
		"!",
		",",
		":",
		";",
		".",
		"->"
	};

	inline const char* get_token_str(token_type token)
	{
		return token_strings[static_cast<size_t>(token)];
	}

    const std::unordered_map<std::wstring, token_type> keyword_map = {
        { L"while", token_type::WHILE },
        { L"if", token_type::IF },
        { L"and", token_type::AND },
        { L"or", token_type::OR },
        { L"not", token_type::NOT },
    	{ L"let", token_type::LET },
    	{ L"const", token_type::CONST },
    	{ L"fn", token_type::FUNCTION },
    };

	typedef std::variant<std::monostate, long long, long double, wchar_t, std::wstring> token;
}
