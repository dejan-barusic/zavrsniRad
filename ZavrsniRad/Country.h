#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include "csvCtype.h"
#include "Field.h"

#pragma once
namespace vsite {

	using namespace std;

	class Country
	{
	public:
		Country(const vector<string> &keys, vector<string> values);

		std::string name;
		std::map<std::string, double> data;
	};

	class Country2
	{
	public:
		Country2(const std::string name, std::vector<std::string> &keys, const std::vector<vsite::Field> values);
		void operator=(const Country2 &other);
		
		std::string name;
		std::vector<std::string> &keys;
		std::vector<vsite::Field> values;
	};
}