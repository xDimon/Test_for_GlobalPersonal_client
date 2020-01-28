// Copyright © 2020 Dmitriy Khaustov. All rights reserved.
//
// Author: Dmitriy Khaustov aka xDimon
// Contacts: khaustov.dm@gmail.com
// File created on: 2020.01.26

// Position.hpp

#pragma once

namespace mtfind
{

struct Position
{
	size_t line = 0;
	size_t column = 0;
	size_t position = 0;

	Position(size_t l, size_t c, size_t p)
		: line(l), column(c), position(p)
	{}

	bool operator==(const Position& rhs) const noexcept
	{
		return rhs.line == line && rhs.column == column && rhs.position == position;
	}
};

}
