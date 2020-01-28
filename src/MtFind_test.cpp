// Copyright © 2020 Dmitriy Khaustov. All rights reserved.
//
// Author: Dmitriy Khaustov aka xDimon
// Contacts: khaustov.dm@gmail.com
// File created on: 2020.01.26

// MtFind_test.cpp

#include <gtest/gtest.h>
#include "MtFind.hpp"
#include <fstream>
#include <sstream>

struct FileContainsText
{
	static size_t n;
	const std::string name;
	FileContainsText(const std::string& text)
	: name("input-test"+std::to_string(++n)+".txt")
	{
		std::ofstream ofs(name, std::ios::trunc);
		ofs << text;
		ofs.close();
	}
	~FileContainsText()
	{
		unlink(name.c_str());
	}
};
size_t FileContainsText::n = 0;

TEST(Worker, no_contains)
{
	{
		std::string text("0123456789|bcdef");
		std::string mask("XYZ");
		FileContainsText file(text);
		mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
		auto result = worker.result();
		EXPECT_TRUE(result.empty());
		auto lastPosition = worker.lastPosition();
		EXPECT_EQ(lastPosition.line, 0);
		EXPECT_EQ(lastPosition.column, 9);
	//	EXPECT_EQ(lastPosition.position, 9);
	}
	{
		std::string text("\n123456789|bcdef");
		std::string mask("XYZ");
		FileContainsText file(text);
		mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
		auto result = worker.result();
		EXPECT_TRUE(result.empty());
		auto lastPosition = worker.lastPosition();
		EXPECT_EQ(lastPosition.line, 1);
		EXPECT_EQ(lastPosition.column, 8);
	//	EXPECT_EQ(lastPosition.position, 9);
	}
	{
		std::string text("\n123\n56789|bcdef");
		std::string mask("XYZ");
		FileContainsText file(text);
		mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
		auto result = worker.result();
		EXPECT_TRUE(result.empty());
		auto lastPosition = worker.lastPosition();
		EXPECT_EQ(lastPosition.line, 2);
		EXPECT_EQ(lastPosition.column, 4);
	//	EXPECT_EQ(lastPosition.position, 9);
	}
	{
		std::string text("\n123\n5678\n|bcdef");
		std::string mask("XYZ");
		FileContainsText file(text);
		mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
		auto result = worker.result();
		EXPECT_TRUE(result.empty());
		auto lastPosition = worker.lastPosition();
		EXPECT_EQ(lastPosition.line, 3);
		EXPECT_EQ(lastPosition.column, 0);
	//	EXPECT_EQ(lastPosition.position, 9);
	}
	{
		std::string text("\n123\n5678\n\nbcdef");
		std::string mask("XYZ");
		FileContainsText file(text);
		mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
		auto result = worker.result();
		EXPECT_TRUE(result.empty());
		auto lastPosition = worker.lastPosition();
		EXPECT_EQ(lastPosition.line, 3);
		EXPECT_EQ(lastPosition.column, 0);
	//	EXPECT_EQ(lastPosition.position, 9);
	}
}

TEST(Worker, at_begin_line_1)
{
	std::string text("XYZ3456789|bcdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0].line, 0);
	EXPECT_EQ(result[0].column, 0);
	EXPECT_EQ(result[0].position, 0);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 0);
	EXPECT_EQ(lastPosition.column, 9);
//	EXPECT_EQ(lastPosition.position, 9);
}

TEST(Worker, at_begin_line_2)
{
	std::string text("\nXYZ45689|bcdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0].line, 1);
	EXPECT_EQ(result[0].column, 0);
	EXPECT_EQ(result[0].position, 1);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 1);
	EXPECT_EQ(lastPosition.column, 8);
//	EXPECT_EQ(lastPosition.position, 9);
}

TEST(Worker, at_begin_line_3)
{
	std::string text("\n123\nXYZ9|bcdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0].line, 2);
	EXPECT_EQ(result[0].column, 0);
	EXPECT_EQ(result[0].position, 5);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 2);
	EXPECT_EQ(lastPosition.column, 4);
//	EXPECT_EQ(lastPosition.position, 9);
}

