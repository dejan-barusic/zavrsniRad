#include <string>
#include <map>
#include <vector>

#pragma once
namespace vsite {

	using namespace std;

	class Country
	{
	public:
		Country(vector<string>, vector<string>);
		~Country();

		std::string name;
		std::map<std::string, double> data;
	};
}