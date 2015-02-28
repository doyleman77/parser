#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "lexer.hpp"
#include "token.hpp"
using namespace std;

Lexer::Lexer()
{
	line_number = 0;
	col = 0;
	current_state = previous_state = 0;

	fin.open("../Debug/DFA_LANGUAGEv2.txt");
	if (fin.is_open() == false)
		cout << "error loading language, check to see if DFA_LANGUAGE.txt is present..." << endl;

	for (int i = 0; i < 127; i++)
	for (int j = 0; j < 13; j++)
		fin >> language[j][i];


	fin.close();

	//fin.open("../Debug/sample_expression.txt");
	fin.open("../Debug/sample_pascal_array.txt");
	if (fin.is_open() == false)
		cout << "error loading pascal code, check to see if sample_pascal.txt is present..." << endl;
	getline(fin, line);

};

Lexer::~Lexer(){};

string Lexer::check_keywords(string& token)
{
	if (token == "+") return "09";
	else if (token == "-") return "10";
	else if (token == "*") return "11";
	else if (token == "/") return "12";
	else if (token == "(") return "13";
	else if (token == ")") return "14";
	else if (token == "begin") return "16";
	else if (token == "end") return "17";
	else if (token == ".") return "18";
	else if (token == "program") return "19";
	else if (token == "var") return "20";
	else if (token == "if") return "21";
	else if (token == "then") return "22";
	else if (token == "else") return "23";
	else if (token == ";") return "24";
	else if (token == "=") return "25";
	else if (token == "procedure") return "26";
	else if (token == "function") return "27";
	else if (token == "..") return "28";
	else return "03"; // identifier is default.
};

Token Lexer::get_token()
{
	// initialize the reading information
	// will need to re-init only at eof when we make parser

	current_state = previous_state = 0;

	// read until the end of pascal program
	while ((!fin.eof()) || (col < line.size()))
	{
		string token;

		if (col >= line.size())
		{
			getline(fin, line);
			col = 0;
			line_number++;
		}

		char token_eval = ' ';

			for (int i = col; i <= line.size(); i++, col++)
			{
				if (i < line.size())
					token_eval = line.at(i);
				
				if (token_eval == '\t' || i >= line.size())
					token_eval = ' ';

				if (line.size() == 0)
				{
					cout << "BLANK LINE" << endl;
					continue;
				}

				int temp_state = previous_state; // used for dang arrays.

				previous_state = current_state;
				current_state = language[current_state][(int)token_eval];

				/***********************************************
				* Psuedo Patch: Giving '.' at the EOF() a word
				* value!
				***********************************************
				if (current_state == 4 && col+1 >= line.size())
				{
					current_state = 55;
					previous_state = 1;
					token.push_back(token_eval);
				}
				***********************************************/
				// Found an accept state

				/**********************************************
				* We need to consider if the #. isn't a #..#
				* for the sake of arrays, which use the syntax
				* [1..5], for example.
				* Key thing to check for is a '..' pair.
				**********************************************/
				// if we're half-way into making a float, or an array range
				if (previous_state == 2)
				{
					if (token_eval == '.') // we need to see if this is a float, or an array index list
					{
						char token_peek = line.at(i + 1); // peek for another .
						if (token_peek == '.')
						{
							Token new_token("", "");
							new_token.token_value = token;
							new_token.token_type = "01";
							//col--; // we need to re-evaluate the '..'
							return new_token;
						}
					}
				}
				// if we have a '.', and are looking for another '.' for array index list
				if (token_eval == '.')
				{
					char token_peek = NULL;
					if (i < line.size()-2)
						token_peek = line.at(i + 1); // peek for another .
					if (token_peek == '.')
					{
						Token new_token("", "");
						new_token.token_value = "..";
						new_token.token_type = "28";
						col+=2; // we need to skip the 2nd '.'
						return new_token;
					}
				}
				
				if (current_state == 55 || col == line.size())
				{

					Token new_token("","");
					switch (previous_state)
					{
					case 0: //NOTHING
						current_state = previous_state = 0;
						//Spaces
						token.clear();
						// Do nothing?
						continue;
						break;
					case 1: // WORD
						current_state = previous_state = 0;
						cout << "WORD:\t" << token << endl;
						new_token.token_value = token;
						new_token.token_type = check_keywords(token);
						return new_token;
						break;
					case 2: // INT
						current_state = previous_state = 0;
						cout << "INT:\t" << token << endl;
						new_token.token_type = "01";
						new_token.token_value = token;
						return new_token;
						break;
					case 3: // SPECIAL
						current_state = previous_state = 0;
						cout << "SPEC:\t" << token << endl;
						new_token.token_type = check_keywords(token);
						new_token.token_value = token;
						return new_token;
						break;
					case 4: // '.'
						cout << "'.'" << endl;
						new_token.token_type = check_keywords(token);
						new_token.token_value = token;
						return new_token;
						break;
					case 5: // REAL
						current_state = previous_state = 0;
						cout << "REAL:\t" << token << endl;
						new_token.token_type = "02";
						new_token.token_value = token;
						return new_token;
						break;
					case 10: // REAL
						current_state = previous_state = 0;
						cout << "REAL:\t" << token << endl;
						new_token.token_type = "02";
						new_token.token_value = token;
						return new_token;
						break;
					case 11: // SPECIAL ':'
						current_state = previous_state = 0;
						cout << "SPEC:\t:" << endl;
						new_token.token_value = token;
						new_token.token_type = "111";
						return new_token;
						break;
					case 12: // SPECIAL ':='
						current_state = previous_state = 0;
						cout << "SPEC:\t:=" << endl;
						new_token.token_value = token;
						new_token.token_type = "112";
						return new_token;
						break;
					default:
						break;
					}
				}

				// Dead state
				else if (current_state == 99)
				{
					cout << "Error with token <<" << token
						<< ">>" << "at line: " << line_number << ", column: " << col+1
						<< "." << endl;
					Token dead("$", "15");
					return dead;
				}
				else
				{
					token.push_back(token_eval);
				}
			}
		}
		line_number++;
		col = 0;
	//fin.close();
	Token new_token("$", "15");
	return new_token;
};