TEST(Worker, at_mid_line_1)
{
	std::string text("0XYZ456789|bcdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0].line, 0);
	EXPECT_EQ(result[0].column, 1);
	EXPECT_EQ(result[0].position, 1);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 0);
	EXPECT_EQ(lastPosition.column, 9);
//	EXPECT_EQ(lastPosition.position, 9);
}

TEST(Worker, at_mid_line_2)
{
	std::string text("\n1XYZ56789|bcdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0].line, 1);
	EXPECT_EQ(result[0].column, 1);
	EXPECT_EQ(result[0].position, 2);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 1);
	EXPECT_EQ(lastPosition.column, 8);
//	EXPECT_EQ(lastPosition.position, 9);
}

TEST(Worker, at_mid_line_3)
{
	std::string text("\n123\n5XYZ9|bcdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0].line, 2);
	EXPECT_EQ(result[0].column, 1);
	EXPECT_EQ(result[0].position, 6);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 2);
	EXPECT_EQ(lastPosition.column, 4);
//	EXPECT_EQ(lastPosition.position, 9);
}

TEST(Worker, at_end_line_last_of_1)
{
	std::string text("0123456XYZ|bcdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0].line, 0);
	EXPECT_EQ(result[0].column, 7);
	EXPECT_EQ(result[0].position, 7);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 0);
	EXPECT_EQ(lastPosition.column, 9);
//	EXPECT_EQ(lastPosition.position, 9);
}

TEST(Worker, at_end_line_last_of_2)
{
	std::string text("\n123456XYZ|bcdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0].line, 1);
	EXPECT_EQ(result[0].column, 6);
	EXPECT_EQ(result[0].position, 7);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 1);
	EXPECT_EQ(lastPosition.column, 8);
//	EXPECT_EQ(lastPosition.position, 9);
}

TEST(Worker, at_end_line_last_of_3)
{
	std::string text("\n123\n56XYZ|bcdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0].line, 2);
	EXPECT_EQ(result[0].column, 2);
	EXPECT_EQ(result[0].position, 7);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 2);
	EXPECT_EQ(lastPosition.column, 4);
//	EXPECT_EQ(lastPosition.position, 9);
}

TEST(Worker, at_end_line_last_with_tail_0)
{
	std::string text("0123456XYZabcdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0].line, 0);
	EXPECT_EQ(result[0].column, 7);
	EXPECT_EQ(result[0].position, 7);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 0);
	EXPECT_EQ(lastPosition.column, 9);
//	EXPECT_EQ(lastPosition.position, 9);
}

TEST(Worker, at_end_line_last_with_tail_1)
{
	std::string text("01234567XYZbcdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0].line, 0);
	EXPECT_EQ(result[0].column, 8);
	EXPECT_EQ(result[0].position, 8);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 0);
	EXPECT_EQ(lastPosition.column, 9);
//	EXPECT_EQ(lastPosition.position, 10);
}

TEST(Worker, at_end_line_last_with_tail_2)
{
	std::string text("012345678XYZcdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 1);
	EXPECT_EQ(result[0].line, 0);
	EXPECT_EQ(result[0].column, 9);
	EXPECT_EQ(result[0].position, 9);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 0);
	EXPECT_EQ(lastPosition.column, 9);
//	EXPECT_EQ(lastPosition.position, 11);
}

TEST(Worker, at_end_line_last_with_tail_all)
{
	std::string text("0123456789XYZdef");
	std::string mask("XYZ");
	FileContainsText file(text);
	mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
	auto result = worker.result();
	ASSERT_EQ(result.size(), 0);
//	EXPECT_EQ(result[0].line, 0);
//	EXPECT_EQ(result[0].column, 9);
//	EXPECT_EQ(result[0].position, 9);
	auto lastPosition = worker.lastPosition();
	EXPECT_EQ(lastPosition.line, 0);
	EXPECT_EQ(lastPosition.column, 9);
//	EXPECT_EQ(lastPosition.position, 9);
}

