#include <iostream>
#include "lexer.hpp"
//#include "parser.hpp"
#include "recursive_parser.hpp"

using namespace std;



int main()
{
	Recursive_Parser parser;
	parser.run();

	cin.get();
	return 0;
}