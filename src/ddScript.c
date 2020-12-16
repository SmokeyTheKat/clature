#include <ddcDef.h>
#include <ddcString.h>
#include <ddcTime.h>

#include <stdio.h>

#include "./lexer.h"
#include "./utils.h"
#include "./parser.h"
#include "./generate.h"
#include "./file.h"
#include "./compile.h"
#include "./system.h"

#include <stdio.h>
#include <stdlib.h>

bool debug;

int main(int agsc, char** ags)
{
	compile_main(agsc, ags);
	system_compile_nasm(agsc, ags);
	system_compile_ld(agsc, ags);
	return 0;
}
