

#include "node.h"

namespace cherie::compiler::ast
{
	struct test_visitor final : visitor
	{
		void visit(program* node) override;
		void visit(boolean_literal* node) override;
		void visit(number_literal* node) override;
		void visit(string_literal* node) override;
		void visit(binary_expression* node) override;
		void visit(primary_expression* node) override;
		void visit(multiplicative_expression* node) override;
		void visit(additive_expression* node) override;
		void visit(unary_expression* node) override;
		void visit(expression* node) override;
		void visit(statement* node) override;
	};

	inline void test_visitor::visit(program* node)
	{
		printf("START OF PROGRAM!\n");
	}

	inline void test_visitor::visit(boolean_literal* node)
	{
		printf("true");
	}

	inline void test_visitor::visit(number_literal* node)
	{
		if (std::holds_alternative<types::integer>(node->value))
		{
			printf(" %lld ", std::get<types::integer>(node->value));
		}
		else
		{
			printf(" %f ", std::get<types::floating_point>(node->value));
		}
	}

	inline void test_visitor::visit(string_literal* node)
	{
		printf("\"%s\"", node->value.c_str());
	}

	inline void test_visitor::visit(binary_expression* node)
	{
		
	}

	inline void test_visitor::visit(primary_expression* node)
	{

	}

	inline void test_visitor::visit(multiplicative_expression* node)
	{

	}

	inline void test_visitor::visit(additive_expression* node)
	{

	}

	inline void test_visitor::visit(unary_expression* node)
	{

	}

	inline void test_visitor::visit(expression* node)
	{

	}

	inline void test_visitor::visit(statement* node)
	{

	}
}
