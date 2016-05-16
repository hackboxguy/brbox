#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "KmodCltCmdline.h"
using namespace std;
int main(int argc, const char* argv[])
{
	KmodCltCmdline CmdLine;
	CmdLine.parse_cmdline_arguments(argc,(char**)argv);
	return 0;
}
