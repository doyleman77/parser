#pragma once
#include <string>
#include <utility>
#include <vector>

struct Local_Variable
{
	std::vector<std::pair<int, int>>	array_indices;
	std::string							type;
	int									size;
	int									offset;
};