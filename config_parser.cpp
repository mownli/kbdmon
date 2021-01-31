#include "config_parser.h"

#include <sstream>
#include <string>
#include <iostream>
#include <cassert>


std::optional<std::string> ConfigParser::getKbdPath() const noexcept
{
	if(kbd_path.empty())
	{
		std::cerr << "Couldn't get keyboard from the file" << std::endl;
		return {};
	}
	else
		return {kbd_path};
}

std::optional<std::string> ConfigParser::getFontPath() const noexcept
{
	if(font_path.empty())
	{
		std::cerr << "Couldn't get font from the file" << std::endl;
		return {};
	}
	else
		return {font_path};
}

std::optional<int> ConfigParser::getWinHeight() const noexcept
{
	if(win_height < 1)
	{
		std::cerr << "Couldn't get window height from the file" << std::endl;
		return {};
	}
	else
		return {win_height};
}

std::optional<int> ConfigParser::getWinWidth() const noexcept
{
	if(win_width < 1)
	{
		std::cerr << "Couldn't get window width from the file" << std::endl;
		return {};
	}
	else
		return {win_width};
}

std::optional<std::vector<std::vector<std::string>>> ConfigParser::getLayout() const noexcept
{
	if(layout.empty())
	{
		std::cerr << "Couldn't get layout from the file" << std::endl;
		return {};
	}
	else
		return {layout};
}

std::optional<std::map<std::string, int> > ConfigParser::getScancodes() const noexcept
{
	if(scancodes.empty())
	{
		std::cerr << "Couldn't get scancodes from the file" << std::endl;
		return {};
	}
	else
		return {scancodes};
}

void ConfigParser::parseLayoutMap(const std::map<std::string, std::string>& layout_map) noexcept
{
	for(const std::pair<const std::string, std::string>& row : layout_map)
	{
		layout.emplace_back();

		std::string::const_iterator tail = row.second.cbegin();
		if(tail == row.second.cend())
		{
			layout.back().emplace_back("");
			continue;
		}
		for(std::string::const_iterator head = row.second.cbegin();;)
		{
			if((*head == ',') || (*head == ';'))
			{
				layout.back().push_back(ConfigReader::trimString({tail, head}));
				tail = head + 1;
			}
			head++;
			if(head == row.second.cend())
			{
				layout.back().push_back(ConfigReader::trimString({tail, head}));
				break;
			}
		}
	}
}

ConfigParser::ConfigParser(const std::string& path)
{
	readData(path);
}

void ConfigParser::readData(const std::string& path)
{
	ConfigReader rdr(path);

	auto layout_map(rdr.getSection("layout"));
	if(layout_map) parseLayoutMap(*layout_map);

	auto scancodes_map(rdr.getSection("scancodes"));
	if(scancodes_map) parseScancodes(*scancodes_map);

	kbd_path = rdr.getValue("general", "kbd").value_or("");
	font_path = rdr.getValue("general", "font").value_or("");

	parseIntString(win_height, rdr.getValue("general", "height").value_or(""));
	parseIntString(win_width, rdr.getValue("general", "width").value_or(""));
}

void ConfigParser::parseIntString(int& dest, const std::string& str) noexcept
{
	std::istringstream ss(str);
	if(ss >> dest) {}
	else
	{
		dest = -1;
	}
}

void ConfigParser::parseScancodes(const std::map<std::string, std::string>& scancodes_map) noexcept
{
	std::stringstream ss{};
	try	{
		for(const std::vector<std::string>& row : layout)
		{
			for(const std::string& str : row)
			{
				if(str == "")
					continue;

				ss << scancodes_map.at(str);
				int num = -1;
				ss >> num;
				auto ret = scancodes.insert({str, num});
				if(!ret.second || ss.fail())
				{
					scancodes.clear();
					std::cerr << "Failure parsing scancode \"" << str << "\"" << std::endl;
					return;
				}
				ss.str("");
				ss.clear();
			}
		}
	}
	catch(std::out_of_range& e) {
		scancodes.clear();
		std::cerr << "Mismatch of layout and scancodes" << std::endl;
	}
}
