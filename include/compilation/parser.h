/*
 * File Name: parser.h
 * Author(s): P. Kamara
 *
 * Parser.
 */

#pragma once

#include "lexer.h"
#include "ast/node.h"

namespace cherie::compiler
{
    std::unique_ptr<ast::expression> parse_expression(lexer* lexer, const int right_binding_power = 0);
    std::unique_ptr<ast::statement> parse_statement(lexer* lexer);
	ast::program* parse(lexer* lexer);
}
