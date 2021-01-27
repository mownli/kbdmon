#include "gtest/gtest.h"
#include "../config_reader.h"
#include <cstdlib>

const static char* emptyFile("empty-file");
const static char* commentsFile("comments-only");
const static char* validFile("valid");

TEST(constructor, bad_path)
{
	EXPECT_ANY_THROW(ConfigReader p("non-existent path"));
}

TEST(constructor, good_path)
{
	if(const char* s = std::getenv("DATADIR"))
	{
		EXPECT_NO_THROW(ConfigReader p(std::string(s) + "/" + emptyFile));
	}
	else
		abort();
}

TEST(config_reader, bad_path)
{
	ConfigReader p;
	EXPECT_EQ(p.read("non-existent path"), -1);
}

TEST(config_reader, good_empty)
{
	ConfigReader p;
	if(const char* s = std::getenv("DATADIR"))
	{
		EXPECT_EQ(p.read(std::string(s) + "/" + emptyFile), 0);
	}
	else
		abort();
}

TEST(config_reader, only_comments)
{
	if(const char* s = std::getenv("DATADIR"))
	{
		ConfigReader p;
		EXPECT_EQ(p.read(std::string(s) + "/" + commentsFile), 0);
	}
	else
		abort();
}

TEST(config_reader, valid)
{
	if(const char* s = std::getenv("DATADIR"))
	{
		ConfigReader p;
		EXPECT_EQ(p.read(std::string(s) + "/" + validFile), 3);
		EXPECT_STREQ(p.getValue("first").value().c_str(), "one");
		EXPECT_STREQ(p.getValue("second").value().c_str(), "two");
		EXPECT_STREQ(p.getValue("third").value().c_str(), "=three");
		EXPECT_EQ(p.getValue("whatever"), std::nullopt);
	}
	else
		abort();
}

