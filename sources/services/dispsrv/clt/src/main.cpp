#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "DispCltCmdline.h"
using namespace std;
int main(int argc, const char* argv[])
{
	DispCltCmdline CmdLine;
	CmdLine.parse_cmdline_arguments(argc,(char**)argv);
	return 0;
}
