#include "app.h"
#include "config_parser.h"

#include <iostream>

const static char* WINDOW_TITLE = "SDL shit";
const static char* PATH_TO_CONFIG = "/home/gyro/docs/prog/projects/kbdmon/resources/config";

static App::Conf prepareConf()
{
	ConfigParser prsr(PATH_TO_CONFIG);

	App::Conf conf {
		.win_height = prsr.getWinHeight().value_or(100),
		.win_width = prsr.getWinWidth().value_or(100),
		.title = WINDOW_TITLE,
		.layout = prsr.getLayout().value_or(decltype(conf.layout){{"q"}}),
		.scancodes = prsr.getScancodes().value_or(decltype(conf.scancodes){{"q", 16}})
	};
	std::optional<std::string> kbd = prsr.getKbdPath();
	std::optional<std::string> font = prsr.getFontPath();
	if(kbd && font)
	{
		conf.kbd_path = *kbd;
		conf.font_path = *font;
	}
	else
	{
		std::cerr << "Provide input device and a font face" << std::endl;
		exit(1);
	}

	return conf;
}

int main()
{
	App::Conf conf(prepareConf());
	App a(conf);
	return a.exec();
}

