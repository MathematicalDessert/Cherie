#include <iostream>
#include <compilation/lexer.h>


#include "state.h"
#include "compilation/parser.h"
#include "vm/instruction.h"

template< typename T > std::array<std::byte, sizeof(T) >  to_bytes(const T& object)
{
	std::array< std::byte, sizeof(T) > bytes;

	auto begin = reinterpret_cast<const std::byte*>(std::addressof(object));
	auto end = begin + sizeof(T);
	std::copy(begin, end, std::begin(bytes));

	return bytes;
}

template< typename T >
T& from_bytes(const std::array<std::byte, sizeof(T) >& bytes, T& object)
{
	static_assert(std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");

	auto begin_object = reinterpret_cast<std::byte*>(std::addressof(object));
	std::copy(std::begin(bytes), std::end(bytes), begin_object);

	return object;
}

int main()
{
	/*auto lexer = std::make_unique<cherie::compiler::lexer>(std::wstring(LR"(
fn hello(a: int) -> ([text] string, [random]? int) {
	return "World!", a;
}

print("Hello " + hello(0).text);
	)"));*/

	std::vector<cherie::vm::instruction> instrs = {
		cherie::vm::instruction(cherie::vm::opcode::nop, 0, 0, 0),
		cherie::vm::instruction(cherie::vm::opcode::load, 15, 0, 0),
		cherie::vm::instruction(cherie::vm::opcode::load, -2, 0, 1),
		cherie::vm::instruction(cherie::vm::opcode::push, 0, 0, 1),
		cherie::vm::instruction(cherie::vm::opcode::pop, 0, 0, 1),
		cherie::vm::instruction(cherie::vm::opcode::halt, 0, 0, 0),
	};

	auto state = std::make_unique<cherie::state_raw>();
	state->program = instrs;
	state->run();
	

	try
	{
		auto lexer = std::make_unique<cherie::compiler::lexer>(R"(
			print(true, false)
		)");

		auto x = parse(lexer.get());
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
