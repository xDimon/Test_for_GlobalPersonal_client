// Copyright © 2020 Dmitriy Khaustov. All rights reserved.
//
// Author: Dmitriy Khaustov aka xDimon
// Contacts: khaustov.dm@gmail.com
// File created on: 2020.01.26

// Worker2.cpp

#include "Worker.hpp"

mtfind::Worker::Worker(std::string mask, const std::string& filepath, size_t offset, size_t remain)
	: _f(fopen(filepath.c_str(), "r"), &fclose)
	, _mask(std::move(mask))
	, _offset(offset)
	, _size(remain)
	, _buff()
	, _lastPosition(0, 0, offset)
{
	assert(_mask.begin() != _mask.end());
	assert(_f != nullptr);
	fseek(_f.get(), offset, SEEK_SET);
	_thread = std::thread([this] { run(); });
}

void mtfind::Worker::run()
{
	size_t line = 0;
	size_t column = 0;

	for (size_t position = _offset; position < _offset + _size; ++position, ++_next)
	{
		_lastPosition.position = position;
		_lastPosition.line = line;
		_lastPosition.column = column;

		auto currentMatchingPosition = _lastPosition;

		for (size_t mPos = 0; mPos < _mask.size(); ++mPos, ++position, ++_next)
		{
			_lastPosition.position = position;
			_lastPosition.line = line;
			_lastPosition.column = column - ((position >= _offset + _size) ? (position - (_offset + _size) + 1) : 0);

			if (_next == _last)
			{
				size_t readSize = _offset + _size - position + _mask.size() - 1;
				if (readSize == 0)
				{
					break;
				}

				auto moveSize = std::min<size_t>(_next - _begin, _mask.size());

				memmove(_begin, _last - moveSize, moveSize);

				readSize = fread(_begin + moveSize, 1, std::min((_end - _begin) - moveSize, readSize), _f.get());
				if (readSize == 0)
				{
					break;
				}

				_base += (_last - _begin) - moveSize;

				_next = _begin + moveSize;
				_last = _next + readSize;
			}

			char inputSymbol = *_next;
			auto maskSymbol = _mask[mPos];

			if (inputSymbol == '\n')
			{
				++line;
				column = 0;
				_lastPosition.line++;
				_lastPosition.column = 0;
//					std::cout << std::endl;
				break;
			}
//				std::cout << inputSymbol << maskSymbol << std::endl;
			++column;

			if (maskSymbol != inputSymbol && maskSymbol != '?')
			{
				_next -= mPos;
				column -= mPos;
				position -= mPos;

				break;
			}

			if (mPos == _mask.size() - 1)
			{
				_result.emplace_back(currentMatchingPosition);

				_next -= mPos;
				column -= mPos;
				position -= mPos;

				break;
			}
		}
	}
}
