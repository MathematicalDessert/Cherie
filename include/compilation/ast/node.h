/*
 * File Name: node.h
 * Author(s): P. Kamara
 *
 * AST nodes.
 */

#pragma once

#include <memory>
#include "conf.h"

namespace cherie::compiler::ast
{
	struct node
	{
		
	};

	struct statement
		: node
	{
		
	};

	struct statement_list
		: node
	{
		std::vector<std::unique_ptr<statement>> statements;
	};
	
	struct expression
		: statement
	{
		
	};

	struct string_literal
		: expression
	{
		explicit string_literal(types::string value)
			: value(std::move(value)) {}
		
		types::string value;
	};

	struct number_literal
		: expression
	{
		explicit number_literal(long long value)
			: value(value) {}

		std::variant<long long> value;
	};
	
	struct expression_list
		: node
	{
		std::vector<std::unique_ptr<expression>> expressions;
	};

	struct unary_expression
		: expression
	{
		token_type operation;
		std::unique_ptr<expression> expression_data;
	};

	struct binary_expression
		: expression
	{
		token_type operation;
		std::unique_ptr<expression> lhs;
		std::unique_ptr<expression> rhs;
	};

	struct call_expression
		: expression
	{
		explicit call_expression(types::string func_name)
			: function_name(std::move(func_name)) {}

		types::string function_name;
		std::vector<std::unique_ptr<expression>> arguments;
	};

	struct function_definition
		: statement_list
	{
		types::string function_name;
	};
	
	struct program
		: node
	{
		std::vector<std::variant<std::unique_ptr<statement>, std::unique_ptr<function_definition>>> body;
	};
}