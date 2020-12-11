#ifndef __ddScript_generate_h__
#define __ddScript_generate_h__

#define BTC_PUSH	0x00
#define BTC_POP		0x01
#define BTC_MOV		0x02
#define BTC_ADD		0x03
#define BTC_SUB		0x04
#define BTC_MUL		0x05
#define BTC_DIV		0x06
#define BTC_MOVZX	0x07
#define BTC_CMP 	0x08
#define BTC_SETE 	0x09
#define BTC_SETNE 	0x0A
#define BTC_SETG 	0x0B
#define BTC_SETGE 	0x0C
#define BTC_SETL 	0x0D
#define BTC_SETLE	0x0E
#define BTC_JE		0x0F
#define BTC_JNE		0x10
#define BTC_JG		0x11
#define BTC_JGE		0x12
#define BTC_JL		0x13
#define BTC_JLE		0x14

struct bitcode
{
	int instruction;
	ddString a;
	ddString b;
};

void generate_asm(struct tokenNode* node, ddString* asmBuffer, sizet* lineCount);

void generate_asm_2reg(struct tokenNode* node, const char* opc, ddString* asmBuffer, sizet* lineCount)
{
	ddString_format(&(asmBuffer[*lineCount]), "	push	r15;\n");
	(*lineCount)++;
	ddString_format(&(asmBuffer[*lineCount]), "	%s	r15, r14;\n", opc);
	(*lineCount)++;
	ddString_format(&(asmBuffer[*lineCount]), "	pop	r15;\n");
	(*lineCount)++;
	ddString_format(&(asmBuffer[*lineCount]), "	pop	r14;\n");
	(*lineCount)++;
	generate_asm(node->right, asmBuffer, lineCount);
	generate_asm(node->left, asmBuffer, lineCount);
	return;
}
void generate_asm_1reg(struct tokenNode* node, const char* opc, ddString* asmBuffer, sizet* lineCount)
{
	ddString_format(&(asmBuffer[*lineCount]), "	push	rax;\n");
	(*lineCount)++;
	ddString_format(&(asmBuffer[*lineCount]), "	%s	r15;\n", opc);
	(*lineCount)++;
	ddString_format(&(asmBuffer[*lineCount]), "	pop	r15;\n");
	(*lineCount)++;
	ddString_format(&(asmBuffer[*lineCount]), "	pop	rax;\n");
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

struct function
{
	ddString name;
	sizet retSize;
};

struct functionTracker
{
	sizet size;
	struct function funs[500];
};

struct functionTracker functiont;

static struct stackTracker stackt;

void init_generation(void)
{
	stackt.top = 0;
	stackt.size = 0;
	functiont.size = 0;
}

void generate_add_var(ddString name, int size)
{
	stackt.vars[stackt.top].name = name;
	stackt.vars[stackt.top].size = size;
	stackt.top++;
	stackt.size += size;
	stackt.vars[stackt.top-1].spos = stackt.size;
};

void generate_add_function(ddString name, sizet retSize)
{
	functiont.funs[functiont.size].name = name;
	functiont.funs[functiont.size].retSize = retSize;
	functiont.size++;
}

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
			ddString_format(&fmt, "qword [rbp-%d]", stackt.vars[i].spos);
			ddString_push_back(&(asmBuffer[*lineCount]), fmt);
			raze_ddString(&fmt);
			return;
		}
	}
	ddString_push_back(&(asmBuffer[*lineCount]), name);
}

