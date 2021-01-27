#include "app.h"
#include "config_parser.h"

#include <iostream>


const static std::string WINDOW_TITLE = "SDL shit";
const static std::string PATH_TO_CONFIG = "/home/gyro/docs/prog/projects/kbdmon/resources/config";
const static int FONT_SIZE = 100;

int main()
{
	ConfigParser prsr(PATH_TO_CONFIG);
	App::Conf conf {
		.h = prsr.getWinHeight().value_or(480),
		.w = prsr.getWinWidth().value_or(640),
		.title = WINDOW_TITLE,
		.kbdPath = prsr.getKbdPath().value(),
		.fontPath = prsr.getFontPath().value(),
		.fontSize = FONT_SIZE,
		.codesLayout = prsr.getCodes().value_or(std::deque<std::vector<int>>{{0x10, 0x11, 0x12}})
	};

	App a(conf);
	return a.exec();
}

