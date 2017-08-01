#include "csvCtype.h"

csvCtype::csvCtype(size_t refs) : std::ctype<char>(&table[0], false, refs) {
	std::copy_n(classic_table(), table_size, table);
	table[';'] |= space; // will be classified as whitespace
	table[','] |= space; // will be classified as whitespace
	table[' '] &= ~space; // will not be classified as whitespace
}