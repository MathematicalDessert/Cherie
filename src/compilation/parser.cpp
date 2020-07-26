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
	void expect(lexer* lexer, const token_type type)
	{
		if (lexer->next_token() != type)
		{
			parser_error("");
		}
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
	
	template<typename T>
	T expect_and_get(lexer* lexer, const token_type type)
	{
		expect(lexer, type);
		return get_token_value<T>(lexer);
	}

	ast::call_expression* parse_call_expression(lexer* lexer)
	{
		auto* call_expression = new ast::call_expression();

		call_expression->function_name = std::get<types::string>(lexer->token_value());

		expect(lexer, token_type::OPEN_PARENTHESIS);

		if (lexer->peek_token() == token_type::CLOSE_PARENTHESIS)
		{
			lexer->next_token();
			return call_expression;
		}

		call_expression->arguments.push_back(parse_expression(lexer));
		while (lexer->peek_token() == token_type::COMMA)
		{
			lexer->next_token();
			call_expression->arguments.push_back(parse_expression(lexer));
		}
		expect(lexer, token_type::CLOSE_PARENTHESIS);
		
		return call_expression;
	}
	
	ast::primary_expression* parse_primary_expression(lexer* lexer)
	{
		switch (lexer->peek_token())
		{
			case token_type::IDENTIFIER:
			{
				lexer->next_token();
				if (lexer->peek_token() == token_type::OPEN_PARENTHESIS)
				{
					return parse_call_expression(lexer);
				}
				return new ast::string_literal(std::get<types::string>(lexer->token_value()));
			}
			case token_type::TRUE: lexer->next_token();  return new ast::boolean_literal(true);
			case token_type::FALSE: lexer->next_token();  return new ast::boolean_literal(false);
			case token_type::LITERAL:
			{
				lexer->next_token();
				const auto literal = lexer->token_value();
				if (std::holds_alternative<types::string>(literal)) // string literal
				{
					return new ast::string_literal(std::get<types::string>(literal));
				}

				if (std::holds_alternative<types::integer>(literal)) // integer literal
				{
					return new ast::number_literal(std::get<types::integer>(literal));
				}

				if (std::holds_alternative<types::floating_point>(literal)) // floating point literal (same as integer)
				{
					return new ast::number_literal(std::get<types::floating_point>(literal));
				}
				break;
			}
		}
	}
	
	ast::multiplicative_expression* parse_multiplicative_expression(lexer* lexer)
	{
		auto* current_expression = parse_primary_expression(lexer);

		auto next_peeked_token = lexer->peek_token();
		switch (next_peeked_token)
		{
			case token_type::MULTIPLY:
			case token_type::DIVIDE:
			{
				while (next_peeked_token == token_type::MULTIPLY || next_peeked_token == token_type::DIVIDE)
				{
					const auto next_token = lexer->next_token(); // wouldn't evaluate, so this is a hack
					current_expression = new ast::binary_expression(next_token, current_expression, parse_primary_expression(lexer));
					next_peeked_token = lexer->peek_token();
				}
			}
			default:
			{
				break;
			}
		}
		return current_expression;
	}
	
	std::unique_ptr<ast::additive_expression> parse_additive_expression(lexer* lexer, const int right_binding_power = 0)
	{
		auto* current_expression = parse_multiplicative_expression(lexer);
		
		auto next_peeked_token = lexer->peek_token();
		switch (next_peeked_token)
		{
			case token_type::ADD:
			case token_type::SUBTRACT:
			{
				while (next_peeked_token == token_type::ADD || next_peeked_token == token_type::SUBTRACT)
				{
					const auto next_token = lexer->next_token(); // wouldn't evaluate, so this is a hack
					current_expression = new ast::binary_expression(next_token, current_expression, parse_multiplicative_expression(lexer));
					next_peeked_token = lexer->peek_token();
				}
			}
			default:
			{
				break;
			}
		}
		return std::unique_ptr<ast::additive_expression>(current_expression);
	}
	
	/**
	 * Had a long discussion about parsing expressions, and I've decided to go with the tried and
	 * tested method of pratt parsing. The rest of the parser will still be recursive descent!
	 */
	std::unique_ptr<ast::expression> parse_expression(lexer* lexer, const int right_binding_power)
	{
		switch(lexer->peek_token())
		{
			case token_type::OPEN_PARENTHESIS: /* ( expression ) */
			{
				lexer->next_token();
				auto expression = parse_expression(lexer, right_binding_power);
				expect(lexer, token_type::CLOSE_PARENTHESIS);
				return expression;
			}			
			default:
			{
				return parse_additive_expression(lexer);
			}
		}
	}

	ast::if_statement* parse_if_statement(lexer* lexer)
	{
		lexer->next_token();
		auto* stmt = new ast::if_statement();

		expect(lexer, token_type::OPEN_PARENTHESIS);
		stmt->condition = parse_expression(lexer);
		expect(lexer, token_type::CLOSE_PARENTHESIS);

		expect(lexer, token_type::OPEN_BRACE);
		stmt->main_block = std::make_unique<ast::statement_block>();
		while (lexer->peek_token() != token_type::CLOSE_BRACE)
		{
			stmt->main_block->statements.push_back(parse_statement(lexer));
		}
		expect(lexer, token_type::CLOSE_BRACE);
		
		return stmt;
	}

	std::unique_ptr<ast::statement> parse_statement(lexer* lexer)
	{
		switch (lexer->peek_token())
		{
		case token_type::LET:
		case token_type::CONST:
		{
			
			break;
		}
		case token_type::IF:
			{
				return std::unique_ptr<ast::statement>(parse_if_statement(lexer));
			}
		default:
			{
				auto x = parse_expression(lexer, 0);
				expect(lexer, token_type::SEMICOLON);
				return x;
			}
		}
	}

	ast::function_definition* parse_function_definition(lexer* lexer)
	{
		auto* function = new ast::function_definition();

		function->function_name = expect_and_get<std::string>(lexer, token_type::IDENTIFIER);
		expect(lexer, token_type::OPEN_PARENTHESIS);
		expect(lexer, token_type::CLOSE_PARENTHESIS);

		expect(lexer, token_type::OPEN_BRACE);
		
		if (lexer->peek_token() == token_type::CLOSE_BRACE)
		{
			return function;
		}

		function->body->statements.push_back(parse_statement(lexer));
		while (lexer->next_token() != token_type::CLOSE_BRACE)
		{
			function->body->statements.push_back(parse_statement(lexer));
		}
		
		expect(lexer, token_type::CLOSE_BRACKET);
		
		return function;
	}
	
	ast::program* parse(lexer* lexer)
	{
		auto* program_node = new ast::program();
		
		auto next_token = lexer->peek_token();
		while (next_token != token_type::EOF)
		{
			if (next_token == token_type::FUNCTION)
			{
				lexer->next_token();
				auto func_def = std::unique_ptr<ast::function_definition>(parse_function_definition(lexer));
				program_node->body.emplace_back(std::move(func_def));
			}
			else
			{
				auto statement = parse_statement(lexer);
				program_node->body.emplace_back(std::move(statement));
			}
			next_token = lexer->peek_token();
		}

		return program_node;
	}
}