#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "GpioCltCmdline.h"
using namespace std;
int main(int argc, const char* argv[])
{
	GpioCltCmdline CmdLine;
	CmdLine.parse_cmdline_arguments(argc,(char**)argv);
	return 0;
}
