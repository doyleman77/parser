#include "parser.hpp"

Parser::Parser()
{
	fin.open("../Debug/PARSER_PRODUCTIONS.txt");

	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			fin >> checking_table[i][j];
	fin.close();
	fin.open("../Debug/PRODUCTION_RELATIONS.txt");
	while (fin.eof() == false)
	{
		string key;
		string value;
		fin >> value;
		fin >> key;
		token_to_prod[key] = value;
	}
	//init the stack with a $
	token_productions.push("$");
	//grabbing tokens
	Token my_token = lexer.get_token();
	token_queue.push(my_token);

	while (my_token.token_type != -1)
	{
		string top_of_stack = token_productions.top();
		int stack_index = stoi(token_to_prod[top_of_stack]);

		//grab another token, store it, and parse
		my_token = lexer.get_token();
		token_queue.push(my_token);
	}

};

Parser::~Parser()
{

};
