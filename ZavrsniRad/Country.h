#pragma once
#include <string>
#include <vector>
#include "Field.h"

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