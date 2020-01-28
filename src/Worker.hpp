// Copyright © 2020 Dmitriy Khaustov. All rights reserved.
//
// Author: Dmitriy Khaustov aka xDimon
// Contacts: khaustov.dm@gmail.com
// File created on: 2020.01.26

// Worker.hpp

#pragma once


#include <cstdio>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <cassert>
#include <cstring>
#include "Constants.hpp"
#include "Position.hpp"

namespace mtfind
{

class Worker
{
private:
	std::string _mask;
	std::unique_ptr<FILE, decltype(&fclose)> _f;
	size_t _offset;
	size_t _size;

	std::array<char, mtfind::WORKER_BUFFER_SIZE> _buff;

	char* const _begin = _buff.data();
	char* _next = _buff.data();
	char* _last = _buff.data();
	char* const _end = _buff.data() + _buff.size();

	size_t _base = 0;

	std::thread _thread;

	std::vector<Position> _result;
	Position _lastPosition;

	void run();

public:
	Worker(std::string mask, const std::string& filepath, size_t offset, size_t remain);
	Worker(const Worker&& that) = delete;
	Worker(Worker&& that) noexcept = delete;

	auto& result()
	{
		return _result;
	}

	auto& lastPosition()
	{
		return _lastPosition;
	}

	auto& thread()
	{
		return _thread;
	}
};

}
