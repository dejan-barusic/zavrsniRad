#include <string>
#include <map>
#include <vector>

#pragma once
namespace vsite {

	class Country
	{
	public:
		Country(vector<string>, vector<string>);
		~Country();

		std::string name;
		std::map<std::string, double> data;
	};
}