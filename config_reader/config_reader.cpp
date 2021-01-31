#include "config_reader.h"
#include <cassert>


ConfigReader::ConfigReader(const std::string& path)
{
	f.open(path);
	if(!f.is_open())
	{
		throw FileError();
	}
	else
	{
		if(parse() == -1)
		{
			throw ParsingError();
		}
	}
}

int ConfigReader::openAndParse(const std::string& path)
{
	if(f.is_open())
		f.close();

	f.open(path);
	if(f.is_open())
	{
		if(parse() == -1)
		{
			return -1;
		}
		return keys_parsed;
	}
	else
		return -1;
}

std::optional<std::string> ConfigReader::getValue(
	const std::string& section,
	const std::string& key) const noexcept
{
	try {
		return {data.at(section).at(key)};
	}
	catch(...)
	{
		return {};
	}
}

std::optional<std::map<std::string, std::string>> ConfigReader::getSection(const std::string& section_key) const noexcept
{
	try {
		return {data.at(section_key)};
	}
	catch(...)
	{
		return {};
	}
}

std::string ConfigReader::trimString(const std::string& str) noexcept
{
	std::string::const_iterator front = str.cbegin();
	std::string::const_reverse_iterator back = str.crbegin();
	for(; front != str.cend(); front++)
	{
		if(!std::isspace(*front))
		{
			break;
		}
	}
	if(front != str.end())
	{
		for(; back != str.crend(); back++)
		{
			if(!std::isspace(*back))
			{
				break;
			}
		}
	}
	return {front, back.base()};
}

int ConfigReader::parse()
{
	assert(f.is_open());
	std::string line;
	keys_parsed = 0;
	std::string section{};
	while(std::getline(f, line))
	{
		line = trimString(line);
		if((line.size() == 0) || (line.front() == '#'))
			continue;
		if(line.front() == '=')
			return -1;

		if(line.front() == '[')
		{
			if(line.back() == ']')
			{
				section = trimString({line.cbegin() + 1, line.cend() - 1});
				continue;
			}
			else
				return -1;
		}

		std::string::const_iterator delimiter = line.cend();
		std::string::const_iterator head = line.cbegin();
		for(; head != line.cend(); head++)
		{
			if((*head == '#') && (delimiter == line.cend()))
				return -1;

			if((*head == '=') && (delimiter == line.cend()))
				delimiter = head;

			if(*head == '#')
				break;
		}
		if(delimiter != line.cend())
		{
			auto ret = data[section].insert({trimString({line.cbegin(), delimiter}), trimString({delimiter + 1, head})});
			if(!ret.second)
				return -1;
			keys_parsed++;
		}
		else
			return -1;
	}
	return keys_parsed;
}
