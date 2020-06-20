/*
 * File Name: state.h
 * Author(s): P. Kamara
 *
 * Cherie State.
 */

#pragma once

#include <memory>
#include <vector>
#include "vm/instruction.h"

namespace cherie
{
    using vm_register = long long;
    struct register_table
    {
        vm_register pc;
        vm_register gpr[9];
    };
	
	struct state_raw
	{
		std::vector<vm_register> stack;
        register_table registers;
        std::vector<vm::instruction> program;

        void execute();
	};
    using state = std::unique_ptr<state_raw>;
}
