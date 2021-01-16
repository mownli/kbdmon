#ifndef SCANCODEMAP_H
#define SCANCODEMAP_H

#include <unordered_map>
#include <string>
#include <set>


class ScanCodeMap
{
	const std::unordered_map<int, const std::string> scmap;
public:
	ScanCodeMap() : scmap(
	{
		{0x10, "Q"},
		{0x11, "W"},
		{0x12, "R"},
		{0x13, "T"},
		{0x14, "Y"},
		{0x15, "U"},
		{0x16, "I"},
		{0x17, "O"},
		{0x18, "P"}
	}) {};

	const std::string& at(int c) const { return scmap.at(c); }
};


#endif // SCANCODEMAP_H
