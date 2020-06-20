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

	std::unique_ptr<ast::expression> parse_arithmetic_operation(lexer* lexer, token_type datum)
	{
		return nullptr;
	}
	
	std::unique_ptr<ast::expression> parse_expression(lexer* lexer, token_type current_token_type)
	{
		switch(current_token_type)
		{
			case token_type::IDENTIFIER:
			{
				switch (lexer->peek_token())
				{
					/*case token_type::ADD:
					case token_type::SUBTRACT:
					case token_type::DIVIDE:
					case token_type::MULTIPLY:
					{
						// arith operation
						return parse_arithmetic_operation(lexer, current_token_type);
					}*/
					case token_type::OPEN_PARENTHESIS: return parse_call_expression(lexer);
					default: break;
				}
				
				break;
			}
			case token_type::TRUE: return std::make_unique<ast::boolean_literal>(true);
			case token_type::FALSE: return std::make_unique<ast::boolean_literal>(false);
			case token_type::LITERAL:
			{
				const auto literal = lexer->token_value();
				if (std::holds_alternative<types::string>(literal)) // string literal
				{
					return std::make_unique<ast::string_literal>(std::get<types::string>(literal));
				}
					
				if (std::holds_alternative<types::integer>(literal)) // integer literal
				{
					return std::make_unique<ast::number_literal>(std::get<types::integer>(literal));
				}

				if (std::holds_alternative<types::floating_point>(literal)) // floating point literal (same as integer)
				{
					return std::make_unique<ast::number_literal>(std::get<types::floating_point>(literal));
				}
				break;
			}
			default:
			{
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
	}

	std::unique_ptr<ast::function_definition> parse_function_definition(lexer* lexer)
	{
		auto func_def = std::make_unique<ast::function_definition>();

		func_def->function_name = expect_and_get<types::string>(lexer, token_type::IDENTIFIER);
		expect(lexer, token_type::OPEN_PARENTHESIS);
		if (lexer->peek_token() != token_type::CLOSE_PARENTHESIS)
		{
			
		}
		expect(lexer, token_type::CLOSE_PARENTHESIS);

		expect(lexer, token_type::OPEN_BRACE);
		expect(lexer, token_type::CLOSE_BRACE);

		return func_def;
	}
	
	ast::program parse(lexer* lexer)
	{
		ast::program program_node;
		
		auto next_token = lexer->next_token();
		while (next_token != token_type::EOF)
		{
			if (next_token == token_type::FUNCTION)
			{
				auto func_def = parse_function_definition(lexer);
				program_node.body.emplace_back(std::move(func_def));
			}
			else
			{
				auto statement = parse_statement(lexer, next_token);
				program_node.body.emplace_back(std::move(statement));
			}
			next_token = lexer->next_token();
		}

		return program_node;
	}
}