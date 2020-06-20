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
    ast::program parse(lexer* lexer);
}
