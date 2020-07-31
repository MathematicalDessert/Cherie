/*
 * File Name: state.h
 * Author(s): P. Kamara
 *
 * Cherie State.
 */

#pragma once

#include <memory>
#include "vm/virtual_machine.h"

namespace cherie
{
	struct state_raw
        : vm::virtual_machine
	{
		
	};
    using state = std::unique_ptr<state_raw>;
}
