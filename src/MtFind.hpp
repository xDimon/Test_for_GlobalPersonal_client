// Copyright © 2020 Dmitriy Khaustov. All rights reserved.
//
// Author: Dmitriy Khaustov aka xDimon
// Contacts: khaustov.dm@gmail.com
// File created on: 2020.01.27

// MtFind.hpp

#pragma once

#include "Worker.hpp"

std::vector<mtfind::Position>
MtFind(
	const std::string& filepath,
	const std::string& mask,
	size_t definedThreadNum = 0
);
