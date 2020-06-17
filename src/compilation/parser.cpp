/*
 * File Name: parser.cpp
 * Author(s): P. Kamara
 *
 * Parser.
 */

#include "exceptions.h"
#include "compilation/parser.h"

namespace cherie::compiler
{
	std::unique_ptr<ast::call_expression> parse_call_expression(lexer* lexer);
	
	void expect(lexer* lexer, token_type type)
	{
		if (lexer->next_token() != type)
		{
			parser_error("");
		}
	}
	
	template<typename T>
	T expect_and_get(lexer* lexer, token_type type)
	{
		if (lexer->next_token() != type)
		{
			parser_error("");
		}
		
		const auto token_value = lexer->token_value();
		if (!std::holds_alternative<T>(token_value))
		{
			parser_error("");
		}
		
		return std::get<T>(token_value);
	}

	template<typename T>
	T get_token_value(lexer* lexer)
	{
		const auto token_value = lexer->token_value();
		if (!std::holds_alternative<T>(token_value))
		{
			parser_error("");
		}

		return std::get<T>(token_value);
	}

	std::unique_ptr<ast::expression> parse_expression(lexer* lexer, token_type current_token_type)
	{
		switch(current_token_type)
		{
			case token_type::IDENTIFIER:
			{
				if (lexer->peek_token() == token_type::OPEN_PARENTHESIS) /* Call expression */
				{
					return parse_call_expression(lexer);
				}
				break;
			}
			case token_type::LITERAL:
			{
				const auto literal = lexer->token_value();
				if (std::holds_alternative<types::string>(literal))
				{
					return std::make_unique<ast::string_literal>(std::get<types::string>(literal));
				}
				break;
			}
		}
		return nullptr;
	}

	std::unique_ptr<ast::call_expression> parse_call_expression(lexer* lexer)
	{
		auto call_expr = std::make_unique<ast::call_expression>(get_token_value<types::string>(lexer));
		
		expect(lexer, token_type::OPEN_PARENTHESIS);
		if (lexer->peek_token() == token_type::CLOSE_PARENTHESIS)
		{
			lexer->next_token();
			return call_expr;
		}

		call_expr->arguments.emplace_back(parse_expression(lexer, lexer->next_token()));
		while (lexer->peek_token() != token_type::CLOSE_PARENTHESIS)
		{
			expect(lexer, token_type::COMMA);
			call_expr->arguments.emplace_back(parse_expression(lexer, lexer->next_token()));
		}
		expect(lexer, token_type::CLOSE_PARENTHESIS);

		return call_expr;
	}
	
	std::unique_ptr<ast::statement> parse_statement(lexer* lexer, token_type current_token_type)
	{
		switch (current_token_type)
		{
		default:
			{
				return parse_expression(lexer, current_token_type);
			}
		}
		return std::make_unique<ast::statement>();
	}

	std::unique_ptr<ast::function_definition> parse_function_definition(lexer* lexer)
	{
		auto func_def = std::make_unique<ast::function_definition>();

		func_def->function_name = expect_and_get<types::string>(lexer, token_type::IDENTIFIER);
		expect(lexer, token_type::OPEN_PARENTHESIS);
		expect(lexer, token_type::CLOSE_PARENTHESIS);

		expect(lexer, token_type::OPEN_BRACE);
		expect(lexer, token_type::CLOSE_BRACE);

		return func_def;
	}
	
	std::unique_ptr<ast::program> parse(std::unique_ptr<lexer> lexer)
	{
		auto program_node = std::make_unique<ast::program>();
		
		auto next_token = lexer->next_token();
		while (next_token != token_type::EOF)
		{
			if (next_token == token_type::FUNCTION)
			{
				auto func_def = parse_function_definition(lexer.get());
				program_node->body.emplace_back(std::move(func_def));
			}
			else
			{
				auto statement = parse_statement(lexer.get(), next_token);
				program_node->body.emplace_back(std::move(statement));
			}
			next_token = lexer->next_token();
		}

		return program_node;
	}

}