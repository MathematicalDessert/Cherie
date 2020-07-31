/*
 * File Name: codegen_visitor.cpp
 * Author(s): P. Kamara
 *
 * Visits AST and generates bytecode.
 */

#include "compilation/ast/node.h"

namespace cherie::compiler::ast
{
    class codegen_visitor final : visitor
    {
        int pc_ = 0;
    public:
        codegen_visitor() {};
        ~codegen_visitor() {};
    	
        FINAL_VISITOR(program)
        {
            for (const auto& stmt : node->body)
            {
                if (std::holds_alternative<std::unique_ptr<statement>>(stmt))
                {
                    std::get<std::unique_ptr<statement>>(stmt)->accept(this);
                }
            }
        }

    	
    };
}