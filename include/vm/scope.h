/*
 * File Name: scope.h
 * Author(s): P. Kamara
 *
 * Virtual Machine scope.
 */

#pragma once

#include <memory>

namespace cherie::vm
{
	struct scope
	{
        std::unique_ptr<scope> parent;
	};

    static auto global_scope = std::make_shared<scope>();
}