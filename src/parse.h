#ifndef __ddcomp_parse_h__
#define __ddcomp_parse_h__

#include "./compile.h"

static sizet format_code_get_lines(ddString code)
{
	sizet lineCount = 1;
	for (sizet i = 0; i < code.length; i++)
		if (code.cstr[i] == '\n') lineCount++;
	return lineCount;
}

static void format_code_init_slines(ddString* slines)
{
	slines = make(ddString, lineCount);
	for (sizet i = 0; i < lineCount; i++)
		slines[i] = make_ddString("");
}

struct codeLine* format_code(ddString code)
{
	sizet lineCount = format_code_get_lines(code);
	ddString* slines;// output lines after clearimg comments and macros
	format_code_init_slines(slines);
	bool writing = true;
	int lptr = 0;
	for (sizet i = 0; i < code.length; i++)
	{
		switch (code.cstr[i])
		{
			case ';'://start comment
				writing = false;
				break;
			case '\n'://end comment + inc line ptr
				writing = true;
				lptr++;
				break;
			default:
				if (writing)
					ddString_push_char_back(&(slines[lptr]), code.cstr[i]);
				break;
		}
	}
	struct codeLine* lines = make(struct codeLine, lineCount);
	for (sizet i = 0; i < lineCount; i++)
	{
		sizet spaceCount = 1;
		for (sizet j = 0; j < slines[i].length; j++)
			if (slines[i].cstr[j] == ' ') spaceCount++;
		lines[i].data = make(ddString, spaceCount);
		lines[i].length = spaceCount;
		for (sizet j = 0; j < spaceCount; j++)
			lines[i].data[j] = make_ddString("");
		int sptr = 0;
		for (sizet j = 0; j < slines[i].length; j++)
		{
			if (slines[i].cstr[j] == ' ')
			{
				sptr++;
				continue;
			}
			ddString_push_char_back(&(lines[i].data[sptr]), slines[i].cstr[j]);
		}
	}
	for (sizet i = 0; i < lineCount; i++)
		raze_ddString(&(slines[i]));
	raze(slines);
	return lines;
}

struct bitCode generate_bitcode(struct codeLine* lines, sizet lineCount)
{
	struct bitCommand* bcode = make(struct bitCommand, 20);
	struct bvar* vars = make(struct bvar, 20);
	sizet bl = 0;
	sizet vc = 0;
	
	bool ieq = false;
	int gn = false;
	int sn = false;
	uint32t sp = 0;
	uint32t msp = 0;
	ddString svn = make_ddString("");
	for (sizet i = 0; i < lineCount; i++)
	{
		sizet lc = bl;
		for (int j = 0; j < lines[i].length; j++)
		{
			//if (ddString_compare_cstring(lines[i].data[j], )
			if (lines[i].data[j].cstr[0] == '@')
			{
				if (lines[i].data[j].cstr[1] == '1')
					bcode[bl].command = DC_DEF1;
				else if (lines[i].data[j].cstr[1] == '2')
					bcode[bl].command = DC_DEF2;
				else if (lines[i].data[j].cstr[1] == '4')
					bcode[bl].command = DC_DEF4;
				else if (lines[i].data[j].cstr[1] == '8')
					bcode[bl].command = DC_DEF8;
				bcode[bl].t1 = DT_REG;
				sp += -(lines[i].data[j].cstr[1] - 48);
				msp += abs(-(lines[i].data[j].cstr[1] - 48));
				bcode[bl].r1o = sp;
				bcode[bl].r1.vreg = r_rbp;
				sn = true;

				bcode[bl].t2 = DT_REG;
				bcode[bl].r2o = 0;
			}
			else if (ddString_compare_cstring(lines[i].data[j], "="))
			{
				ieq = true;
			}
			else if (is_number(lines[i].data[j]))
			{
				if (ieq) // in equal statement
				{
					bcode[bl].t2 = DT_INT;
					bcode[bl].r2o = 0;
					int vall = ddString_to_int(lines[i].data[j]);
					bcode[bl].r2.vint = vall;
					vars[vc].name = make_ddString(svn.cstr);
					vars[vc].type = DT_INT;
					vars[vc].value.vint = vall;
				}
			}
		}
		bl++;
	}
	struct bitCode output;
	output.bcommands = bcode;
	output.length = bl;
	output.stackSize = sp;
	return output;
}
#endif
