/*
 * File Name: node.h
 * Author(s): P. Kamara
 *
 * AST nodes.
 */

#pragma once

#include <memory>
#include <vector>
#include "conf.h"
#include "compilation/token.h"

namespace cherie::compiler::ast
{
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

	enum class node_type
	{
		none
	};

	struct visitor
	{
		visitor() = default;
		virtual ~visitor() = default;

		virtual void visit(program* node) = 0;
		virtual void visit(boolean_literal* node) = 0;
		virtual void visit(number_literal* node) = 0;
		virtual void visit(string_literal* node) = 0;
		virtual void visit(binary_expression* node) = 0;
		virtual void visit(primary_expression* node) = 0;
		virtual void visit(multiplicative_expression* node) = 0;
		virtual void visit(additive_expression* node) = 0;
		virtual void visit(unary_expression* node) = 0;
		virtual void visit(expression* node) = 0;
		virtual void visit(statement* node) = 0;
	};
	
	struct node
	{
		template<typename T>
		T cast_to()
		{
			static_assert(!std::is_base_of<node, T>(), "T must inherit from node!");
			return reinterpret_cast<T>(this);
		};
		
		[[nodiscard]] bool is_a(const node_type type) const { return type_ == type; }

		virtual void accept(visitor* visitor) = 0;
	private:
		node_type type_ = node_type::none;
	};

	struct statement
		: node
	{
		void accept(visitor* visitor) override
		{
			visitor->visit(this);
		}
	};

	struct expression
		: statement
	{
		void accept(visitor* visitor) override
		{
			visitor->visit(this);
		}
	};

	struct unary_expression
		: expression
	{
		token_type operation;
		std::unique_ptr<expression> rhs;
		
		void accept(visitor* visitor) override
		{
			visitor->visit(this);
		}
	};

	struct additive_expression
		: expression
	{
		void accept(visitor* visitor) override
		{
			visitor->visit(this);
		}
	};
	
	struct multiplicative_expression
		: additive_expression
	{
		void accept(visitor* visitor) override
		{
			visitor->visit(this);
		}
	};

	struct primary_expression
		: multiplicative_expression
	{
		void accept(visitor* visitor) override
		{
			visitor->visit(this);
		}
	};

	struct binary_expression
		: primary_expression
	{
		binary_expression(token_type op, primary_expression* lhs, primary_expression* rhs)
			: operation(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

		binary_expression(token_type op, additive_expression* lhs, additive_expression* rhs)
			: operation(op), lhs(std::move(reinterpret_cast<primary_expression*>(lhs))), rhs(std::move(reinterpret_cast<primary_expression*>(rhs))) {}

		void accept(visitor* visitor) override
		{
			
			printf("(");
			lhs->accept(visitor);
			printf("%s", get_op_symbol(operation));
			rhs->accept(visitor);
			printf(")");
		}
		
		token_type operation;
		std::unique_ptr<primary_expression> lhs;
		std::unique_ptr<primary_expression> rhs;
	};
	
	struct string_literal
		: primary_expression
	{
		explicit string_literal(types::string value)
			: value(std::move(value)) {}

		void accept(visitor* visitor) override
		{
			visitor->visit(this);
		}
		
		types::string value;
	};

	struct number_literal
		: primary_expression
	{
		explicit number_literal(types::integer value)
			: value(value) {}

		explicit number_literal(types::floating_point value)
			: value(value) {}

		void accept(visitor* visitor) override
		{
			visitor->visit(this);
		}
		
		std::variant<types::integer, types::floating_point> value;
	};

	struct boolean_literal
		: primary_expression
	{
		explicit boolean_literal(const bool value)
			: value(value) {}

		void accept(visitor* visitor) override
		{
			visitor->visit(this);
		}
		
		bool value;
	};
	
	struct program
		: node
	{
		void accept(visitor* visitor) override
		{
			visitor->visit(this);
			for (const auto& stmt : body)
			{
				if (std::holds_alternative<std::unique_ptr<statement>>(stmt))
				{
					std::get<std::unique_ptr<statement>>(stmt)->accept(visitor);
				}

			}
		}
		
		std::vector<std::variant<std::unique_ptr<statement>>> body;
	};
}
