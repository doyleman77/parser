#pragma once
#include <string>

struct Parameter_Data
{
	std::string		type;
	int				size;
	int				offset;
	bool			referred;
};