TEST(Worker, intersection)
{
	{                   //01234567890
		std::string text("abZXZXZXZcd");
		std::string mask("ZXZ");
		FileContainsText file(text);
		mtfind::Worker worker(mask, file.name, 0, 10); worker.thread().join();
		auto result = worker.result();
		ASSERT_EQ(result.size(), 3);
		EXPECT_TRUE((result[0].line == 0) && (result[0].column == 2));
		EXPECT_TRUE((result[1].line == 0) && (result[1].column == 4));
		EXPECT_TRUE((result[2].line == 0) && (result[2].column == 6));
	}
}

TEST(MtFind, find_in_empty_text)
{
	{
		std::string text("");
		std::string mask("?");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		EXPECT_TRUE(result.empty());
	}
}

TEST(MtFind, find_in_empty_lines)
{
	{
		std::string text("\n\n\n");
		std::string mask("?");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		EXPECT_TRUE(result.empty());
	}
}

TEST(MtFind, find_none)
{
	{                   //12345
		std::string text("abcde");
		std::string mask("X");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		EXPECT_TRUE(result.empty());
	}
}

TEST(MtFind, find_one_symbol)
{
	{                   //12345
		std::string text("Abcde");
		std::string mask("A");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 1);
		EXPECT_EQ(result[0].line, 1);
		EXPECT_EQ(result[0].column, 1);
	}
	{                   //12345
		std::string text("abCde");
		std::string mask("C");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 1);
		EXPECT_EQ(result[0].line, 1);
		EXPECT_EQ(result[0].column, 3);
	}
	{                   //12345
		std::string text("abcdE");
		std::string mask("E");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 1);
		EXPECT_EQ(result[0].line, 1);
		EXPECT_EQ(result[0].column, 5);
	}
	{                   //12345  12345
		std::string text("abXcd\nefXgh");
		std::string mask("X");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 2);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 3));
		EXPECT_TRUE((result[1].line == 2) && (result[0].column == 3));
	}
}

TEST(MtFind, find_pair_symbols)
{
	{                   //1234
		std::string text("ABcd");
		std::string mask("AB");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 1);
		EXPECT_EQ(result[0].line, 1);
		EXPECT_EQ(result[0].column, 1);
	}
	{                   //1234
		std::string text("aBCd");
		std::string mask("BC");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 1);
		EXPECT_EQ(result[0].line, 1);
		EXPECT_EQ(result[0].column, 2);
	}
	{                   //1234
		std::string text("abCD");
		std::string mask("CD");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 1);
		EXPECT_EQ(result[0].line, 1);
		EXPECT_EQ(result[0].column, 3);
	}
}

TEST(MtFind, find_any_one_symbol)
{
	{                   //123
		std::string text("abc");
		std::string mask("?");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 3);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 1));
		EXPECT_TRUE((result[1].line == 1) && (result[1].column == 2));
		EXPECT_TRUE((result[2].line == 1) && (result[2].column == 3));
	}
	{                   //12  12
		std::string text("ab\ncd");
		std::string mask("?");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 4);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 1));
		EXPECT_TRUE((result[1].line == 1) && (result[1].column == 2));
		EXPECT_TRUE((result[2].line == 2) && (result[2].column == 1));
		EXPECT_TRUE((result[3].line == 2) && (result[3].column == 2));
	}
}

TEST(MtFind, find_pair_any_symbols)
{
	{                   //12345
		std::string text("abcde");
		std::string mask("??");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 2);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 1));
		EXPECT_TRUE((result[1].line == 1) && (result[1].column == 3));
	}
						//0 1234 56789 01
	{                   //1  123  1234  1
		std::string text("a\nbcd\nefgh\ni");
		std::string mask("??");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 3);
		EXPECT_TRUE((result[0].line == 2) && (result[0].column == 1) && (result[0].position == 2));
		EXPECT_TRUE((result[1].line == 3) && (result[1].column == 1) && (result[1].position == 6));
		EXPECT_TRUE((result[2].line == 3) && (result[2].column == 3) && (result[2].position == 8));
	}
}

