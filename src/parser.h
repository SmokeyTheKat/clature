#ifndef __ddScript_parser_h__
#define __ddScript_parser_h__

struct tokenNode
{
	struct tokenNode* parent;
	struct tokenNode* left;
	struct tokenNode* right;
	struct token* value;
};

struct tokenNode make_tokenNode(struct tokenNode* parent, struct tokenNode* left, struct tokenNode* right, struct token* value)
{
	struct tokenNode output;
	output.parent = parent;
	output.left = left;
	output.right = right;
	output.value = value;
	return output;
}

const char charKeys[] = {
	'{', '=', '@', '+', '-', '*', '/', '('
};

int parser_find_closer(struct token* tokens, sizet len, int start, char obracket, char cbracket)
{
	int i = start+1;
	int layer = 1;
	for (; layer != 0; i++)
	{
		if (tokens[i].value.cstr[0] == obracket)
		{
			layer++;
		}
		if (tokens[i].value.cstr[0] == cbracket)
		{
			layer--;
		}
	}
	return i-1;
}

struct tokenTree
{
	struct tokenNode* head;
};

void parser(struct token* tokens, struct tokenNode* node, sizet min, sizet max, sizet len)
{
	for (int k = 0; k < 8; k++)
	{
		bool keyFound = false;
		for (int i = min; i < max; i++)
		{
			if (tokens[i].value.cstr[0] == charKeys[k])
			{
				if (tokens[i].type == TKN_SYNTAX)
				{
					if (tokens[i].value.cstr[0] == '(')
					{
						int ndb = parser_find_closer(tokens, len, i, '(', ')');
						parser(tokens, node, i+1, ndb, len);
						return;
					}
				}
				//ddPrintf("min:%d   max:%d    i:%d    tkv:%s\n", min, max, i, tokens[i].value.cstr);
				if (i != min)
				{
					struct tokenNode* left = make(struct tokenNode, 1); 
					(*left) = make_tokenNode(node, nullptr, nullptr, nullptr);
					parser(tokens, left, min, i, len);
					node->left = left;
				}

				if (i != max)
				{
					struct tokenNode* right = make(struct tokenNode, 1); 
					(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
					parser(tokens, right, i+1, max, len);
					node->right = right;
				}

				node->value = &(tokens[i]);
				return;
			}
			if (tokens[i].type == TKN_SYNTAX)
			{
				if (tokens[i].value.cstr[0] == '(')
				{
					int ndb = parser_find_closer(tokens, max, i, '(', ')');
					i = ndb;
					continue;
				}
			}
		}
	}
	int i = min;
	if (i == max) return;
	//ddPrintf("min:%d   max:%d    i:%d    tkv:%s\n", min, max, i, tokens[i].value.cstr);
	if (i != min)
	{
		struct tokenNode* left = make(struct tokenNode, 1); 
		(*left) = make_tokenNode(node, nullptr, nullptr, nullptr);
		parser(tokens, left, min, i, len);
		node->left = left;
	}

	if (i != max)
	{
		struct tokenNode* right = make(struct tokenNode, 1); 
		(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
		parser(tokens, right, i+1, max, len);
		node->right = right;
	}

	node->value = &(tokens[i]);
	return;
}

#endif
