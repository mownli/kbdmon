#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <string>
#include <fstream>
#include <map>
#include <optional>


class ConfigReader
{
private:
	std::ifstream f;
	std::map<std::string, std::map<std::string, std::string>> data;
	int keys_parsed = 0;

	int parse() ;
public:
	ConfigReader() {};
	ConfigReader(const std::string& path);
	int openAndParse(const std::string& path) ;
	std::optional<std::string> getValue(const std::string& section, const std::string& key) const noexcept;
	std::optional<std::map<std::string, std::string>> getSection(const std::string& section_key) const noexcept;

	static std::string trimString(const std::string& str) noexcept;

	class ParsingError : public std::exception {};
	class FileError : public std::exception {};
};

#endif // CONFIG_READER_H
