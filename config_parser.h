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
	ConfigParser(const std::string& path);

	std::optional<std::string> getKbdPath() const noexcept;
	std::optional<std::string> getFontPath() const noexcept;
	std::optional<int> getWinHeight() const noexcept;
	std::optional<int> getWinWidth() const noexcept;
	std::optional<std::vector<std::vector<std::string>>> getLayout() const noexcept;
	std::optional<std::map<std::string, int>> getScancodes() const noexcept;
private:
	std::vector<std::vector<std::string>> layout;
	std::map<std::string, int> scancodes;
	std::string kbd_path;
	std::string font_path;
	int win_height;
	int win_width;

	void parseLayoutMap(const std::map<std::string, std::string>& layout_map) noexcept;
	void parseScancodes(const std::map<std::string, std::string>& scancodes_map) noexcept;
	void readData(const std::string& path);

	static void parseIntString(int& dest, const std::string& str) noexcept;
};

#endif // CONFIG_PARSER_H
