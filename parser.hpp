#pragma once

#include <fstream>
#include <map>
#include <stack>
#include <queue>

#include "lexer.hpp"

class Parser
{
private:
	Lexer lexer;
	stack<string> token_productions;
	queue<Token> token_queue;

	ifstream fin;

	map<string, string> prod_reduction;
	map<string, string> token_to_prod;
	char checking_table[15][15];
public:
	Parser();
	~Parser();
	void parse();
};