/*
 * File Name: print_visitor.cpp
 * Author(s): P. Kamara
 *
 * Visits AST and prints out.
 */

#pragma once

#include "compilation/ast/node.h"

namespace cherie::compiler::ast
{
	struct print_visitor final : visitor
	{
		FINAL_VISITOR(program)
		{
			for (const auto& stmt : node->body)
			{
				if (std::holds_alternative<std::unique_ptr<statement>>(stmt))
				{
					std::get<std::unique_ptr<statement>>(stmt)->accept(this);
				}
			}
		}

		FINAL_VISITOR(boolean_literal)
		{
			printf(node->value ? "true" : "false");
		}


		FINAL_VISITOR(number_literal)
		{
			if (std::holds_alternative<types::integer>(node->value))
			{
				printf("%lld", std::get<types::integer>(node->value));
			}
			else
			{
				printf("%f", std::get<types::floating_point>(node->value));
			}
		}


		FINAL_VISITOR(string_literal)
		{
			printf("\"%s\"", node->value.c_str());
		}

		FINAL_VISITOR(binary_expression)
		{
			printf("(");
			node->lhs->accept(this);
			printf("%s", get_op_symbol(node->operation));
			node->rhs->accept(this);
			printf(")");
		}

		FINAL_VISITOR(primary_expression)
		{
			node->accept(this);
		}

		FINAL_VISITOR(multiplicative_expression)
		{
			node->accept(this);
		}

		FINAL_VISITOR(additive_expression)
		{
			node->accept(this);
		}

		FINAL_VISITOR(unary_expression)
		{
			printf("(%s", get_op_symbol(node->operation));
			printf(")");
		}

		FINAL_VISITOR(expression)
		{
			node->accept(this);
		}

		FINAL_VISITOR(statement)
		{
			node->accept(this);
		}

		FINAL_VISITOR(statement_block)
		{
			node->accept(this);
		}

		FINAL_VISITOR(function_definition)
		{
			node->accept(this);
		}

		FINAL_VISITOR(call_expression)
		{
			printf("%s(", node->function_name.c_str());

			for (auto arg_idx = 0; arg_idx < node->arguments.size(); arg_idx++)
			{
				node->arguments.at(arg_idx)->accept(this);
				if (arg_idx + 1 < node->arguments.size())
				{
					printf(",");
				}
			}

			printf(")");
		}

		FINAL_VISITOR(if_statement)
		{
			printf("if (");
			node->condition->accept(this);
			printf(") {\n");
			for (const auto& stmt : node->main_block->statements)
			{
				printf("    ");
				stmt->accept(this);
				printf("\n");
			}
			printf("}");
		}
	};
}