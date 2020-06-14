#include <iostream>
#include <compilation/lexer.h>

int main()
{
	const auto lexer = new cherie::compiler::lexer(std::wstring(LR"(
fn hello(a: int) -> ([text] string, [random]? int) {
	return "World!", a;
}

print("Hello " + hello(0).text);
	)"));

	auto token = lexer->next_token();
	while(token != cherie::compiler::token_type::EOF)
	{
		printf("[ID: %d] %s\n", token, cherie::compiler::get_token_str(token));
		token = lexer->next_token();
	}
}