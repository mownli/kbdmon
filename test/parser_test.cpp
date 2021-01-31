#include "gtest/gtest.h"
#include "../config_parser.h"
#include <iostream>

const static char* DATA_DIR_ENV = "DATADIR";

const static char* VALID_FILE("valid");
const static char* MISMATCH_FILE("mismatch");
const static char* NON_EXISTENT_FILE("dsfjdfklljkdfsljksdwoepqiropqweirpoweir");


class ConfigParserTest : public ::testing::Test
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

TEST_F(ConfigParserTest, valid)
{
	ConfigParser prsr(std::string(s) + "/" + VALID_FILE);
	auto layout = prsr.getLayout();

	std::cout << "size_outer=" << layout->size() << std::endl;

	for(auto& row : *layout)
	{
		std::cout << "size_inner=" << row.size() << std::endl;
		for(auto& str : row)
		{
			std::cout << str << std::endl;
		}
	}

	EXPECT_EQ((*layout)[0].front(), "q");
	EXPECT_EQ((*layout)[1].front(), "");
	EXPECT_EQ((*layout)[2].front(), "e");
}

TEST_F(ConfigParserTest, failure)
{
	ConfigParser prsr(std::string(s) + "/" + MISMATCH_FILE);
	EXPECT_EQ(prsr.getScancodes(), std::nullopt);

	EXPECT_ANY_THROW(ConfigParser qq(NON_EXISTENT_FILE));
}
