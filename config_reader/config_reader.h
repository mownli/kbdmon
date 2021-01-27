#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <string>
#include <fstream>
#include <unordered_map>
#include <optional>


class ConfigReader
{
	std::ifstream f;
	std::unordered_map<std::string, std::string> data;

	void parse();
public:
	int keysParsed = 0;

	ConfigReader() {};
	ConfigReader(const std::string& path);
	int read(const std::string& path);
	std::optional<std::string> getValue(const std::string& key) const;
};

#endif // CONFIG_READER_H
