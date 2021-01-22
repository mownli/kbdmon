#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <fstream>
#include <unordered_map>


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
	std::string getValue(const std::string& key);

};

#endif // CONFIG_PARSER_H
