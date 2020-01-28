// Copyright © 2020 Dmitriy Khaustov. All rights reserved.
//
// Author: Dmitriy Khaustov aka xDimon
// Contacts: khaustov.dm@gmail.com
// File created on: 2020.01.26

// MtFind.cpp

#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include "MtFind.hpp"

std::vector<mtfind::Position> MtFind(const std::string& filepath, const std::string& mask, size_t definedThreadNum)
{
	std::vector<mtfind::Position> result;

	struct stat64 st{};
	stat64(filepath.data(), &st);

	// Trivial case
	if (st.st_size < mask.size())
	{
		return result;
	}

	// Calculation of needed threads
	size_t workersNumber = std::min<size_t>(
		st.st_size / mask.size(),
		std::thread::hardware_concurrency()
	);

	// Redefine threads num (for unit-tests)
	if (definedThreadNum)
	{
		workersNumber = definedThreadNum;
	}

	size_t portion = (st.st_size + workersNumber - 1) / workersNumber;

	std::vector<std::unique_ptr<mtfind::Worker>> workers;
	for (size_t workerId = 0; workerId < workersNumber; ++workerId)
	{
		workers.emplace_back(
			std::make_unique<mtfind::Worker>(
				mask, filepath, workerId * portion, portion
			)
		);
	}

	mtfind::Position lastPosition(0, 0, 0);

//	std::ifstream ifs(filepath.data());
//
//	std::string str;
//	str.resize(portion);

	for (auto& worker : workers)
	{
		worker->thread().join();

//		static size_t n = 0;
//		ifs.seekg(n++ * portion);
//		memset(&str[0], 0, portion);
//		ifs.read(&str[0], portion);
//
//		std::cout << "\n=============================================\n"
//		    << str << "\n\n"
//			"End at L=" << worker->lastPosition().line << " "
//			"C=" << worker->lastPosition().column << " "
//			"P=" << worker->lastPosition().position << "\n\n";
//		std::cout << worker->result().size() <<  " \n";

		for (auto& match : worker->result())
		{
			if (lastPosition.position > match.position)
			{
				continue;
			}

//			ifs.seekg(match.position);
//			char str[mask.size()+1];
//			ifs.read(str, mask.size());
//			str[mask.size()]=0;
//
//			std::cout
//				<< match.position << ' '
//				<< match.line << ' '
//				<< match.column << ' '
//				<< str << '\n';

			if (match.line == 0)
			{
				match.column += lastPosition.column;
			}
			match.line += lastPosition.line;
			result.emplace_back(match.line+1, match.column+1, match.position);
			lastPosition.position = match.position + mask.size();
		}

		if (worker->lastPosition().line == 0)
		{
			lastPosition.column += worker->lastPosition().column + 1;
		}
		else
		{
			lastPosition.column = worker->lastPosition().column;
		}
		lastPosition.line += worker->lastPosition().line;
	}

	return result;
}
