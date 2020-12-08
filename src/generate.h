#ifndef __ddScript_generate_h__
#define __ddScript_generate_h__

void generate_asm(struct tokenNode* node, ddString* asmBuffer, sizet* lineCount);

void generate_asm_2reg(struct tokenNode* node, const char* opc, ddString* asmBuffer, sizet* lineCount)
{
	ddString_format(&(asmBuffer[*lineCount]), "	push rax;\n");
	(*lineCount)++;
	ddString_format(&(asmBuffer[*lineCount]), "	%s r15, r14;\n", opc);
	(*lineCount)++;
	ddString_format(&(asmBuffer[*lineCount]), "	pop r15;\n");
	(*lineCount)++;
	ddString_format(&(asmBuffer[*lineCount]), "	pop r14;\n");
	(*lineCount)++;
	generate_asm(node->right, asmBuffer, lineCount);
	generate_asm(node->left, asmBuffer, lineCount);
	return;
}
void generate_asm_1reg(struct tokenNode* node, const char* opc, ddString* asmBuffer, sizet* lineCount)
{
	ddString_format(&(asmBuffer[*lineCount]), "	push rax;\n");
	(*lineCount)++;
	ddString_format(&(asmBuffer[*lineCount]), "	%s r15;\n", opc);
	(*lineCount)++;
	ddString_format(&(asmBuffer[*lineCount]), "	pop r15;\n");
	(*lineCount)++;
	ddString_format(&(asmBuffer[*lineCount]), "	pop rax;\n");
	(*lineCount)++;
	generate_asm(node->right, asmBuffer, lineCount);
	generate_asm(node->left, asmBuffer, lineCount);
	return;
}

struct variable
{
	ddString name;
	sizet spos;
	sizet size;
};

struct stackTracker
{
	sizet size;
	sizet top;
	struct variable vars[500];
	
};

static struct stackTracker stackt;

void init_generation(void)
{
	stackt.top = 0;
	stackt.size = 0;
}

void generate_add_var(ddString name, int size)
{
	stackt.vars[stackt.top].name = name;
	stackt.vars[stackt.top].size = size;
	stackt.top++;
	stackt.size += size;
	stackt.vars[stackt.top-1].spos = stackt.size;
};

const char* ASM_SIZES[5] = {
	"",
	"byte",
	"word",
	"",
	"dword"
};

void generate_print_var(ddString name, ddString* asmBuffer, sizet* lineCount)
{
	for (sizet i = 0; i < stackt.top; i++)
	{
		name.cstr[name.length] = '\0';
		stackt.vars[i].name.cstr[stackt.vars[i].name.length] = '\0';
		if (ddString_compare(name, stackt.vars[i].name))
		{
			ddString fmt = make_ddString("");
			ddString_format(&fmt, "qword [rbp-%d]", /*ASM_SIZES[stackt.vars[i].size],*/ stackt.vars[i].spos);
			ddString_push_back(&(asmBuffer[*lineCount]), fmt);
			raze_ddString(&fmt);
			return;
		}
	}
	ddString_push_back(&(asmBuffer[*lineCount]), name);
}

void generate_asm(struct tokenNode* node, ddString* asmBuffer, sizet* lineCount)
{
	if (node == nullptr) return;
	if (node->value->type == TKN_ASSEMBLY)
	{
		node->value->value.cstr[0] = '	';
		remake_ddString(&(asmBuffer[*lineCount]), node->value->value.cstr);
		ddString_push_cstring_back(&(asmBuffer[*lineCount]), ";\n");
		(*lineCount)++;
		return;
	}
	if (node->value->type == TKN_KEYWORD)
	{
		ddPrintf("FOSDF");
		return;
	}
	switch (node->value->value.cstr[0])
	{
		case ';': return;
		case '{':
		{
			generate_asm(node->right, asmBuffer, lineCount);
			break;
		}
		case '}':
		{
			break;
		}
		case '=':
		{
			if (node->left->value->value.cstr[0] == '@')
			{
				generate_add_var(node->left->right->right->value->value, ddString_to_int(node->left->right->value->value));
				ddString_format(&(asmBuffer[*lineCount]), "	pop qword [rbp-%d];\n", /*ASM_SIZES[ddString_to_int(node->left->right->value->value)],*/ stackt.vars[stackt.top-1].spos);
				(*lineCount)++;
				generate_asm(node->right, asmBuffer, lineCount);
				return;
			}
			else
			{
				ddString_format(&(asmBuffer[*lineCount]), "	pop %s;\n", node->left->value->value.cstr);
				(*lineCount)++;
				generate_asm(node->right, asmBuffer, lineCount);
			}
			break;
		}
		case '@':
		{
			ddPrintf("	FLKJDSFL:\n");
			break;
		}
		case '*':
			generate_asm_1reg(node, "mul", asmBuffer, lineCount);
			return;
		case '/':
			generate_asm_1reg(node, "div", asmBuffer, lineCount);
			return;
		case '+':
			generate_asm_2reg(node, "add", asmBuffer, lineCount);
			return;
		case '-':
			generate_asm_2reg(node, "sub", asmBuffer, lineCount);
			return;
		default:
		{
			asmBuffer[*lineCount].length = 0;
			ddString_push_cstring_back(&(asmBuffer[*lineCount]), "	push ");
			generate_print_var(node->value->value, asmBuffer, lineCount);
			ddString_push_cstring_back(&(asmBuffer[*lineCount]), ";\n");
			(*lineCount)++;
			break;
		}
	}
}

#endif
