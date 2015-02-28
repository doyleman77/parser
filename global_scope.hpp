#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "parameter_data.hpp"
#include "local_variable.hpp"

struct Global_Scope
{
	std::vector<std::pair<int, int>>		array_indices;
	std::string								type;
	int										size;
	int										offset;
	std::map<std::string, Parameter_Data>	parameters;
	std::map<std::string, Local_Variable>	local_variables;
};