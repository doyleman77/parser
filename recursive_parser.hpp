#pragma once
#include <map>
#include <queue>
#include <string>

#include "global_scope.hpp"
#include "lexer.hpp"
#include "token.hpp"

class Recursive_Parser
{
private:
	Lexer					lexer;
	Token					token_eval;

	string					current_scope;
	string					current_variable;

	unsigned int			variable_size_multiplier;
	
	unsigned int			current_scope_offset;
	unsigned int			global_scope_offset;

	char					local_variable_type; // p = parameter, l = local, f for function. should only check for these
	bool					passing_by_reference; // used for symbol table
	bool					inserting_array;	  // used for symbol table to insert range pairs
	std::queue<string>		variable_list_holding;

	std::vector<std::pair<int, int>> temp_array_indices;
	
	std::map <string, Global_Scope> symbol_table;
public:
	/*ctr*/		Recursive_Parser();
	/*dtr*/		~Recursive_Parser();

	void		run();

	void		expr();
	void		term();
	void		term_prime();
	void		factor();
	void		factor_prime();
	
	void		bool_expr();
	void		multiple_statement();
	void		statement();
	void		block();
	
	void		variable_list();
	void		variable_production();
	void		array_production();
	void		array_dimension_production();
	void		variable_production_optional();
	void		variable_datatype();
	void		variable_simple_datatype();

	void		inter_variable_production();
	void		parameter();
	void		parameter_production();
	void		parameter_production_optional();

	void		var_func_proc(); 
	void		program_production();
	void		function_production();
	void		procedure_production();

	bool		find_identifier();
};