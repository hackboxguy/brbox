#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "I2CCltCmdline.h"
using namespace std;
int main(int argc, const char* argv[])
{
	I2CCltCmdline CmdLine;
	CmdLine.parse_cmdline_arguments(argc,(char**)argv);
	return 0;
}
