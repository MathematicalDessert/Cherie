/*
 * File Name: instruction.h
 * Author(s): P. Kamara
 *
 * VM Instruction.
 */

#pragma once

namespace cherie::vm
{
	enum class opcode : std::uint8_t
	{
		nop,
		push,
		pop,
		load,
		add,
		addr,
		halt,
	};

	enum class addressing_mode
	{
		imm,
		reg,
		direct,
		mem,
	};

#pragma pack(push, 1)
	template <size_t Bits, typename T>
	constexpr T sign_extend(const T& v) noexcept {
		static_assert(std::is_integral<T>::value, "T is not integral");
		static_assert((sizeof(T) * 8u) >= Bits, "T is smaller than the specified width");
		if constexpr ((sizeof(T) * 8u) == Bits) return v;
		else {
			using s = struct { long long val : Bits; };
			return reinterpret_cast<const s*>(&v)->val;
		}
	}

	template <size_t Bytes, typename Type>
	struct sized_bit_container
	{
		std::byte val[Bytes];

		explicit sized_bit_container(Type v)
		{
			operator=(v);
		}

		sized_bit_container() = default;

		Type operator=(Type rhs)
		{
			std::memcpy(val, &rhs, Bytes);
			return rhs;
		}

		explicit operator Type() const
		{
			Type res;
			std::memcpy(&res, val, Bytes);
			return sign_extend<Bytes * 8>(res);
		}
	};

	using int24_t = sized_bit_container<3, std::int32_t>;
	using int56_t = sized_bit_container<7, std::int64_t>;
	static_assert(sizeof(int24_t) == 3);
	static_assert(sizeof(int56_t) == 7);
	
	struct instruction
	{
		union
		{
			struct
			{
				opcode op;
				union
				{
					int56_t al;
					struct
					{
						uint32_t a;
						union
						{
							int24_t b;
							struct
							{
								int16_t bs;
								int8_t c;
							};
						};
					};
				};
			};
			uint64_t raw = 0;
		};

		explicit instruction() = default;
		
		explicit instruction(const opcode op)
			: op(op) {}

		explicit instruction(const opcode op, const int56_t al)
			: op(op), al(al) {}

		explicit instruction(const opcode op, const int32_t a, const int24_t b)
			: op(op), a(a), b(b) {}

		explicit instruction(const opcode op, const int32_t a, const int16_t bs, const int8_t c = 0)
			: op(op), a(a), bs(bs), c(c) {}
	};
}
#pragma pack(pop)
