/*
 * File Name: visitor.h
 * Author(s): P. Kamara
 *
 * Abstract Visitor Class.
 */

#pragma once

#define VIRTUAL_VISITOR(x) virtual void visit(x* node) = 0;
#define FINAL_VISITOR(x) void visit(x* node) override

namespace cherie::compiler::ast
{
	struct if_statement;
	struct call_expression;
	struct function_definition;
	struct statement_block;
	struct statement;
	struct expression;
	struct unary_expression;
	struct additive_expression;
	struct multiplicative_expression;
	struct primary_expression;
	struct binary_expression;
	struct string_literal;
	struct number_literal;
	struct boolean_literal;
	struct program;

	struct visitor
	{
		visitor() = default;
		virtual ~visitor() = default;

		VIRTUAL_VISITOR(program)
		VIRTUAL_VISITOR(boolean_literal)
		VIRTUAL_VISITOR(number_literal)
		VIRTUAL_VISITOR(string_literal)
		VIRTUAL_VISITOR(binary_expression)
		VIRTUAL_VISITOR(primary_expression)
		VIRTUAL_VISITOR(multiplicative_expression)
		VIRTUAL_VISITOR(additive_expression)
		VIRTUAL_VISITOR(unary_expression)
		VIRTUAL_VISITOR(expression)
		VIRTUAL_VISITOR(statement)
		VIRTUAL_VISITOR(statement_block)
		VIRTUAL_VISITOR(function_definition)
		VIRTUAL_VISITOR(call_expression)
		VIRTUAL_VISITOR(if_statement)
	};
}