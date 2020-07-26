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
            FUNCTION, WHILE, IF, RETURN,
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

        const std::unordered_map<token_type, const char*> operator_name = {
		    { token_type::ADD, "+" },
		    { token_type::SUBTRACT, "-" },
		    { token_type::MULTIPLY, "*" },
		    { token_type::DIVIDE, "/" },
        };
		
        const std::unordered_map<token_type, int> operator_precedence = {
			{ token_type::ADD, 1 },
            { token_type::SUBTRACT, 1 },
            { token_type::MULTIPLY, 2 },
            { token_type::DIVIDE, 2 },
        };

		inline int get_op_binding_power(const token_type type)
		{
            return operator_precedence.at(type);
		}

		inline const char* get_op_symbol(const token_type type)
		{
            return operator_name.at(type);
		}
	}
	
	enum class type
	{
		implicit,
		string,
		number,
		character,
	};
}