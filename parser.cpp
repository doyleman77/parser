#include <sstream>

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
	fin.close();
	fin.open("../Debug/PRODUCTIONS.txt");
	while (fin.eof() == false)
	{
		string key;
		string value;
		fin >> key;
		fin >> value;
		prod_reduction[key] = value;
	}

	//init the stack with a $
	token_productions.push("15");

	//grabbing tokens
	Token my_token = lexer.get_token();
	string token_to_push;

	token_queue.push(my_token);
	
	token_to_push += my_token.token_type;

	string token_placement_type;

	//main reduction algorithm	
	while (my_token.token_type != "-1")
	{
		bool done = false;
		while (!done)
		{
			string top_of_stack = token_productions.top();
			//int ttp_index = stoi(top_of_stack);
			//int stack_index = stoi(token_to_prod[top_of_stack]);
			int stack_index = stoi(top_of_stack);
			int incoming_index = stoi(token_to_push);

			token_placement_type = checking_table[stack_index - 1][incoming_index - 1];

			if (token_placement_type == "=")
			{
				token_productions.push(token_to_push);
				done = true;
				continue;
			}
			else if (token_placement_type == "<") //place with a reduction token
			{
				token_productions.push(token_placement_type);
				token_productions.push(token_to_push);
				done = true;
				continue;
			}
			else if (token_placement_type == ">") //reduce
			{
				string reduction;
				string tok;
				//while (!done) // constantly try to place token_to_push;
				{
					while (tok != "<") //grab reduction until <
					{
						tok = token_productions.top();
						token_productions.pop();
						if (tok != "<")
							reduction += tok;
					}
					string new_production = prod_reduction[reduction];
					//see how to place the new production on
					top_of_stack = token_productions.top();
					stack_index = stoi(top_of_stack);
					incoming_index = stoi(new_production);

					token_placement_type = checking_table[stack_index - 1][incoming_index - 1];

					//don't care about adding =, only about adding <. then we always push new_production value
					if (token_placement_type == "<")
						token_productions.push(token_placement_type);
					token_productions.push(new_production);
				}
			}	// we need to look this all until token_to_push is finally added as a < or a = (set done to true on such a thing)
		}
		//grab another token, store it, and parse
		my_token = lexer.get_token();
		token_queue.push(my_token);

		token_to_push = my_token.token_type;
	}

};

Parser::~Parser()
{

};
