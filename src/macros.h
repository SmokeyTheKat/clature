#ifndef __ddScript_macros_h__
#define __ddScript_macros_h__

#include "./file.h"

struct macroDefs
{
	ddString lhs;
	ddString rhs;
};

static struct macroDefs defs[1000];
static sizet defCount = 0;

static void set_def(ddString lhs, ddString rhs)
{
	defs[defCount].lhs = lhs;
	defs[defCount++].rhs = rhs;
}
static sizet get_nextline_pos(ddString* file, sizet low)
{
	for (sizet i = low; file->length; i++)
	{
		if (file->cstr[i] == '\n') return i;
	}
	return 0;
}
static ddString read_to_end(ddString* file, sizet low)
{
	ddString output = make_ddString("");
	for (sizet i = low; file->cstr[i] != '\n'; i++)
		ddString_push_char_back(&output, file->cstr[i]);
	return output;
}
static ddString read_word(ddString* file, sizet low)
{
	ddString output = make_ddString("");
	for (sizet i = low; file->cstr[i] != ' ' && file->cstr[i] != '\n'; i++)
		ddString_push_char_back(&output, file->cstr[i]);
	return output;
}

void execute_macros(ddString* file)
{
	for (sizet i = 0; i < defCount; i++)
	{
		for (sizet j = 0; j < file->length; j++)
		{
			if (file->cstr[j-1] >= 'a' &&
			    file->cstr[j-1] <= 'z' ||
			    file->cstr[j-1] >= 'A' &&
			    file->cstr[j-1] <= 'Z' ||
			    file->cstr[j-1] >= '0' &&
			    file->cstr[j-1] <= '9' ||
			    file->cstr[j-1] == '_' ||
			    file->cstr[j+defs[i].lhs.length] >= 'a' &&
			    file->cstr[j+defs[i].lhs.length] <= 'z' ||
			    file->cstr[j+defs[i].lhs.length] >= 'A' &&
			    file->cstr[j+defs[i].lhs.length] <= 'Z' ||
			    file->cstr[j+defs[i].lhs.length] >= '0' &&
			    file->cstr[j+defs[i].lhs.length] <= '9' ||
			    file->cstr[j+defs[i].lhs.length] == '_') continue;

			bool found = true;
			for (sizet k = 0; k < defs[i].lhs.length; k++)
			{
				if (file->cstr[j+k] != defs[i].lhs.cstr[k]) found = false;
			}
			if (found)
			{
				for (sizet k = 0; k < defs[i].lhs.length; k++)
				{
					ddString_delete_at(file, j);
				}
				for (sizet k = 0; k < defs[i].rhs.length; k++)
				{
					ddString_insert_char_at(file, defs[i].rhs.cstr[k], j+k);
				}
			}
		}
	}
}

void read_macros(ddString* file)
{
	for (sizet i = 0; i < file->length; i++)
	{
		for (sizet j = 0; j < defCount; j++)
		{
			switch (file->cstr[i+defs[j].lhs.length])
			{
				case 'a' ... 'z':
				case 'A' ... 'Z':
				case '0' ... '9':
				case '_':
					continue;
			}
			switch (file->cstr[i-1])
			{
				case 'a' ... 'z':
				case 'A' ... 'Z':
				case '0' ... '9':
				case '_':
					continue;
			}
			bool found = true;
			for (sizet k = 0; k < defs[j].lhs.length; k++)
			{
				if (file->cstr[i+k] != defs[j].lhs.cstr[k]) found = false;
			}
			if (found)
			{
				for (sizet k = 0; k < defs[j].lhs.length; k++)
				{
					ddString_delete_at(file, i);
				}
				for (sizet k = 0; k < defs[j].rhs.length; k++)
				{
					ddString_insert_char_at(file, defs[j].rhs.cstr[k], i+k);
				}
				i = i+defs[i].rhs.length;
			}
		}
		if (file->cstr[i] == '/')
		{
			if (file->cstr[i+1] == 'd' &&
			    file->cstr[i+2] == 'e' &&
			    file->cstr[i+3] == 'f')
			{
				// /def @8 int
				ddString lhs = read_word(file, i+5);
				ddString rhs = read_to_end(file, i+5+lhs.length+1);
				set_def(lhs, rhs);
				file->cstr[i] = ';';
			}
			else if (file->cstr[i+1] == 'i' &&
				 file->cstr[i+2] == 'n' &&
			    	 file->cstr[i+3] == 'c')
			{
				ddString path = read_word(file, i+5);
				sizet pathlen = path.length;
				if (path.cstr[0] == '~')
				{
					ddString incpath = make_ddString("usr/share/ds/include/");
					path.cstr[0] = '/';
					for (sizet k = 0; k < incpath.length; k++)
					{
						ddString_insert_char_at(&path, incpath.cstr[k], 1+k);
					}
				}
				ddPrint_ddString_nl(path);
				ddString newfile = read_file(path.cstr);
				ddString_push_char_back(&newfile, '\n');
				sizet k;
				for (k = 0; k < newfile.length; k++)
				{
					ddString_insert_char_at(file, newfile.cstr[k], (i+5+pathlen+1)+k);
				}
				file->cstr[i] = ';';
				i = i+5+pathlen;
			}
		}
	}
}

#endif
