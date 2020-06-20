#include <iostream>
#include <compilation/lexer.h>


#include "state.h"
#include "compilation/parser.h"
#include "vm/instruction.h"

int main()
{
	/*auto lexer = std::make_unique<cherie::compiler::lexer>(std::wstring(LR"(
fn hello(a: int) -> ([text] string, [random]? int) {
	return "World!", a;
}

print("Hello " + hello(0).text);
	)"));*/

	std::vector<cherie::vm::instruction> instrs = {
		cherie::vm::instruction(cherie::vm::opcode::nop),
		cherie::vm::instruction(cherie::vm::opcode::load, 0, 15),
		cherie::vm::instruction(cherie::vm::opcode::load, 1, 9999999999),
		cherie::vm::instruction(cherie::vm::opcode::push, 1),
		cherie::vm::instruction(cherie::vm::opcode::pop, 0),
		cherie::vm::instruction(cherie::vm::opcode::halt),
	};
	
	auto state = std::make_unique<cherie::state_raw>();
	state->program = instrs;
	state->execute();
	
	try
	{
		auto lexer = std::make_unique<cherie::compiler::lexer>(R"(
			print(true, false)
		)");

		auto x = cherie::compiler::parse(lexer.get());
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
