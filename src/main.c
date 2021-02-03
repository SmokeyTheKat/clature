#include <ddcDef.h>
#include <ddcString.h>
#include <ddcTime.h>

#include <stdio.h>
#include <unistd.h>

#define MAX_OFILE_SIZE 100000000
#define MAX_TOKEN_SIZE 100000000
#define MAX_TREE_SIZE  1000000

#include "./args.h"
#include "./utils.h"
#include "./lexer.h"
#include "./parser.h"
#include "./generate.h"
#include "./file.h"
#include "./compile.h"
#include "./system.h"

#include <stdio.h>
#include <stdlib.h>

bool debug;

int main(int argc, char** argv)
{
	init_qalloc(15000000000);
	read_args(argc, argv);
	compile_main(argc, argv);
	if (args_if_def(make_constant_ddString("-o")))
	{
		system_compile_nasm();
		system_compile_ld();
		if (!args_if_def(make_constant_ddString("--no-clean-up")))
			system_compile_clear();
	}
	return 0;
}
