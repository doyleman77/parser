#pragma once
#include <fstream>
#include "token.hpp"

class Lexer
{
private:
	int current_state;
	int previous_state;
	int language[13][127];
	ifstream fin;
	string line;

	unsigned int line_number;
	unsigned int col;
	string check_keywords(string&);
public:
	Lexer();
	~Lexer();

	Token get_token();
};