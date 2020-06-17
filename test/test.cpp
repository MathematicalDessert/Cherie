#include <iostream>
#include <compilation/lexer.h>

#include "compilation/parser.h"

int main()
{
	/*auto lexer = std::make_unique<cherie::compiler::lexer>(std::wstring(LR"(
fn hello(a: int) -> ([text] string, [random]? int) {
	return "World!", a;
}

print("Hello " + hello(0).text);
	)"));*/

	auto lexer = std::make_unique<cherie::compiler::lexer>(R"(
/*fn hello()
{
	print("World!");
}*/
fn hello() {}
print("Hello ")
hello()
	)");

	auto x = cherie::compiler::parse(std::move(lexer));
	/*auto token = lexer->next_token();
	while(token != cherie::compiler::token_type::EOF)
	{
		x->body.pop_back();
		printf("[ID: %d] %s\n", token, cherie::compiler::get_token_str(token));
		token = lexer->next_token();
	}*/
}
