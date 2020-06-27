/*
 * File Name: virtual_machine.cpp
 * Author(s): P. Kamara
 *
 * Virtual Machine.
 */

#include "vm/virtual_machine.h"

namespace cherie::vm
{
	void virtual_machine::run()
	{
		while (true)
		{
			const auto& next_instruction = program[registers.pc++];
			switch (next_instruction.op)
			{
				case opcode::nop: /* no operation */
				{
					break;
				}
				case opcode::push: /* push value from register onto stack */
				{
					stack.push_back(registers.gpr[next_instruction.c]);
					break;
				}
				case opcode::pop: /* push value from stack into a register*/
				{
					registers.gpr[next_instruction.c] = stack.back();
					stack.pop_back();
					break;
				}
				case opcode::load: /* loads value into register */
				{
					registers.gpr[next_instruction.c] = next_instruction.a;
					break;
				}
				case opcode::add: /* R(n) = R(n) + R(z) */
				{
					//registers.gpr[next_instruction.c] = registers.gpr[next_instruction.bs] + next_instruction.as;
					break;
				}
				case opcode::addr:
				{
					break;
				}
				case opcode::halt: /* terminates execution*/
				{
					return;
				}
				default:
				{
					return; // Bad opcode
				}
			}
		}
	}
}