#include "config_parser.h"
#include "dbg.h"


std::optional<std::deque<std::vector<int>>> ConfigParser::getCodes() const
{
	if(codes.empty())
		return {};
	else
		return {codes};
}

std::optional<std::string> ConfigParser::getKbdPath() const
{
	if(kbdPath.empty())
		return {};
	else
		return {kbdPath};
}

std::optional<std::string> ConfigParser::getFontPath() const
{
	if(fontPath.empty())
		return {};
	else
		return {fontPath};
}

std::optional<int> ConfigParser::getWinHeight() const
{
	if(winHeight == -1)
		return {};
	else
		return {winHeight};
}

std::optional<int> ConfigParser::getWinWidth() const
{
	if(winWidth == -1)
		return {};
	else
		return {winWidth};
}

ConfigParser::ConfigParser(const std::string& path)
{
	readData(path);
}

void ConfigParser::readData(const std::string& path)
{
	ConfigReader rdr(path);
	auto layout = rdr.getValue("layout");
	if(layout) parseCodes(*layout);

	auto kbd = rdr.getValue("kbd");
	if(kbd) kbdPath = *kbd;

	auto font = rdr.getValue("font");
	if(font) fontPath = *font;

	auto height = rdr.getValue("height");
	if(height)
	{
		try
		{
			winHeight = std::stoi(*height);
		}
		catch(...) {}
	}

	auto width = rdr.getValue("width");
	if(width)
	{
		try
		{
			winWidth = std::stoi(*width);
		}
		catch(...) {}
	}
}

void ConfigParser::parseCodes(const std::string& layout)
{
	codes.push_back(std::vector<int>());
	auto dequeIter = codes.begin();

	auto head = layout.cbegin(), tail = head;
	auto conversion = [&]()
	{
		try {
			int num = std::stoi(std::string(tail, head));
			dequeIter->push_back(num);
		}
		catch(std::invalid_argument& e) {
			dequeIter->push_back(0);
		}
		tail = head + 1;
	};

	while(1)
	{
		if(*head == ',')
		{
			conversion();
		}
		else if(*head == ';')
		{
			conversion();
			codes.push_back(std::vector<int>());
			dequeIter++;
		}

		if(++head == layout.cend())
		{
			conversion();
			break;
		}
	}

	if(codes.back().size() == 0)
		codes.pop_back();
}
