/*
 * File Name: parser.h
 * Author(s): P. Kamara
 *
 * Parser.
 */

#pragma once

#include "lexer.h"
#include "ast/node.h"

namespace cherie::compiler
{
	class parser
	{
        std::unique_ptr<lexer> lexer_;

        template<typename T>
        T get_token_value()
        {
            const auto token_value = lexer_->token_value();
            if (!std::holds_alternative<T>(token_value))
            {
                parser_error("");
            }

            return std::get<T>(token_value);
        }
		
        void expect(token_type type) const;

        template<typename T>
        T expect_and_get(const token_type type)
        {
            expect(type);
            return get_token_value<T>();
        }

        ast::call_expression* parse_call_expression();
        ast::primary_expression* parse_primary_expression();
        ast::multiplicative_expression* parse_multiplicative_expression();
        ast::additive_expression* parse_additive_expression();
        ast::expression* parse_expression();
		
        ast::function_definition* parse_function_definition();
        ast::statement_block* parse_statement_block();

        ast::assignment_statement* parse_assignment_statement();
        ast::while_statement* parse_while_statement();
        ast::if_statement* parse_if_statement();
        ast::statement* parse_statement();
	public:
        explicit parser(lexer* lexer);
		
        ast::program* parse();
	};
}
