#include <iostream>
#include <compilation/lexer/lexer.h>

int main()
{
	const auto lexer = new cherie::compiler::lexer(std::wstring(LR"(
	a = 2;
	print(a - 4);
	)"));

	cherie::compiler::token_type token = lexer->next_token();
	while(token != cherie::compiler::token_type::EOF)
	{
		printf("%d\n", token);
		token = lexer->next_token();
	}
}