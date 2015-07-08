#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "HotwireCltCmdline.h"
using namespace std;
int main(int argc, const char* argv[])
{
	HotwireCltCmdline CmdLine;
	CmdLine.parse_cmdline_arguments(argc,(char**)argv);
	return 0;
}
