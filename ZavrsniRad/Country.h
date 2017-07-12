#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>

#pragma once
namespace vsite {

	using namespace std;

	class Country
	{
	public:
		Country(vector<string>, vector<string>);
		~Country();
		void operator=(Country other);

		std::string name;
		std::map<std::string, double> data;
	};
}