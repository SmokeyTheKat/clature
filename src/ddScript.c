#include <ddcDef.h>
#include <ddcString.h>

#include "./utils.h"

sizet parse_get_line_count(ddString file)
{
	sizet output = 0;
	for (int i = 0; i < file.length; i++)
		if (file.cstr[i] == '\n') output++;
	return output;
}

const char CNT_OPERATORS[8] = {
	'=', '@', '+', '-',
	'*', '/', '(', '/'
};

ddString left_split(ddString str, int ptr)
{
	ptr--;
	ddString output = make_ddString("");
	for (int i = ptr; i >= 0; i--)
		ddString_push_char_back(&output, str.cstr[i]);
	return output;
}
ddString right_split(ddString str, int ptr)
{
	ptr++;
	ddString output = make_ddString("");
	for (int i = ptr; i < str.length; i++)
		ddString_push_char_back(&output, str.cstr[i]);
	return output;
}

struct compileNode* make_compileNode(struct compileNode* parent, int type)
{
	struct compileNode* output = make(struct compileNode, 1);
	output->parent = parent;
	output->type = type;
	return output;
}

void compile_line_print_asm(const char op, const char* lhs, const char* rhs)
{
	switch (op)
	{
		case '+':
			ddPrintf("add r9, %s;\n", value);
			break;
		case '-':
			ddPrintf("sub r9, %s;\n", value);
			break;
		case '*':
			ddPrintf("mov rax, r9;\n");
			ddPrintf("mov r9, %s;\n", value);
			ddPrintf("mul r9, %s;\n", value);
			break;
		case '/':
			ddPrintf("mov rax, r9;\n");
			ddPrintf("mov r9, %s;\n", value);
			ddPrintf("div r9, %s;\n", value);
			break;
	}
}

void compile_line_gets(ddString seg, struct compileNode* cnptr)
{
	int ij = 0;
	while (ij<8)
	{
		bool found = false;
		for (int i = 0; i < seg.length; i++)
		{
			if (seg.cstr[i] == CNT_OPERATORS[ij])
			{
				found = true;
				//ddPrint_char_nl(CNT_OPERATORS[ij]);
				if (i != 0 && i != seg.length-1)
				{
					cnptr->type = CNT_OPERATORS[ij];
					ddString left = left_split(seg, i);
					struct compileNode* leftNode = make_compileNode(cnptr, 0x69);
					compile_line_gets(left, leftNode);
					cnptr->left = leftNode;
					raze_ddString(&left);

					ddString right = right_split(seg, i);
					struct compileNode* rightNode = make_compileNode(cnptr, 0x69);
					compile_line_gets(right, rightNode);
					cnptr->right = rightNode;
					raze_ddString(&right);
					return;
				}
				else return;// reutnr the value
			}
		}
		ij++;
	}
}

void compile_line(ddString line)
{
	//struct compileNode* head = make(struct compileNode, 1);
	//struct compileNode* ptr = head;
	struct compileNode* cn = make_compileNode(nullptr, 0x00);
	compile_line_gets(line, cn);
	ddPrint_char_nl(cn->type);
	ddPrint_char_nl(cn->right->type);
	ddPrint_int_nl(cn->right->right->right->right->type);
}

sizet parse_lines(ddString file)
{
	sizet lineCount = parse_get_line_count(file);
	bool inComment = false;
	ddString line = make_ddString("");
	for (int i = 0; i < file.length; i++)
	{
		switch (file.cstr[i])
		{
			case ';':
				inComment = true;
				compile_line(line);
				break;
			case '\n':
				inComment = false; 
				remake_ddString(&line, "");
				break;
			default:
				if (!inComment) ddString_push_char_back(&line, file.cstr[i]);
				break;
		}
	}
}

void macro_pass(ddString* file)
{
	
}

int main(int agsc, char** ags)
{
	ddPrint_cstring("\x1b[38;2;255;255;255m");
	if (agsc < 2) compile_error("NO INPUT FILES");
	compile_warning("STACK OVERFLOW");
	ddString file = make_ddString(ags[1]);
	sizet lineCount = parse_lines(file);
	//macro_pass(&file);
}
