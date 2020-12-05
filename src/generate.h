#ifndef __ddScript_generate_h__
#define __ddScript_generate_h__

void generate_asm(struct tokenNode* node);

void generate_asm_2reg(struct tokenNode* node, const char* opc)
{
	ddPrintf("	push r15;\n");
	ddPrintf("	%s r15, r14;\n", opc);
	ddPrintf("	pop r15;\n");
	ddPrintf("	pop r14;\n");
	generate_asm(node->right);
	generate_asm(node->left);
	return;
}
void generate_asm_1reg(struct tokenNode* node, const char* opc)
{
	ddPrintf("	push r15;\n");
	ddPrintf("	%s r15;\n", opc);
	ddPrintf("	pop r15;\n");
	ddPrintf("	pop rax;\n");
	generate_asm(node->right);
	generate_asm(node->left);
	return;
}

struct bitCode
{
	int* v;
};

void generate_asm(struct tokenNode* node)
{
	if (node == nullptr) return;
	switch (node->value->value.cstr[0])
	{
		case ';': return;
		case '{':
		{
			generate_asm(node->right);
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
				ddPrintf("	pop %s;\n", node->left->right->right->value->value.cstr);
				generate_asm(node->right);
				return;
			}
			else
			{
				ddPrintf("	pop %s;\n", node->left->value->value.cstr);
				generate_asm(node->right);
			}
			break;
		}
		case '*':
			generate_asm_1reg(node, "mul");
			return;
		case '/':
			generate_asm_1reg(node, "div");
			return;
		case '+':
			generate_asm_2reg(node, "add");
			return;
		case '-':
			generate_asm_2reg(node, "sub");
			return;
		default:
		{
			ddPrintf("	push %s;\n", node->value->value.cstr);
			break;
		}
	}
}

#endif
