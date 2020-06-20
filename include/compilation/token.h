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
#include "definitions.h"

namespace cherie::compiler
{
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
    	{ CHE_STR("true"), token_type::TRUE },
    	{ CHE_STR("false"), token_type::FALSE },
    	{ CHE_STR("string"), token_type::TYPE_STRING },
    	{ CHE_STR("number"), token_type::TYPE_NUMBER },
    };

	typedef std::variant<std::monostate, bool, types::integer, types::floating_point, types::che_char, types::string> token;
}
