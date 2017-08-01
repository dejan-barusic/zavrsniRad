#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include "csvCtype.h"
#include "Field.h"
#include <vector>

#pragma once
namespace vsite {

	class Country
	{
	public:
		Country(const std::string name, std::vector<std::string> &keys, const std::vector<vsite::Field> values);
		void operator=(const Country &other);
		
		std::string name;
		std::vector<std::string> &keys;
		std::vector<vsite::Field> values;
	};
}