#pragma once

#include <string>
using namespace std;

class Token
{
public:
	Token();
	string token_value;
	string token_type;
	Token(string const t_value, unsigned short t_type);
};