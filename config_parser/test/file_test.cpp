#include "../config_parser.h"
#include <cassert>
#include <fstream>


int main(int, char** argv)
{
	const std::string res = argv[1];
	const std::string path_a = res + "/a";

	std::fstream f(path_a);
	if(f.bad()) abort();

	return 0;
}

