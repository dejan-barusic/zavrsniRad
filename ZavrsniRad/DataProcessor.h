#include <vector>
#include <ostream>
#include <iomanip>
#include "Country.h"

namespace vsite {

#pragma once
	class DataProcessor
	{
	public:
		DataProcessor(const std::vector<std::string> &keys, std::vector<vsite::Country2> &countries);
		DataProcessor(const std::vector<std::string> &keys, std::vector<vsite::Country2> &countries, std::ostream &os);

		void out(const std::string &name, const std::vector<std::string> &selectedKeys);


	private:
		std::string getValidKey(const std::string &partialKey);
		std::vector<int> getIndexes(const std::vector<std::string> &selectedKeys);
		void toOutput(std::vector<vsite::Country2>::iterator fromCountry, std::vector<vsite::Country2>::iterator toCountry, vector<int> valuesIndexes);

		const std::vector<std::string> &keys;
		std::vector<vsite::Country2> &countries;
		std::ostream &output;
	};
}