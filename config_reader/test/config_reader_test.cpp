#include "gtest/gtest.h"
#include "../config_reader.h"
#include <cstdlib>


const static char* DATA_DIR_ENV = "DATADIR";

const static char* EMPTY_FILE = "empty_file";
const static char* COMMENTS_AND_SECTIONS_FILE = "comments_and_sections";
const static char* SAME_KEYS_FILE = "same_keys";
const static char* NO_DELIMITER_FILE = "no_delimiter";
const static char* NO_KEY_FILE = "no_key";
const static char* BAD_SECTION_FILE = "bad_section";
const static char* NON_EXISTENT_FILE = "nonExistentFile";
const static char* VALID_FILE = "valid";


class ConfigReaderTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
	  if((s = std::getenv(DATA_DIR_ENV))) {}
	  else abort();
  }
  const char* s;
  // void TearDown() override {}
};


TEST_F(ConfigReaderTest, valid)
{
	ConfigReader p(std::string(s) + "/" + VALID_FILE);
	EXPECT_STREQ(p.getValue("", "first")->c_str(), "one");
	EXPECT_STREQ(p.getValue("", "second")->c_str(), "two");
	EXPECT_STREQ(p.getValue("sect one", "third")->c_str(), "");
	EXPECT_STREQ(p.getValue("sect one", "fourth")->c_str(), "four");
	EXPECT_STREQ(p.getValue("sect three", "fifth")->c_str(), "five");

	EXPECT_EQ(p.getValue("qwejwqe", "qwhkjsdhfkj"), std::nullopt);
}

TEST_F(ConfigReaderTest, valid_layout)
{
	ConfigReader p(std::string(s) + "/" + VALID_FILE);
	auto mapp(p.getSection("layout"));
	EXPECT_EQ(mapp->size(), 3);
	EXPECT_STREQ(mapp->at("1").c_str(), "q");
	EXPECT_STREQ(mapp->at("2").c_str(), "w");
	EXPECT_STREQ(mapp->at("3").c_str(), "e");

}

TEST_F(ConfigReaderTest, same_keys)
{
	EXPECT_THROW(ConfigReader p(std::string(s) + "/" + SAME_KEYS_FILE), ConfigReader::ParsingError);
}

TEST_F(ConfigReaderTest, no_delimiter)
{
	EXPECT_THROW(ConfigReader p(std::string(s) + "/" + NO_DELIMITER_FILE), ConfigReader::ParsingError);
}

TEST_F(ConfigReaderTest, no_key)
{
	EXPECT_THROW(ConfigReader p(std::string(s) + "/" + NO_KEY_FILE), ConfigReader::ParsingError);
}

TEST_F(ConfigReaderTest, comments_and_sections)
{
	ConfigReader p;
	EXPECT_EQ(p.openAndParse(std::string(s) + "/" + COMMENTS_AND_SECTIONS_FILE), 0);
}

TEST_F(ConfigReaderTest, empty_file)
{
	ConfigReader p;
	EXPECT_EQ(p.openAndParse(std::string(s) + "/" + EMPTY_FILE), 0);
}

TEST_F(ConfigReaderTest, bad_section)
{
	EXPECT_THROW(ConfigReader p(std::string(s) + "/" + BAD_SECTION_FILE), ConfigReader::ParsingError);
}

TEST_F(ConfigReaderTest, non_existent_file)
{
	EXPECT_THROW(ConfigReader p(std::string(s) + "/" + NON_EXISTENT_FILE), ConfigReader::FileError);
}
