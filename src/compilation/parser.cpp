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
	parser::parser(lexer* lexer)
		: lexer_(lexer) {}

	void parser::expect(const token_type type) const
	{
		if (const auto next_token = lexer_->next_token(); next_token != type)
		{
			parser_error("expected '%s' but got '%s' on line %d, column %d", get_op_symbol(type), get_op_symbol(next_token), lexer_->line(), lexer_->column());
		}
	}

	ast::call_expression* parser::parse_call_expression()
	{
		auto* expression = new ast::call_expression();

		expression->function_name = get_token_value<types::string>();
		
		expect(token_type::OPEN_PARENTHESIS);
		
		if (lexer_->peek_token() == token_type::CLOSE_PARENTHESIS)
		{
			lexer_->next_token();
			return expression;
		}

		expression->arguments.push_back(std::unique_ptr<ast::expression>(parse_expression()));
		while (lexer_->peek_token() == token_type::COMMA)
		{
			lexer_->next_token();
			expression->arguments.push_back(std::unique_ptr<ast::expression>(parse_expression()));
		}
		expect(token_type::CLOSE_PARENTHESIS);
		
		return expression;
	}

	ast::primary_expression* parser::parse_primary_expression()
	{
		switch (lexer_->next_token())
		{
			case token_type::IDENTIFIER:
			{
				if (lexer_->peek_token() == token_type::OPEN_PARENTHESIS)
				{
					return parse_call_expression();
				}
				return new ast::variable(std::get<types::string>(lexer_->token_value()));
			}
			case token_type::TRUE: return new ast::boolean_literal(true);
			case token_type::FALSE:  return new ast::boolean_literal(false);
			case token_type::LITERAL:
			{
				const auto literal = lexer_->token_value();
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
			default:
			{
				break;
			}
		}
		return nullptr;
	}
	
	ast::multiplicative_expression* parser::parse_multiplicative_expression()
	{
		auto* current_expression = parse_primary_expression();

		auto next_peeked_token = lexer_->peek_token();
		switch (next_peeked_token)
		{
			case token_type::MULTIPLY:
			case token_type::DIVIDE:
			{
				while (next_peeked_token == token_type::MULTIPLY || next_peeked_token == token_type::DIVIDE)
				{
					const auto next_token = lexer_->next_token(); // wouldn't evaluate, so this is a hack
					current_expression = new ast::binary_expression(next_token, current_expression, parse_primary_expression());
					next_peeked_token = lexer_->peek_token();
				}
			}
			default:
			{
				break;
			}
		}
		return current_expression;
	}

	ast::additive_expression* parser::parse_additive_expression()
	{
		auto* current_expression = parse_multiplicative_expression();

		auto next_peeked_token = lexer_->peek_token();
		switch (next_peeked_token)
		{
			case token_type::ADD:
			case token_type::SUBTRACT:
			{
				while (next_peeked_token == token_type::ADD || next_peeked_token == token_type::SUBTRACT)
				{
					const auto next_token = lexer_->next_token(); // wouldn't evaluate, so this is a hack
					current_expression = new ast::binary_expression(next_token, current_expression, parse_multiplicative_expression());
					next_peeked_token = lexer_->peek_token();
				}
				break;
			}
			default:
			{
				break;
			}
		}
		return current_expression;
	}

	ast::expression* parser::parse_expression()
	{
		if (lexer_->peek_token() == token_type::OPEN_PARENTHESIS)
		{
			expect(token_type::OPEN_PARENTHESIS);
			auto* expression = parse_expression();
			expect(token_type::CLOSE_PARENTHESIS);
			return expression;
		}
		return parse_additive_expression();
	}

	ast::function_definition* parser::parse_function_definition()
	{
		auto* func_def = new ast::function_definition();

		func_def->function_name = expect_and_get<types::string>(token_type::IDENTIFIER);
		
		expect(token_type::OPEN_PARENTHESIS);
		expect(token_type::CLOSE_PARENTHESIS);

		expect(token_type::OPEN_BRACE);

		if (lexer_->peek_token() == token_type::CLOSE_BRACE)
		{
			return func_def;
		}

		func_def->body->statements.push_back(std::unique_ptr<ast::statement>(parse_statement()));
		while (lexer_->next_token() != token_type::CLOSE_BRACE)
		{
			func_def->body->statements.push_back(std::unique_ptr<ast::statement>(parse_statement()));
		}

		expect(token_type::CLOSE_BRACKET);
		
		return func_def;
	}
	
	ast::statement_block* parser::parse_statement_block()
	{
		expect(token_type::OPEN_BRACE);
		
		auto* statement_block = new ast::statement_block();

		while (lexer_->peek_token() != token_type::CLOSE_BRACE)
		{
			auto new_statement = std::unique_ptr<ast::statement>(parse_statement());
			statement_block->statements.emplace_back(std::move(new_statement));
		}

		expect(token_type::CLOSE_BRACE);
		return statement_block;
	}

	ast::assignment_statement* parser::parse_assignment_statement()
	{
		auto* statement = new ast::assignment_statement();

		if (lexer_->next_token() == token_type::CONST)
		{
			statement->immutable = true;
		}
		
		statement->variable_name = expect_and_get<types::string>(token_type::IDENTIFIER);
		expect(token_type::EQUALS);
		statement->value = std::unique_ptr<ast::expression>(parse_expression());
		
		return statement;
	}
	
	ast::while_statement* parser::parse_while_statement()
	{
		expect(token_type::WHILE);

		auto* new_statement = new ast::while_statement();

		expect(token_type::OPEN_PARENTHESIS);
		new_statement->condition = std::unique_ptr<ast::expression>(parse_expression());
		expect(token_type::CLOSE_PARENTHESIS);

		new_statement->block = std::unique_ptr<ast::statement_block>(parse_statement_block());
		
		return new_statement;
	}

	ast::if_statement* parser::parse_if_statement()
	{
		expect(token_type::IF);
		
		auto* new_statement = new ast::if_statement();

		expect(token_type::OPEN_PARENTHESIS);
		new_statement->condition = std::unique_ptr<ast::expression>(parse_expression());
		expect(token_type::CLOSE_PARENTHESIS);

		new_statement->main_block = std::unique_ptr<ast::statement_block>(parse_statement_block());

		while (lexer_->peek_token() == token_type::ELSE)
		{
			lexer_->next_token();
			if (lexer_->peek_token() == token_type::IF) // else if
			{
				lexer_->next_token();
				//new_statement->elseif_blocks.insert(std::make_pair(parse_expression(), parse_statement_block()));
			}
			else
			{
				new_statement->else_block = std::unique_ptr<ast::statement_block>(parse_statement_block());
				break;
			}
		}
		
		return new_statement;
	}
	
	ast::statement* parser::parse_statement()
	{
		ast::statement* new_statement = nullptr;
		
		switch(lexer_->peek_token())
		{
			case token_type::LET:
			case token_type::CONST:
			{
				new_statement = parse_assignment_statement();
				expect(token_type::SEMICOLON);
				break;
			}
			case token_type::IF: // If Statement
			{
				new_statement = parse_if_statement();
				break;
			}
			case token_type::WHILE:
			{
				new_statement = parse_while_statement();
				break;
			}
			default:
			{
				new_statement = parse_expression();
				expect(token_type::SEMICOLON);
				break;
			}
		}

		return new_statement;
	}

	ast::program* parser::parse()
	{
		auto* program_node = new ast::program();

		auto next_token = lexer_->peek_token();
		while (next_token != token_type::EOF)
		{
			if (next_token == token_type::FUNCTION) // Function Definition
			{
				lexer_->next_token();
				auto new_function = std::unique_ptr<ast::function_definition>(parse_function_definition());
				program_node->body.emplace_back(std::move(new_function));
			}
			else // Any other generic statement
			{
				auto new_statement = std::unique_ptr<ast::statement>(parse_statement());
				program_node->body.emplace_back(std::move(new_statement));
			}
			
			next_token = lexer_->peek_token();
		}
		
		return program_node;
	}
}
