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
    std::unique_ptr<ast::program> parse(std::unique_ptr<lexer> lexer);
}