TEST(MtFind, find_any_first_symbol)
{
	{                   //123456789012
		std::string text("aZbcZdeZZfgZ");
		std::string mask("?Z");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 4);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 1));
		EXPECT_TRUE((result[1].line == 1) && (result[1].column == 4));
		EXPECT_TRUE((result[2].line == 1) && (result[2].column == 7));
		EXPECT_TRUE((result[3].line == 1) && (result[3].column == 11));
	}
	{                   //123456789012
		std::string text("ZabZcdZZefZg");
		std::string mask("?Z");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 3);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 3));
		EXPECT_TRUE((result[1].line == 1) && (result[1].column == 6));
		EXPECT_TRUE((result[2].line == 1) && (result[2].column == 10));
	}
	{                   //123  123
		std::string text("abc\nZcde");
		std::string mask("?Z");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.empty());
	}
}

TEST(MtFind, find_any_last_symbol)
{
	{                   //123456789012
		std::string text("aZbcZdeZZfgZ");
		std::string mask("Z?");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 3);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 2));
		EXPECT_TRUE((result[1].line == 1) && (result[1].column == 5));
		EXPECT_TRUE((result[2].line == 1) && (result[2].column == 8));
	}
	{                   //123456789012
		std::string text("ZabZcdZZefZg");
		std::string mask("Z?");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 4);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 1));
		EXPECT_TRUE((result[1].line == 1) && (result[1].column == 4));
		EXPECT_TRUE((result[2].line == 1) && (result[2].column == 7));
		EXPECT_TRUE((result[3].line == 1) && (result[3].column == 11));
	}
	{                   //1234  123
		std::string text("abcZ\ndef");
		std::string mask("Z?");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.empty());
	}
}

TEST(MtFind, find_any_mid_symbol)
{
	{                   //123456789012345
		std::string text("aXYZbcXYZdeXYZf");
		std::string mask("X?Z");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 3);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 2));
		EXPECT_TRUE((result[1].line == 1) && (result[1].column == 7));
		EXPECT_TRUE((result[2].line == 1) && (result[2].column == 12));
	}
	{                   //1234567890123
		std::string text("XYZabXYZcdXYZ");
		std::string mask("X?Z");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 3);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 1));
		EXPECT_TRUE((result[1].line == 1) && (result[1].column == 6));
		EXPECT_TRUE((result[2].line == 1) && (result[2].column == 11));
	}
	{                   //123  123
		std::string text("abX\nZcd");
		std::string mask("X?Z");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.empty());
	}
}

TEST(MtFind, find_any_bilateral_symbols)
{
	{                   //123456789012345
		std::string text("aXYZbcXYZdeXYZf");
		std::string mask("?Y?");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 3);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 2));
		EXPECT_TRUE((result[1].line == 1) && (result[1].column == 7));
		EXPECT_TRUE((result[2].line == 1) && (result[2].column == 12));
	}
	{                   //1234567890123
		std::string text("XYZabXYZcdXYZ");
		std::string mask("?Y?");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 3);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 1));
		EXPECT_TRUE((result[1].line == 1) && (result[1].column == 6));
		EXPECT_TRUE((result[2].line == 1) && (result[2].column == 11));
	}
	{                   //1234  1234  1234
		std::string text("abXY\nZcdX\nYZef");
		std::string mask("?Y?");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.empty());
	}
}

TEST(MtFind, find_with_intersection)
{
	{                   //12345678901
		std::string text("abZXZXZXZcd");
		std::string mask("ZXZ");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_EQ(result.size(), 2);
		EXPECT_TRUE((result[0].line == 1) && (result[0].column == 3));
		EXPECT_TRUE((result[1].line == 1) && (result[1].column == 7));
	}
}

TEST(MtFind, find_special)
{
	{
		std::string text = R"(I've paid my dues
Time after time.
I've done my sentence
But committed no crime.
And bad mistakes ?
I've made a few.
I've had my share of sand kicked in my face
But I've come through.
)";
		std::string mask("?ad");
		FileContainsText file(text);
		auto result = MtFind(file.name, mask);
		ASSERT_TRUE(result.size() == 3);
		EXPECT_TRUE((result[0].line == 5) && (result[0].column == 5));
		EXPECT_TRUE((result[1].line == 6) && (result[1].column == 6));
		EXPECT_TRUE((result[2].line == 7) && (result[2].column == 6));
	}
}
