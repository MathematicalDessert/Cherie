/*
 * File Name: virtual_machine.h
 * Author(s): P. Kamara
 *
 * Virtual Machine.
 */

#pragma once
#include <vector>

#include "instruction.h"

namespace cherie::vm
{
    using vm_register = signed long long;
    struct register_table
    {
        vm_register pc;
        vm_register gpr[9];
    };
	
	class virtual_machine
	{
        std::vector<vm_register> stack;
        register_table registers = {};
	//protected:
        
	public:
        std::vector<instruction> program;
        void run();
	};
}
