/*
 * File Name: lexer.h
 * Author(s): P. Kamara
 *
 * Lexer.
 */

#pragma once

#include <string>
#include "token.h"

namespace cherie::compiler
{
	class lexer
	{
        size_t line_ = 1;
        size_t column_ = 0;

        types::string current_lexeme_;
        std::unique_ptr<types::istream> stream_;
		
        token current_token_;
		token peeked_token_;
        token_type peeked_token_type_ = token_type::NONE;
		
        types::string source_;

        [[nodiscard]] bool is_end_of_source(size_t ptr) const;
        [[nodiscard]] types::che_char peek() const;
        types::che_char get(bool skip_whitespace = false, bool clear_whitespace = false);
		
        [[nodiscard]] types::string read_string();
        [[nodiscard]] long long read_number(bool is_hex);
        [[nodiscard]] long double read_float();
		
        void discard();
        void get_and_discard();

        void skip_comment(bool is_long_comment);
        void tokenize_string_literal(token& token_reference);
        void tokenize_number_literal(token& token_reference, types::che_char first_digit);
        token_type tokenize_symbol(types::che_char symbol);
        token_type tokenize_identifier_or_keyword(token& token_reference, types::che_char first_character);
        token_type tokenize(token& token_reference);
	public:
        explicit lexer(types::string source);
		
        token_type next_token();
        token_type peek_token();

        [[nodiscard]] token token_value() const { return current_token_; }
        [[nodiscard]] token peeked_token_value() const { return peeked_token_; }

        static const auto eof = std::char_traits<types::che_char>::eof();
	};
}
