// Copyright © 2020 Dmitriy Khaustov. All rights reserved.
//
// Author: Dmitriy Khaustov aka xDimon
// Contacts: khaustov.dm@gmail.com
// File created on: 2020.01.26

// main.cpp

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include "Constants.hpp"
#include "MtFind.hpp"

int main(int argc, char** argv)
{
	// Check arguments number
	if (argc != 3)
	{
		std::cerr
			<< "Usage:\n"
			<< "\t" << argv[0] << " <filename> <template>" << std::endl;
		return EXIT_FAILURE;
	}

	const std::string filename(argv[1]);
	const std::string mask(argv[2]);

	// Check mask size
	if (mask.empty())
	{
		std::cerr
			<< "Error: mask mustn't be empty" << std::endl;
		return EXIT_FAILURE;
	}
	if (mask.size() > mtfind::MAX_MASK_SIZE)
	{
		std::cerr
			<< "Error: lenght of mask must be upto 100" << std::endl;
		return EXIT_FAILURE;
	}

	// Get file stat
	struct stat64 st{};
	stat64(filename.data(), &st);

	if (errno)
	{
		std::cerr
			<< "Error: can't get file info: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Check file size
	if (st.st_size > mtfind::MAX_FILE_SIZE)
	{
		std::cerr
			<< "Error: too large file (exspect upto " << mtfind::MAX_FILE_SIZE << " bytes)" << std::endl;
		return EXIT_FAILURE;
	}

	// Trivial case
	if (st.st_size < mask.size())
	{
		std::cout << "0" << std::endl;
		return EXIT_SUCCESS;
	}

	// Open file
	std::ifstream ifs(filename.data());
	if (!ifs.is_open())
	{
		std::cerr
			<< "Error: can't open file: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	// Target function
	auto result = MtFind(filename, mask);

	std::string foundString;
	foundString.resize(mask.size());

	// Output
	std::cout << result.size() << '\n';
	for (auto& match : result)
	{
		ifs.seekg(match.position);
		ifs.read(&foundString[0], mask.size());

		std::cout
//			<< match.position << ' '
			<< match.line << ' '
			<< match.column << ' '
			<< foundString << '\n';
	}

	return EXIT_SUCCESS;
}
