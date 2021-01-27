#include "gtest/gtest.h"
#include "../config_parser.h"


const static char* configFile("config");
const static char* configFile2("config2");

TEST(getFromConfig, first)
{
	if(const char* s = std::getenv("DATADIR"))
	{
		ConfigParser prsr(std::string(s) + "/" + configFile);
		auto vect = prsr.getCodes();
		EXPECT_EQ(vect.value()[0][0], 33);
		EXPECT_EQ(vect.value()[0][1], 44);
		EXPECT_EQ(vect.value()[1][0], 1);
		EXPECT_EQ(vect.value()[1][1], 0);
		EXPECT_EQ(vect.value()[1][2], 0);
		EXPECT_EQ(vect.value()[1][3], 0);
		EXPECT_EQ(vect.value()[2][0], 5);

		EXPECT_STREQ(prsr.getKbdPath()->c_str(), "/dev/input/by-path/pci-0000:15:00.0-usb-0:5:1.0-event-kbd");
		EXPECT_STREQ(prsr.getFontPath()->c_str(), "/usr/share/fonts/liberation/LiberationSans-Regular.ttf");
		EXPECT_EQ(prsr.getWinHeight().value(), 640);
		EXPECT_EQ(prsr.getWinWidth().value(), 480);
	}
	else
		abort();
}

TEST(getFromConfig, second)
{
	if(const char* s = std::getenv("DATADIR"))
	{
		ConfigParser prsr(std::string(s) + "/" + configFile2);
		auto vect = prsr.getCodes();
		EXPECT_EQ(vect->size(), 1);
		EXPECT_EQ(vect.value()[0][0], 33);
		EXPECT_EQ(vect.value()[0][1], 44);
		EXPECT_EQ(vect.value()[0][2], 55);
	}
	else
		abort();
}
