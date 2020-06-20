/*
 * File Name: definitions.h
 * Author(s): P. Kamara
 *
 * Language wide definitions
 */

#pragma once

#undef EOF // included from stdio.h, fucking stupid.

namespace cherie
{
	namespace compiler
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
            TRUE, FALSE,
            /* Symbols */
            ASSIGN, ADD_ASSIGN, SUBTRACT_ASSIGN, MULTIPLY_ASSIGN, DIVIDE_ASSIGN,
            EQUALS, ADD, SUBTRACT, MULTIPLY, DIVIDE,
            INCREMENT, DECREMENT,
            /* Types */
            TYPE_STRING, TYPE_NUMBER, TYPE_CHARACTER,

            OPEN_PARENTHESIS, CLOSE_PARENTHESIS,
            OPEN_BRACE, CLOSE_BRACE,
            OPEN_BRACKET, CLOSE_BRACKET,

            QUESTION_MARK, EXCLAMATION_MARK,
            COMMA, COLON, SEMICOLON, DOT,
            ARROW,
            COUNT,
        };
	}
	
	enum class type
	{
		implicit,
		string,
		number,
		character,
	};
}