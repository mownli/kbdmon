#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <vector>
#include <deque>
#include <string>
#include <stdexcept>

#include "config_reader/config_reader.h"


class ConfigParser
{
public:
	ConfigParser() {};
	ConfigParser(const std::string& path);

	std::optional<std::deque<std::vector<int>>> getCodes() const;
	std::optional<std::string> getKbdPath() const;
	std::optional<std::string> getFontPath() const;
	std::optional<int> getWinHeight() const;
	std::optional<int> getWinWidth() const;
private:
	std::deque<std::vector<int>> codes{};
	std::string kbdPath{};
	std::string fontPath{};
	int winHeight = -1;
	int winWidth = -1;

	void parseCodes(const std::string& layout);
	void readData(const std::string& path);
};

#endif // CONFIG_PARSER_H
