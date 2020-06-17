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
#include "conf.h"

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

    const std::unordered_map<types::string, token_type> keyword_map = {
        { CHE_STR("while"), token_type::WHILE },
        { CHE_STR("if"), token_type::IF },
        { CHE_STR("and"), token_type::AND },
        { CHE_STR("or"), token_type::OR },
        { CHE_STR("not"), token_type::NOT },
    	{ CHE_STR("let"), token_type::LET },
    	{ CHE_STR("const"), token_type::CONST },
    	{ CHE_STR("fn"), token_type::FUNCTION },
    };

	typedef std::variant<std::monostate, long long, long double, types::che_char, types::string> token;
}
