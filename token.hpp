#pragma once

#include <string>
using namespace std;

class Token
{
public:
	Token();
	string token_value;
	string token_type;
	Token(string const t_value, string const t_type);
};