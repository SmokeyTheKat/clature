#ifndef __ddScript_parser_h__
#define __ddScript_parser_h__

struct tokenNode;

void parser(struct token* tokens, struct tokenNode* node, sizet min, sizet max, sizet len);
int parser_find_closer(struct token* tokens, sizet len, int start, char obracket, char cbracket);
struct tokenNode** parser_main(struct token* tokens, sizet tokenCount, sizet* _treeCount);
struct tokenNode make_tokenNode(struct tokenNode* parent, struct tokenNode* left, struct tokenNode* right, struct token* value);

extern bool debug;
const sizet charKeysLength = 14;
const char charKeys[] = { '{', '}', '=', '@', '+', '-', '*', '/', '<', '>', '!', '?', '[', '(' };

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

static inline struct tokenNode* parser_split_right(struct token* tokens, struct tokenNode* node, sizet min, sizet max, sizet len, sizet i)
{
	if (i != max)
	{
		struct tokenNode* right = make(struct tokenNode, 1); 
		(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
		parser(tokens, right, i+1, max, len);
		node->right = right;
		return right;
	}
	return nullptr;
}
static inline struct tokenNode* parser_split_left(struct token* tokens, struct tokenNode* node, sizet min, sizet max, sizet len, sizet i)
{
	if (i != min)
	{
		struct tokenNode* left = make(struct tokenNode, 1); 
		(*left) = make_tokenNode(node, nullptr, nullptr, nullptr);
		parser(tokens, left, min, i, len);
		node->left = left;
		return left;
	}
	return nullptr;
}
static inline void parser_bisplit(struct token* tokens, struct tokenNode* node, sizet min, sizet max, sizet len, sizet i)
{
	parser_split_right(tokens, node, min, max, len, i);
	parser_split_left(tokens, node, min, max, len, i);
}

void parser(struct token* tokens, struct tokenNode* node, sizet min, sizet max, sizet len)
{
	for (sizet k = 0; k < charKeysLength; k++)
	{
		for (sizet i = min; i < max; i++)
		{
			if (tokens[i].type == TKN_KEYWORD)
			{
				if (ddString_compare_cstring(tokens[i].value, "sub"))
				{
					for (sizet i = min; i < max; i++)
					{
						node->value = &(tokens[i]);
						struct tokenNode* right = make(struct tokenNode, 1); 
						(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
						node->right = right;
						node = node->right;
					}
					return;
				}
				else if (ddString_compare_cstring(tokens[i].value, "continue"))
				{
					node->value = &(tokens[i]);
					return;
				}
				node->value = &(tokens[i]);
				parser_split_right(tokens, node, min, max, len, i);
				return;
			}
			else if (k == charKeysLength-1 && tokens[i].type == TKN_LITERAL && tokens[i+1].value.cstr[0] == '(')
			{
				tokens[i].type = TKN_FUNCTION;
				while (tokens[i].value.cstr[0] != ')')
				{
					struct tokenNode* right = make(struct tokenNode, 1); 
					(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
					node->right =right ;
					node->value = &(tokens[i]);
					node = right;
					i++;
				}
				parser_split_right(tokens, node, min, max, len, i);
				node->value = &(tokens[i]);
				i++;
				return;
			}
			if (tokens[i].value.cstr[0] == charKeys[k] || tokens[i].value.cstr[1] == charKeys[k])
			{
				if (tokens[i].type == TKN_OPERATOR)
				{
					if (tokens[i].value.cstr[0] == '@' && i+2 < len && tokens[i+2].value.cstr[0] == '[')
					{
						struct tokenNode* right = make(struct tokenNode, 1); 
						(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
						node->right = right;
						node->value = &(tokens[i]);
						node = right;
						i++;
						right = make(struct tokenNode, 1); 
						(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
						node->right = right;
						node->value = &(tokens[i]);
						node = right;
						i++;
						right = make(struct tokenNode, 1); 
						(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
						node->right = right;
						node->value = &(tokens[i]);
						node = right;
						i++;
						int ndb = parser_find_closer(tokens, len, i, '[', ']');
						parser(tokens, node, i, ndb, len);
						return;
					}
					else if (tokens[i].value.cstr[0] == '?')
					{
						parser_split_right(tokens, node, min, max, len, i);
						node->value = &(tokens[i]);
						return;
					}
				}
				if (tokens[i].type == TKN_SYNTAX)
				{
					if (tokens[i].value.cstr[0] == '(')
					{
						int ndb = parser_find_closer(tokens, len, i, '(', ')');
						parser(tokens, node, i+1, ndb, len);
						return;
					}
				}
				parser_bisplit(tokens, node, min, max, len, i);
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
	sizet i = min;
	if (i == max) return;
	parser_bisplit(tokens, node, min, max, len, i);
	node->value = &(tokens[i]);
	return;
}

struct tokenNode** parser_main(struct token* tokens, sizet tokenCount, sizet* _treeCount)
{
	struct tokenNode** trees = make(struct tokenNode*, 1000000);
	sizet treeCount = 0;

	sizet commandPos = 0;
	sizet nextCommandPos = tokens_get_next_command(tokens, commandPos, tokenCount);
	while (commandPos < tokenCount)
	{
		if (debug) printf("c: %lld    nc: %lld\n", commandPos, nextCommandPos);
		struct tokenNode* commandHead = make(struct tokenNode, 1);
		(*commandHead) = make_tokenNode(nullptr, nullptr, nullptr, nullptr);
		parser(tokens, commandHead, commandPos, nextCommandPos, tokenCount);
		trees[treeCount++] = commandHead;
		commandPos = nextCommandPos+1;
		nextCommandPos = tokens_get_next_command(tokens, commandPos, tokenCount);
	}
	(*_treeCount) = treeCount;
	return trees;
}

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
/*

void parser(struct token* tokens, struct tokenNode* node, sizet min, sizet max, sizet len)
{
	for (int k = 0; k < 11; k++)
	{
		bool keyFound = false;
		for (int i = min; i < max; i++)
		{
			if (tokens[i].type == TKN_KEYWORD)
			{
				if (ddString_compare_cstring(tokens[i].value, "sub"))
				{
					for (sizet i = min; i < max; i++)
					{
						node->value = &(tokens[i]);
						struct tokenNode* right = make(struct tokenNode, 1); 
						(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
						node->right = right;
						node = node->right;
					}
					return;
				}
				node->value = &(tokens[i]);
				struct tokenNode* right = make(struct tokenNode, 1); 
				(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
				parser(tokens, right, i+1, max, len);
				node->right = right;
				return;
			}
			else if (k == 10 && tokens[i].type == TKN_LITERAL && tokens[i+1].value.cstr[0] == '(')
			{
				tokens[i].type = TKN_FUNCTION;
				while (tokens[i].value.cstr[0] != ')')
				{
					struct tokenNode* right = make(struct tokenNode, 1); 
					(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
					node->right =right ;
					node->value = &(tokens[i]);
					node = right;
					i++;
				}
				struct tokenNode* right = make(struct tokenNode, 1); 
				(*right) = make_tokenNode(node, nullptr, nullptr, nullptr);
				node->right =right ;
				node->value = &(tokens[i]);
				node = right;
				i++;


				return;
			}
			if (tokens[i].value.cstr[0] == charKeys[k] || tokens[i].value.cstr[1] == charKeys[k])
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
*/

#endif
