#include "config_reader.h"
#include <iostream>


ConfigReader::ConfigReader(const std::string& path)
{
	f.open(path);
	if(!f.is_open())
	{
		throw std::runtime_error("Couldn't open " + path);
	}
	else
		parse();
}

int ConfigReader::read(const std::string& path)
{
	if(f.is_open())
		f.close();

	f.open(path);
	if(f.is_open())
	{
		parse();
		return keysParsed;
	}
	else
		return -1;
}

std::string ConfigReader::getValue(const std::string& key)
{
	return data.at(key);
}

void ConfigReader::parse()
{
	std::string line;
	keysParsed = 0;
	while(std::getline(f, line))
	{
		if((line.size() == 0) || (line[0] == '#') || (line[0] == '='))
		{
			continue;
		}

		for(auto it = line.cbegin(); it != line.cend(); it++)
		{
			if(*it == '=')
			{
				data[std::string(line.cbegin(), it)] = std::string(it + 1, line.cend());
				keysParsed++;
				break;
			}
		}
	}
}