sizet scope = 0;

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
		if (ddString_compare_cstring(node->value->value, "if"))
		{
			ddString_format(&(asmBuffer[*lineCount]), "	je	SC%d;\n", scope+1);
			(*lineCount)++;
			ddString_format(&(asmBuffer[*lineCount]), "	cmp	r15, 0;\n");
			(*lineCount)++;
			ddString_format(&(asmBuffer[*lineCount]), "	pop	r15;\n");
			(*lineCount)++;
			generate_asm(node->right, asmBuffer, lineCount);
			return;
		}
		if (ddString_compare_cstring(node->value->value, "sub"))
		{
			generate_add_function(node->right->right->right->value->value, ddString_to_int(node->right->right->value->value));
			ddString_format(&(asmBuffer[*lineCount]), "%s:\n", functiont.funs[functiont.size-1].name.cstr);
			(*lineCount)++;
			ddString_format(&(asmBuffer[*lineCount]), "	push rbp;\n");
			(*lineCount)++;
			ddString_format(&(asmBuffer[*lineCount]), "	mov rbp, rsp;\n");
			(*lineCount)++;
			ddString_format(&(asmBuffer[*lineCount]), "	pop rbp;\n");
			(*lineCount)++;
			generate_asm(node->right, asmBuffer, lineCount);
		}
		return;
	}
	switch (node->value->value.cstr[0])
	{
		case ';': return;
		case '{':
		{
			generate_asm(node->right, asmBuffer, lineCount);
			scope++;
			break;
		}
		case '}':
		{
			ddString_format(&(asmBuffer[*lineCount]), "SC%d:\n", scope);
			(*lineCount)++;
			break;
		}
		case '>':
		{
			if (node->value->value.cstr[1] == '=')
			{
				ddString_format(&(asmBuffer[*lineCount]), "	push	r15;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	movzx	r15, al;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	setg	al;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	cmp	r14, r15;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	pop	r14;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	pop	r15;\n");
				(*lineCount)++;
				generate_asm(node->left, asmBuffer, lineCount);
				generate_asm(node->right, asmBuffer, lineCount);
				return;
			}
			else
			{
				ddString_format(&(asmBuffer[*lineCount]), "	push	r15;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	movzx	r15, al;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	setge	al;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	cmp	r14, r15;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	pop	r14;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	pop	r15;\n");
				(*lineCount)++;
				generate_asm(node->left, asmBuffer, lineCount);
				generate_asm(node->right, asmBuffer, lineCount);
				return;
			}
		}
		case '<':
		{
			if (node->value->value.cstr[1] == '=')
			{
				ddString_format(&(asmBuffer[*lineCount]), "	push	r15;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	movzx	r15, al;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	setl	al;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	cmp	r14, r15;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	pop	r14;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	pop	r15;\n");
				(*lineCount)++;
				generate_asm(node->left, asmBuffer, lineCount);
				generate_asm(node->right, asmBuffer, lineCount);
				return;
			}
			else
			{
				ddString_format(&(asmBuffer[*lineCount]), "	push	r15;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	movzx	r15, al;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	setle	al;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	cmp	r14, r15;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	pop	r14;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	pop	r15;\n");
				(*lineCount)++;
				generate_asm(node->left, asmBuffer, lineCount);
				generate_asm(node->right, asmBuffer, lineCount);
				return;
			}
		}
		case '!':
		{
			if (node->value->value.cstr[1] == '=')
			{
				ddString_format(&(asmBuffer[*lineCount]), "	push	r15;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	movzx	r15, al;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	setne	al;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	cmp	r14, r15;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	pop	r14;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	pop	r15;\n");
				(*lineCount)++;
				generate_asm(node->left, asmBuffer, lineCount);
				generate_asm(node->right, asmBuffer, lineCount);
				return;
			}
		}
		case '=':
		{
			if (node->value->value.cstr[1] == '=')
			{
				ddString_format(&(asmBuffer[*lineCount]), "	push	r15;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	movzx	r15, al;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	sete	al;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	cmp	r14, r15;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	pop	r14;\n");
				(*lineCount)++;
				ddString_format(&(asmBuffer[*lineCount]), "	pop	r15;\n");
				(*lineCount)++;
				generate_asm(node->left, asmBuffer, lineCount);
				generate_asm(node->right, asmBuffer, lineCount);
				return;
			}
			if (node->left->value->value.cstr[0] == '@')
			{
				generate_add_var(node->left->right->right->value->value, ddString_to_int(node->left->right->value->value));
				ddString_format(&(asmBuffer[*lineCount]), "	pop	qword[rbp-%d];\n", stackt.vars[stackt.top-1].spos);
				(*lineCount)++;
				generate_asm(node->right, asmBuffer, lineCount);
				return;
			}
			else
			{
				ddString_format(&(asmBuffer[*lineCount]), "	pop	qword[rbp-%d];\n", stackt.vars[stackt.top-1].spos, node->left->value->value.cstr);
				(*lineCount)++;
				generate_asm(node->right, asmBuffer, lineCount);
			}
			break;
		}
		case '@':
		{
			//ddPrintf("	FLKJDSFL:\n");
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
			ddString_push_cstring_back(&(asmBuffer[*lineCount]), "	push	");
			generate_print_var(node->value->value, asmBuffer, lineCount);
			ddString_push_cstring_back(&(asmBuffer[*lineCount]), ";\n");
			(*lineCount)++;
			break;
		}
	}
}

#endif
