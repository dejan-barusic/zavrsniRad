#include <locale>

#pragma once
class csvCtype : public std::ctype<char>
{
public:
	csvCtype(size_t refs = 0);

	mask table[table_size];
};

