#include <iostream>
#include "recursive_parser.hpp"


Recursive_Parser::Recursive_Parser()
{
	current_scope = "";
	variable_size_multiplier = 1;
};

Recursive_Parser::~Recursive_Parser()
{

};

void Recursive_Parser::run()
{
	token_eval = lexer.get_token();
	program_production();
	cout << "Good code!" << endl;
	cin.get();
};

void Recursive_Parser::expr()
{
	term();
};

void Recursive_Parser::term()
{
	factor();
	term_prime();
};

void Recursive_Parser::term_prime()
{
	// if we add or subtract
	if (token_eval.token_type == "09" ||
		token_eval.token_type == "10")
	{
		token_eval = lexer.get_token();
		term();
		term_prime();
	}
};

void Recursive_Parser::factor()
{
	// token is a int
	if (token_eval.token_type == "01")
	{
		token_eval = lexer.get_token();
		factor_prime();
	}
	// token is a real
	else if (token_eval.token_type == "02")
	{
		token_eval = lexer.get_token();
		factor_prime();
	}
	// token is a identifier
	else if (token_eval.token_type == "03")
	{
		if (find_identifier() == true)
		{
			token_eval = lexer.get_token();
			factor_prime();
		}
		else
		{
			cout << "Error! Identifier " << token_eval.token_value << " does not exist!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	// token is a ( <expr> )
	else if (token_eval.token_type == "13")
	{
		token_eval = lexer.get_token();
		expr();
		if (token_eval.token_type == "14")
		{
			token_eval = lexer.get_token();
			factor_prime();
		}
		else
		{
			std::cout << std::endl << "Error with token " <<
				token_eval.token_value << " at factor()" << std::endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	// if defining a signed/unsigned type
	else if (token_eval.token_type == "09" ||
		token_eval.token_type == "10")
	{
		token_eval = lexer.get_token();
		factor();
	}
	else
	{
		std::cout << std::endl << "Error with token " <<
			token_eval.token_value << " at factor()" << std::endl;
		cin.get();
		exit(EXIT_FAILURE);
	}
};

void Recursive_Parser::factor_prime()
{
	// if we're multiplying or dividing
	if (token_eval.token_type == "11" ||
		token_eval.token_type == "12")
	{
		token_eval = lexer.get_token();
		factor();
		factor_prime();
	}
};

void Recursive_Parser::bool_expr()
{
	expr();
	if (token_eval.token_value == "=" || token_eval.token_value == "<"
		|| token_eval.token_value == ">")
	{
		token_eval = lexer.get_token();
		expr();
	}
	else
	{
		cout << "Error! Missing '=' in boolean expression!" << endl;
		cin.get();
		exit(EXIT_FAILURE);
	}
};

void Recursive_Parser::multiple_statement()
{
	if (token_eval.token_value == ";")
	{
		token_eval = lexer.get_token();
		statement();
		multiple_statement();
	}
};

void Recursive_Parser::statement()
{
	// token is variable
	if (token_eval.token_type == "03")
	{

		if (find_identifier() == true)
		{
			token_eval = lexer.get_token();
			factor_prime();
		}
		else
		{
			cout << "Error! Identifier " << token_eval.token_value << " does not exist!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
		if (token_eval.token_value == ":=")
		{
			token_eval = lexer.get_token();
			expr();
		}
		else
		{
			cout << "Error, missing := before variable!" << endl;
			cin.get();
			exit(EXIT_SUCCESS);
		}
	}
	else if (token_eval.token_value == "if")
	{
		token_eval = lexer.get_token();
		bool_expr();
		if (token_eval.token_value == "then")
		{
			token_eval = lexer.get_token();
			statement();
			if (token_eval.token_value == "else")
			{
				token_eval = lexer.get_token();
				statement();
			}
		}
		else
		{
			cout << "Error, missing then in the if statement!" << endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	else if (token_eval.token_value == "begin")
	{
		token_eval = lexer.get_token();
		statement();
		multiple_statement();
		if (token_eval.token_value == "end")
		{
			token_eval = lexer.get_token();
		}
	}

};

void Recursive_Parser::variable_list()
{
	if (token_eval.token_value == ",")
	{
		token_eval = lexer.get_token();
		if (token_eval.token_type == "03")
		{
			//capture the variable name
			variable_list_holding.push(token_eval.token_value);

			token_eval = lexer.get_token();
			variable_list();
		}
		else
		{
			cout << "Error: Expecting identifier!" << endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	// epsilon
};

void Recursive_Parser::array_production()
{
	//clear old indices
	temp_array_indices.clear();

	if (token_eval.token_value == "[")
	{
		// initialize array multiplier
		variable_size_multiplier = 1;
		token_eval = lexer.get_token();
		// first dimension
		array_dimension_production();
		// test for 2nd dimension
		if (token_eval.token_value == ",")
		{
			token_eval = lexer.get_token();
			array_dimension_production();
			// test for 3rd dimension
			if (token_eval.token_value == ",")
			{
				token_eval = lexer.get_token();
				array_dimension_production();
			}
		}
		if (token_eval.token_value == "]") // [# .. # ]
		{
			token_eval = lexer.get_token();
			if (token_eval.token_value == "of") // [# .. #] of
			{
				token_eval = lexer.get_token();
				inserting_array = true;
				variable_simple_datatype();			// [# .. #] of int/bool
				inserting_array = false;
				variable_size_multiplier = 1;

			}
			else
			{
				cout << "Error! Missing 'of' for declaring array datatype!"
				<< endl;
				cin.get();
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "Error! Missing ']' at end of array range!"
			<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		cout << "Error! Missing '[' for array range!"
			<< endl;
		cin.get();
		exit(EXIT_FAILURE);
	}
};

void Recursive_Parser::array_dimension_production()
{
	// first dimension
	if (token_eval.token_type == "01") // [#
	{
		int range_begin = stoi(token_eval.token_value);

		token_eval = lexer.get_token();
		if (token_eval.token_value == "..") // [#..
		{
			token_eval = lexer.get_token();
			if (token_eval.token_type == "01") // [# .. #
			{
				int range_end = stoi(token_eval.token_value);
				variable_size_multiplier = ((range_end - range_begin + 1) * variable_size_multiplier);
				temp_array_indices.push_back(make_pair(range_begin, range_end));
				token_eval = lexer.get_token();
			}
			else
			{
				cout << "Error! Missing integer at end of array range!"
					<< endl;
				cin.get();
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "Error! Missing '..' in array range!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		cout << "Error! Missing integer at start of array range!"
			<< endl;
		cin.get();
		exit(EXIT_FAILURE);
	}
};

void Recursive_Parser::variable_simple_datatype()
{
	if (token_eval.token_value == "integer")
	{
		// we need to add the variable_list to the symbol table, with proper offsets and sizes

		// if we are global
		if (current_scope.empty() == true)
		{
			while (variable_list_holding.empty() == false)
			{
				// if the variable doesn't exist already 
				if (symbol_table.count(variable_list_holding.front()) == 0)
				{
					symbol_table[variable_list_holding.front()].offset = global_scope_offset;
					// properly calculate these offsets and sizes for arrays
					if (inserting_array == true)
					{
						global_scope_offset += (4 * variable_size_multiplier);
						symbol_table[variable_list_holding.front()].size = (4 * variable_size_multiplier);
						// add indices
						for (int i = 0; i < temp_array_indices.size(); i++)
						{
							symbol_table[variable_list_holding.front()].array_indices.push_back(temp_array_indices.at(i));
						}
					}
					else
					{
						global_scope_offset += 4;
						symbol_table[variable_list_holding.front()].size = 4;
					}
					
					
					symbol_table[variable_list_holding.front()].type = "integer";

					
					variable_list_holding.pop();

				}
				else
				{
					cout << "Error! Identifier " << variable_list_holding.front() << " already exists in this scope!"
						<< endl;
					cin.get();
					exit(EXIT_FAILURE);
				}
			}
		}
		/**********************************************
		* else we are to some local procedure/function
		**********************************************/
		else
		{
			// if we're giving a function a datatype
			if (local_variable_type == 'f')
			{
				symbol_table[current_scope].size = 4;
				symbol_table[current_scope].type = "integer";
				//return;
			}
			// we are in Parameters
			else if (local_variable_type == 'p')
			{
				while (variable_list_holding.empty() == false)
				{
					// if the variable doesn't exist already 
					if (symbol_table[current_scope].parameters.count(variable_list_holding.front()) == 0)
					{
						symbol_table[current_scope].parameters[variable_list_holding.front()].offset = current_scope_offset;
						symbol_table[current_scope].parameters[variable_list_holding.front()].size = 4;
						symbol_table[current_scope].parameters[variable_list_holding.front()].type = "integer";

						current_scope_offset += 4;
						variable_list_holding.pop();

					}
					else
					{
						cout << "Error! Identifier " << variable_list_holding.front() << " already exists in this scope!"
							<< endl;
						cin.get();
						exit(EXIT_FAILURE);
					}
				}
			}
			// We are in Locals
			else if (local_variable_type == 'l')
			{
				while (variable_list_holding.empty() == false)
				{
					// if the variable doesn't exist already in both parameters AND newly made local
					if (symbol_table[current_scope].local_variables.count(variable_list_holding.front()) == 0
						&& symbol_table[current_scope].parameters.count(variable_list_holding.front()) == 0)
					{
						symbol_table[current_scope].local_variables[variable_list_holding.front()].offset = current_scope_offset;
						if (inserting_array == true)
						{
							current_scope_offset += (4 * variable_size_multiplier);
							symbol_table[current_scope].local_variables[variable_list_holding.front()].size = (4 * variable_size_multiplier);
							// add indices
							for (int i = 0; i < temp_array_indices.size(); i++)
							{
								symbol_table[current_scope].local_variables[variable_list_holding.front()].array_indices.push_back(temp_array_indices.at(i));
							}
						}
						else
						{
							current_scope_offset += 4;
							symbol_table[current_scope].local_variables[variable_list_holding.front()].size = 4;
						}

						symbol_table[current_scope].local_variables[variable_list_holding.front()].type = "integer";
						variable_list_holding.pop();

					}
					else
					{
						cout << "Error! Identifier " << variable_list_holding.front() << " already exists in this scope!"
							<< endl;
						cin.get();
						exit(EXIT_FAILURE);
					}
				}
			}
		}
		token_eval = lexer.get_token();
		return;
	}
	/**********************************
	* BOOLEAN
	**********************************/
	else if (token_eval.token_value == "boolean")
	{
		// if we are global
		if (current_scope.empty() == true)
		{
			while (variable_list_holding.empty() == false)
			{
				// if the variable doesn't exist already 
				if (symbol_table.count(variable_list_holding.front()) == 0)
				{
					symbol_table[variable_list_holding.front()].offset = global_scope_offset;
					// properly calculate these offsets and sizes for arrays
					if (inserting_array == true)
					{
						global_scope_offset += (1 * variable_size_multiplier);
						symbol_table[variable_list_holding.front()].size = (1 * variable_size_multiplier);
						// add indices
						for (int i = 0; i < temp_array_indices.size(); i++)
						{
							symbol_table[variable_list_holding.front()].array_indices.push_back(temp_array_indices.at(i));
						}
					}
					else
					{
						global_scope_offset += 1;
						symbol_table[variable_list_holding.front()].size = 1;
					}


					symbol_table[variable_list_holding.front()].type = "boolean";


					variable_list_holding.pop();

				}
				else
				{
					cout << "Error! Identifier " << variable_list_holding.front() << " already exists in this scope!"
						<< endl;
					cin.get();
					exit(EXIT_FAILURE);
				}
			}
		}
		/**********************************************
		* else we are to some local procedure/function
		**********************************************/
		else
		{
			// if we're giving a function a datatype
			if (local_variable_type == 'f')
			{
				symbol_table[current_scope].size = 1;
				symbol_table[current_scope].type = "boolean";
				//return;
			}
			// we are in Parameters
			else if (local_variable_type == 'p')
			{
				while (variable_list_holding.empty() == false)
				{
					// if the variable doesn't exist already 
					if (symbol_table[current_scope].parameters.count(variable_list_holding.front()) == 0)
					{
						symbol_table[current_scope].parameters[variable_list_holding.front()].offset = current_scope_offset;
						symbol_table[current_scope].parameters[variable_list_holding.front()].size = 1;
						symbol_table[current_scope].parameters[variable_list_holding.front()].type = "boolean";

						current_scope_offset += 1;
						variable_list_holding.pop();
					}
					else
					{
						cout << "Error! Identifier " << variable_list_holding.front() << " already exists in this scope!"
							<< endl;
						cin.get();
						exit(EXIT_FAILURE);
					}
				}
			}
			// We are in Locals
			else if (local_variable_type == 'l')
			{
				while (variable_list_holding.empty() == false)
				{
					// if the variable doesn't exist already in both parameters AND newly made local
					if (symbol_table[current_scope].local_variables.count(variable_list_holding.front()) == 0
						&& symbol_table[current_scope].parameters.count(variable_list_holding.front()) == 0)
					{
						symbol_table[current_scope].local_variables[variable_list_holding.front()].offset = current_scope_offset;
						// properly calculate these offsets and sizes for arrays
						if (inserting_array == true)
						{
							current_scope_offset += (1 * variable_size_multiplier);
							symbol_table[current_scope].local_variables[variable_list_holding.front()].size = (1 * variable_size_multiplier);
							// add indices
							for (int i = 0; i < temp_array_indices.size(); i++)
							{
								symbol_table[variable_list_holding.front()].array_indices.push_back(temp_array_indices.at(i));
							}
						}
						else
						{
							current_scope_offset += 1;
							symbol_table[current_scope].local_variables[variable_list_holding.front()].size = 1;
						}


						symbol_table[variable_list_holding.front()].type = "boolean";


						variable_list_holding.pop();

					}
					else
					{
						cout << "Error! Identifier " << variable_list_holding.front() << " already exists in this scope!"
							<< endl;
						cin.get();
						exit(EXIT_FAILURE);
					}
				}
			}
		}
		token_eval = lexer.get_token();
		return;
	}
	else
	{
		cout << "Error! Expecting datatype integer or boolean, recieved " <<
			token_eval.token_value << " instead!" << endl;
		cin.get();
		exit(EXIT_FAILURE);
	}
};

void Recursive_Parser::variable_datatype()
{
	if (token_eval.token_value == "integer")
	{
		// we need to add the variable_list to the symbol table, with proper offsets and sizes
		
		// if we are global
		if (current_scope.empty() == true)
		{
			while (variable_list_holding.empty() == false)
			{
				// if the variable doesn't exist already 
				if (symbol_table.count(variable_list_holding.front()) == 0 )
				{
					symbol_table[variable_list_holding.front()].offset = global_scope_offset;
					symbol_table[variable_list_holding.front()].size = 4;
					symbol_table[variable_list_holding.front()].type = "integer";
					
					global_scope_offset += 4;
					variable_list_holding.pop();

				}
				else
				{
					cout << "Error! Identifier " << variable_list_holding.front() << " already exists in this scope!"
						<< endl;
					cin.get();
					exit(EXIT_FAILURE);
				}
			}
		}
		/**********************************************
		* else we are to some local procedure/function
		**********************************************/
		else
		{
			// we are in Parameters
			if (local_variable_type == 'p')
			{
				while (variable_list_holding.empty() == false)
				{
					// if the variable doesn't exist already 
					if (symbol_table[current_scope].parameters.count(variable_list_holding.front()) == 0)
					{
						symbol_table[current_scope].parameters[variable_list_holding.front()].offset = current_scope_offset;
						symbol_table[current_scope].parameters[variable_list_holding.front()].size = 4;
						symbol_table[current_scope].parameters[variable_list_holding.front()].type = "integer";
						if (passing_by_reference == true)
							symbol_table[current_scope].parameters[variable_list_holding.front()].referred = true;
						else
							symbol_table[current_scope].parameters[variable_list_holding.front()].referred = false;
						current_scope_offset += 4;
						variable_list_holding.pop();

					}
					else
					{
						cout << "Error! Identifier " << variable_list_holding.front() << " already exists in this scope!"
							<< endl;
						cin.get();
						exit(EXIT_FAILURE);
					}
				}
			}
			// We are in Locals
			if (local_variable_type == 'l')
			{
				while (variable_list_holding.empty() == false)
				{
					// if the variable doesn't exist already in both parameters AND newly made local
					if (symbol_table[current_scope].local_variables.count(variable_list_holding.front()) == 0
						&& symbol_table[current_scope].parameters.count(variable_list_holding.front()) == 0)
					{
						symbol_table[current_scope].local_variables[variable_list_holding.front()].offset = current_scope_offset;
						symbol_table[current_scope].local_variables[variable_list_holding.front()].size = 4;
						symbol_table[current_scope].local_variables[variable_list_holding.front()].type = "integer";

						current_scope_offset += 4;
						variable_list_holding.pop();

					}
					else
					{
						cout << "Error! Identifier " << variable_list_holding.front() << " already exists in this scope!"
							<< endl;
						cin.get();
						exit(EXIT_FAILURE);
					}
				}
			}
		}
		token_eval = lexer.get_token();
		return;
	}
	/**********************************
	* BOOLEAN
	**********************************/
	else if (token_eval.token_value == "boolean")
	{
		// if we are global
		if (current_scope.empty() == true)
		{
			while (variable_list_holding.empty() == false)
			{
				// if the variable doesn't exist already 
				if (symbol_table.count(variable_list_holding.front()) == 0)
				{
					symbol_table[variable_list_holding.front()].offset = global_scope_offset;
					symbol_table[variable_list_holding.front()].size = 1;
					symbol_table[variable_list_holding.front()].type = "boolean";

					global_scope_offset += 1;
					variable_list_holding.pop();

				}
				else
				{
					cout << "Error! Identifier " << variable_list_holding.front() << " already exists in this scope!"
						<< endl;
					cin.get();
					exit(EXIT_FAILURE);
				}
			}
		}
		/**********************************************
		* else we are to some local procedure/function
		**********************************************/
		else
		{
			// we are in Parameters
			if (local_variable_type == 'p')
			{
				while (variable_list_holding.empty() == false)
				{
					// if the variable doesn't exist already 
					if (symbol_table[current_scope].parameters.count(variable_list_holding.front()) == 0)
					{
						symbol_table[current_scope].parameters[variable_list_holding.front()].offset = current_scope_offset;
						symbol_table[current_scope].parameters[variable_list_holding.front()].size = 1;
						symbol_table[current_scope].parameters[variable_list_holding.front()].type = "boolean";
						if (passing_by_reference == true)
							symbol_table[current_scope].parameters[variable_list_holding.front()].referred = true;
						else
							symbol_table[current_scope].parameters[variable_list_holding.front()].referred = false;
						current_scope_offset += 1;
						variable_list_holding.pop();
					}
					else
					{
						cout << "Error! Identifier " << variable_list_holding.front() << " already exists in this scope!"
							<< endl;
						cin.get();
						exit(EXIT_FAILURE);
					}
				}
			}
			// We are in Locals
			if (local_variable_type == 'l')
			{
				while (variable_list_holding.empty() == false)
				{
					// if the variable doesn't exist already in both parameters AND newly made local
					if (symbol_table[current_scope].local_variables.count(variable_list_holding.front()) == 0
						&& symbol_table[current_scope].parameters.count(variable_list_holding.front()) == 0)
					{
						symbol_table[current_scope].local_variables[variable_list_holding.front()].offset = current_scope_offset;
						symbol_table[current_scope].local_variables[variable_list_holding.front()].size = 1;
						symbol_table[current_scope].local_variables[variable_list_holding.front()].type = "boolean";
						
						current_scope_offset += 1;
						variable_list_holding.pop();

					}
					else
					{
						cout << "Error! Identifier " << variable_list_holding.front() << " already exists in this scope!"
							<< endl;
						cin.get();
						exit(EXIT_FAILURE);
					}
				}
			}
		}
		token_eval = lexer.get_token();
		return;
	}
	else if (token_eval.token_value == "array")
	{
		token_eval = lexer.get_token();
		array_production();
		return;
	}
	else
	{
		cout << "Error! Expecting datatype integer, boolean, or array; recieved " <<
			token_eval.token_value << " instead!" << endl;
		cin.get();
		exit(EXIT_FAILURE);
	}
};

void Recursive_Parser::variable_production()
{
	//token is a variable
	if (token_eval.token_type == "03")
	{
		// capture token name for symbol_table use
		variable_list_holding.push(token_eval.token_value);

		token_eval = lexer.get_token();
		variable_list();
		if (token_eval.token_value == ":")
		{
			token_eval = lexer.get_token();
			// reset multiplier
			variable_size_multiplier = 1;
			variable_datatype();

			if (token_eval.token_value == ";")
			{
				token_eval = lexer.get_token();
				variable_production_optional();
			}
			else
			{
				cout << "Error! Expected ';' after datatype!"
					<< endl;
				cin.get();
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "Error! ':' token expected after identifier(s)!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		cout << "Error! " << token_eval.token_value << " is not a valid identifier!"
			<< endl;
		cin.get();
		exit(EXIT_FAILURE);
	}

	// clear out the current variable list
	// we should not get here.
	while (variable_list_holding.empty() == false)
		variable_list_holding.pop();
};

// to add additional lists of variables, this is called.
// etc var a,b,c : integer; d, e, f : boolean;
void Recursive_Parser::variable_production_optional()
{
	// var, variable, varlist, :, datatype, ;, variable_production_optional()
	if (token_eval.token_value == "var")
	{
		token_eval = lexer.get_token();
		//token is a variable
		if (token_eval.token_type == "03")
		{
			//store variable name
			variable_list_holding.push(token_eval.token_value);

			token_eval = lexer.get_token();
			variable_list();
			if (token_eval.token_value == ":")
			{
				token_eval = lexer.get_token();
				variable_datatype();
				if (token_eval.token_value == ";")
				{
					token_eval = lexer.get_token();
					variable_production_optional();
				}
				else
				{
					cout << "Error! Expected ';' after datatype!"
						<< endl;
					cin.get();
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				cout << "Error! ':' token expected after identifier(s)!"
					<< endl;
				cin.get();
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "Error! " << token_eval.token_value << " is not a valid identifier!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	
	// variable, varlist, :, datatype, ;, variable_production_optional()
	if (token_eval.token_type == "03")
	{
		// store variable name
		variable_list_holding.push(token_eval.token_value);

		token_eval = lexer.get_token();
		variable_list();
		if (token_eval.token_value == ":")
		{
			token_eval = lexer.get_token();
			variable_datatype();
			if (token_eval.token_value == ";")
			{
				token_eval = lexer.get_token();
				variable_production_optional();
			}
			else
			{
				cout << "Error! Expected ';' after datatype!"
					<< endl;
				cin.get();
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "Error! ':' token expected after identifier(s)!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}

	// epsilon
};

// language = "(", parameter_production ")", ";"
void Recursive_Parser::parameter()
{
	if (token_eval.token_value == "(")
	{
		token_eval = lexer.get_token();
		// TODO
		parameter_production();
		if (token_eval.token_value == ")")
		{
			token_eval = lexer.get_token();
			if (token_eval.token_value == ";")
			{
				token_eval = lexer.get_token();
			}
			else
			{
				cout << "Error! Missing ';' at the end of parameter list!"
					<< endl;
				cin.get();
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "Error! Missing ')' at the end of parameter list!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		cout << "Error! Missing '(' at the beginning of parameter list!"
			<< endl;
		cin.get();
		exit(EXIT_FAILURE);
	}
};

// this is the same as variable_production, but var is optional,
// and epsilon is a valid option
void Recursive_Parser::parameter_production()
{
	local_variable_type = 'p';
	// is the parameter passed by reference?
	if (token_eval.token_value == "var")
	{
		passing_by_reference = true;
		token_eval = lexer.get_token();
		
		if (token_eval.token_type == "03")
		{
			//store the parameter
			variable_list_holding.push(token_eval.token_value);

			token_eval = lexer.get_token();
			variable_list();
			if (token_eval.token_value == ":")
			{
				token_eval = lexer.get_token();
				variable_datatype();

				// check for more parameter lists
				parameter_production_optional();
			}
			else
			{
				cout << "Error! Missing ':' in parameter list!"
					<< endl;
				cin.get();
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "Error! Identifier expected in parameter list!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	else if (token_eval.token_type == "03")
	{
		passing_by_reference = false;
		//store the parameter
		variable_list_holding.push(token_eval.token_value);

		token_eval = lexer.get_token();
		variable_list();
		if (token_eval.token_value == ":")
		{
			token_eval = lexer.get_token();
			variable_datatype();

			// check for more parameter lists
			parameter_production_optional();
		}
		else
		{
			cout << "Error! Missing ':' in parameter list!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}

	// epsilon
};

// the same as variable_production_optional; allows additional
// declaration/lists of parameters. Epsilon, and var are options
void Recursive_Parser::parameter_production_optional()
{
	if (token_eval.token_value == ";")
	{
		token_eval = lexer.get_token();
		// is the parameter passed by reference
		if (token_eval.token_value == "var")
		{
			passing_by_reference = true;
			token_eval = lexer.get_token();
 
			if (token_eval.token_type == "03")
			{
				//store the parameter
				variable_list_holding.push(token_eval.token_value);

				token_eval = lexer.get_token();
				variable_list();
				if (token_eval.token_value == ":")
				{
					token_eval = lexer.get_token();
					variable_datatype();

					//check for more parameter lists
					parameter_production_optional();
				}
				else
				{
					cout << "Error! Missing ':' in parameter list!"
						<< endl;
					cin.get();
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				cout << "Error! Identifier expected in parameter list!"
					<< endl;
				cin.get();
				exit(EXIT_FAILURE);
			}
		}
		else if (token_eval.token_type == "03")
		{
			passing_by_reference = false;
			//store the parameter
			variable_list_holding.push(token_eval.token_value);

			token_eval = lexer.get_token();
			variable_list();
			if (token_eval.token_value == ":")
			{
				token_eval = lexer.get_token();
				variable_datatype();

				//check for more parameter lists
				parameter_production_optional();
			}
			else
			{
				cout << "Error! Missing ':' in parameter list!"
					<< endl;
				cin.get();
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "Error! Missing identifier in parameter list!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	// epsilon
};

void Recursive_Parser::block()
{
	if (token_eval.token_value == "begin")
	{
		token_eval = lexer.get_token();
		statement();
		multiple_statement();
		if (token_eval.token_value == "end")
		{
			token_eval = lexer.get_token();
			/*if (token_eval.token_value == ".")
			{
				token_eval = lexer.get_token();
			}
			else
			{
				cout << "Error ending block! Missing '.'" << endl;
				exit(EXIT_FAILURE);
			}*/
		}
		else
		{
			cout << "Error ending block! Missing 'end'" << endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		cout << "Error beginning block! Missing 'begin'" << endl;
		cin.get();
		exit(EXIT_FAILURE);
	}
};

void Recursive_Parser::var_func_proc()
{
	current_scope = "";
	if (token_eval.token_value == "var")
	{
		token_eval = lexer.get_token();
		variable_production();
		var_func_proc();
	}
	else if (token_eval.token_value == "procedure")
	{
		current_scope_offset = 0;
		token_eval = lexer.get_token();
		procedure_production();
		var_func_proc();
	}
	else if (token_eval.token_value == "function")
	{
		current_scope_offset = 0;
		token_eval = lexer.get_token();
		function_production();
		var_func_proc();
	}
	// reset scope
	current_scope = "";
	// epsilon
};

void Recursive_Parser::inter_variable_production()
{
	local_variable_type = 'l';
	if (token_eval.token_value == "var")
	{
		token_eval = lexer.get_token();
		variable_production();
	}
	// epsilon
}

void Recursive_Parser::procedure_production()
{
	if (token_eval.token_type == "03")
	{
		//capture the name of the procedure
		current_scope = token_eval.token_value;
		// check to see if this exists elsewhere
		if (symbol_table.count(current_scope) > 0)
		{
			cout << "Error! Identifier " << current_scope << " already exists!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}

		symbol_table[current_scope].offset = 0;
		symbol_table[current_scope].size = 0;

		token_eval = lexer.get_token();
		if (token_eval.token_value == "(")
		{
			token_eval = lexer.get_token();
			parameter_production();
			if (token_eval.token_value == ")")
			{
				token_eval = lexer.get_token();
				if (token_eval.token_value == ";")
				{
					token_eval = lexer.get_token();
					inter_variable_production();
					block();
					if (token_eval.token_value == ";")
					{
						token_eval = lexer.get_token();
					}
					else
					{
						cout << "Error! Missing ';' at the end of procedure block!"
							<< endl;
						cin.get();
						exit(EXIT_FAILURE);
					}
				}
				else
				{
					cout << "Error! Missing ';' at the end of procedures parameters!"
						<< endl;
					cin.get();
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				cout << "Error! Missing '(' at procedures parameters!"
					<< endl;
				cin.get();
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "Error! Missing '(' at procedure declaration!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
		// reset current scope to global aka null
		current_scope = "";
	}
	else
	{
		cout << "Error! Missing identifier for procedure!"
			<< endl;
		cin.get();
		exit(EXIT_FAILURE);
	}
};

void Recursive_Parser::function_production()
{
	if (token_eval.token_type == "03")
	{
		//capture the name of the procedure
		current_scope = token_eval.token_value;
		// check to see if this exists elsewhere
		if (symbol_table.count(current_scope) > 0)
		{
			cout << "Error! Identifier " << current_scope << " already exists!"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}

		symbol_table[current_scope].offset = 0;
		symbol_table[current_scope].size = 0;

		token_eval = lexer.get_token();
		if (token_eval.token_value == "(")
		{
			token_eval = lexer.get_token();
			parameter_production();
			if (token_eval.token_value == ")")
			{
				token_eval = lexer.get_token();
				if (token_eval.token_value == ":")
				{
					token_eval = lexer.get_token();
					//variable_datatype();
					// so we know what datatype this function returns
					local_variable_type = 'f';
					variable_simple_datatype();
					if (token_eval.token_value == ";")
					{
						token_eval = lexer.get_token();
						inter_variable_production();
						block();
						if (token_eval.token_value == ";")
						{
							token_eval = lexer.get_token();
						}
						else
						{
							cout << "Error! Missing ';' at the end of function definition!"
								<< endl;
							cin.get();
							exit(EXIT_FAILURE);
						}
					}
					else
					{
						cout << "Error! Missing ';' at end of function block!"
							<< endl;
						cin.get();
						exit(EXIT_FAILURE);
					}
				}
				else
				{
					cout << "Error! Missing ':' to specify function return type!"
						<< endl;
					cin.get();
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				cout << "Error! Missing ')' at end of function parameter list!"
					<< endl;
				cin.get();
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "Error! Missing '(' after function identifier!"
				<< endl; 
			cin.get();
			exit(EXIT_FAILURE);
		}
		//reset current_scope to global aka null
		current_scope = "";
	}
	else
	{
		cout << "Error! Missing identifier for function!"
			<< endl;
		cin.get();
		exit(EXIT_FAILURE);
	}
};

bool Recursive_Parser::find_identifier()
{
	//we can only access globals
	if (current_scope.empty() == true)
	{
		if (symbol_table.count(token_eval.token_value) > 0)
			return true;
		else
			return false;
	}
	// we are in some scope
	else
	{
		// check the locals first
		if (symbol_table[current_scope].local_variables.count(token_eval.token_value) > 0)
			return true;
		// check the parameters
		else if (symbol_table[current_scope].parameters.count(token_eval.token_value) > 0)
			return true;
		// check the globals
		else if (symbol_table.count(token_eval.token_value) > 0)
			return true;
		else
			return false;
	}
}
void Recursive_Parser::program_production()
{
	if(token_eval.token_value == "program")
	{
		token_eval = lexer.get_token();
		//program identifier
		if(token_eval.token_type == "03")
		{
			token_eval = lexer.get_token();
			if(token_eval.token_value == "(")
			{
				token_eval = lexer.get_token();
				//some input func goes here
				/*
				program_io();
				*/
				if(token_eval.token_value == ")")
				{
					token_eval = lexer.get_token();
					if(token_eval.token_value == ";")
					{
						token_eval = lexer.get_token();

						// Preliminary declarations
						var_func_proc();
						block();

						if(token_eval.token_value == ".")
						{
							token_eval = lexer.get_token();
						}
						else
						{
							cout << "Error! Missing '.' at end of program block!" <<
								endl;
							cin.get();
							exit(EXIT_FAILURE);
						}
					}
					else
					{
						cout << "Error! Missing ';' at end of program header!" <<
							endl;
						cin.get();
						exit(EXIT_FAILURE);
					}
				}
				else
				{
					cout << "Error! Missing ')' at end of program input!" <<
						endl;
					cin.get();
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				cout << "Error! Missing '(' at beginning of program input!" <<
					endl;
				cin.get();
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			cout << "Error! Expecting program identifier. Did you use a keyword, instead?"
				<< endl;
			cin.get();
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		cout << "Error. Did not define program!" << endl;
		cin.get();
		exit(EXIT_FAILURE);
	}
};