/*
 * File Name: node.h
 * Author(s): P. Kamara
 *
 * AST nodes.
 */

#pragma once

#include <map>
#include <memory>
#include <vector>
#include "conf.h"
#include "compilation/token.h"
#include "visitors/visitor.h"

#define NODE_ACCEPT void accept(visitor* visitor) override \
					{ \
						visitor->visit(this); \
					} \

namespace cherie::compiler::ast
{
	enum class node_type
	{
		none,
		program_node,
	};
	
	struct node
	{
		virtual ~node() = default;

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

	struct statement_block
		: node
	{
		std::vector<std::unique_ptr<statement>> statements;
		
		NODE_ACCEPT
	};

	struct function_definition final
		: node
	{
		std::string function_name;
		std::unique_ptr<statement_block> body;

		NODE_ACCEPT
	};
	
	struct if_statement final
		: statement
	{
		NODE_ACCEPT
		
		std::unique_ptr<expression> condition;
		std::unique_ptr<statement_block> main_block;
		std::map<expression, std::unique_ptr<statement_block>> elseif_blocks;
		std::unique_ptr<statement_block> else_block;
	};

	struct expression
		: statement
	{
		NODE_ACCEPT
	};

	struct unary_expression
		: expression
	{
		token_type operation;
		std::unique_ptr<expression> rhs;
		
		NODE_ACCEPT
	};

	struct additive_expression
		: expression
	{
		NODE_ACCEPT
	};
	
	struct multiplicative_expression
		: additive_expression
	{
		NODE_ACCEPT
	};

	struct primary_expression
		: multiplicative_expression
	{
		NODE_ACCEPT
	};

	struct call_expression
		: primary_expression
	{
		NODE_ACCEPT
		
		std::string function_name;
		std::vector<std::unique_ptr<expression>> arguments;
	};
	
	struct binary_expression
		: primary_expression
	{
		binary_expression(const token_type op, primary_expression* lhs, primary_expression* rhs)
			: operation(op), lhs(lhs), rhs(rhs) {}

		binary_expression(const token_type op, additive_expression* lhs, additive_expression* rhs)
			: operation(op), lhs(reinterpret_cast<primary_expression*>(lhs)), rhs(reinterpret_cast<primary_expression*>(rhs)) {}

		NODE_ACCEPT
		
		token_type operation;
		std::unique_ptr<primary_expression> lhs;
		std::unique_ptr<primary_expression> rhs;
	};
	
	struct string_literal
		: primary_expression
	{
		explicit string_literal(types::string value)
			: value(std::move(value)) {}

		NODE_ACCEPT
		
		types::string value;
	};

	struct number_literal
		: primary_expression
	{
		explicit number_literal(types::integer value)
			: value(value) {}

		explicit number_literal(types::floating_point value)
			: value(value) {}

		NODE_ACCEPT
		
		std::variant<types::integer, types::floating_point> value;
	};

	struct boolean_literal
		: primary_expression
	{
		explicit boolean_literal(const bool value)
			: value(value) {}

		NODE_ACCEPT
		
		bool value;
	};
	
	struct program
		: node
	{
		NODE_ACCEPT
		
		std::vector<std::variant<std::unique_ptr<function_definition>, std::unique_ptr<statement>>> body;
	};
}
