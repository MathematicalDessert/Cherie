#include <iostream>
#include <compilation/lexer.h>


#include "state.h"
#include "compilation/parser.h"
#include "compilation/ast/visitors/print_visitor.h"
#include "vm/instruction.h"

int main()
{
	/*auto lexer = std::make_unique<cherie::compiler::lexer>(std::wstring(LR"(
fn hello(a: int) -> ([text] string, [random]? int) {
	return "World!", a;
}

print("Hello " + hello(0).text);
	)"));*/

	/*auto state = std::make_unique<cherie::state_raw>();
	state->program = instrs;
	state->run();

	std::cout << sizeof(cherie::vm::instruction) << std::endl;
	*/
	// 2 + (2 + 1) / 3 * 2 - a
	try
	{
		const auto lexer = std::make_unique<cherie::compiler::lexer>(R"(
			let a = 1;
			print(a + 2);
		)");

		auto* p = new cherie::compiler::parser(lexer.get());
		auto* vis = new cherie::compiler::ast::print_visitor();
		p->parse()->accept(vis);
	}
	catch (std::exception& exception)
	{
		std::cout << exception.what() << std::endl;
	}
	/*auto token = lexer->next_token();
	while(token != cherie::compiler::token_type::EOF)
	{
		x->body.pop_back();
		printf("[ID: %d] %s\n", token, cherie::compiler::get_token_str(token));
		token = lexer->next_token();
	}*/
}
