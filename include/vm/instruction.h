/*
 * File Name: instruction.h
 * Author(s): P. Kamara
 *
 * VM Instruction.
 */

#pragma once

namespace cherie::vm
{
	enum class opcode
	{
		nop,
		push,
		pop,
		load,
		halt,
	};

	enum class addressing_mode
	{
		imm,
		reg,
		direct,
		mem,
	};
	
	struct instruction
	{
		union
		{
			struct
			{
				opcode op: 7;
				union
				{
					int reg : 3;
					struct
					{
						int reg: 3;
						long long operand : 54;
					} ro; 
				};
			};
			long long raw;
		};

		explicit instruction(const opcode op)
			: op(op) {}

		explicit instruction(const opcode op, const int reg)
			: op(op), reg(reg) {}
		
		explicit instruction(const opcode op, const int reg, const long long value)
			: op(op) {
			ro.reg = reg;
			ro.operand = value;
		}
	};
	//static_assert(sizeof(instruction) == sizeof(long long));
}
