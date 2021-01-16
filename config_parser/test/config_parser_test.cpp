#include "../config_parser.h"
#include <cassert>
#include <cstdlib>
#include <fstream>

#include <dbg.h>
#include <filesystem>
#include <iostream>

int open_file()
{
	return 0;
}

int test2()
{
	return 0;
}

int main(int, char** argv)
{
	switch(atoi(argv[1]))
	{
	case 1:	return open_file();
	case 2:	return test2();
	default: return 1;
	}
}

