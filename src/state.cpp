/*
 * File Name: state.cpp
 * Author(s): P. Kamara
 *
 * Cherie State.
 */

#include "state.h"

namespace cherie
{
	void state_raw::execute()
	{
		while (true)
		{
			const auto& next_instruction = program[registers.pc++];
			switch (next_instruction.op)
			{
				case vm::opcode::nop: /* no operation */
				{
					break;
				}
				case vm::opcode::push: /* push value from register onto stack */
				{
					stack.push_back(registers.gpr[next_instruction.reg]);
					break;
				}
				case vm::opcode::pop: /* push value from stack into a register*/
				{
					registers.gpr[next_instruction.reg] = stack.back();
					stack.pop_back();
					break;
				}
				case vm::opcode::load: /* loads value into register */
				{
					registers.gpr[next_instruction.ro.reg] = next_instruction.ro.operand;
					break;
				}
				case vm::opcode::halt: /* terminates execution*/
				{
					return;
				}
				default:
				{
					break;
				}
			}
		}
	}
}