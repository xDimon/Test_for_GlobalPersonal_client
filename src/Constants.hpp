// Copyright © 2020 Dmitriy Khaustov. All rights reserved.
//
// Author: Dmitriy Khaustov aka xDimon
// Contacts: khaustov.dm@gmail.com
// File created on: 2020.01.26

// Constants.hpp

#pragma once

namespace mtfind
{
	constexpr size_t MAX_FILE_SIZE = 1 << 30; // 1Gb
	constexpr size_t MAX_MASK_SIZE = 100; // 100b
	constexpr size_t WORKER_BUFFER_SIZE = 1<<13; // 8kb
}
