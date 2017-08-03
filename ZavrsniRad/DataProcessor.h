#include <vector>
#include <ostream>
#include <iomanip>
#include "Country.h"
#include <algorithm>
#include <numeric>
#include <stdlib.h>
#include <iomanip>
#include <ios>
#include <string>

namespace vsite {

	using namespace std;

#pragma once
	class DataProcessor {
	public:
		DataProcessor(const std::vector<std::string> &keys, std::vector<vsite::Country> &countries);
		DataProcessor(const std::vector<std::string> &keys, std::vector<vsite::Country> &countries, std::ostream &os);
		void selectByName(const std::string &name);
		void selectByColumnValue(const std::string &column, const double &min, const double &max);
		void outSelection();
		void outSelection(const std::string &column);
		void outSelection(const std::vector<std::string> &columns);
		void outColumnNames();
		void sortAlphabetically();
		void sortByColumnName(const std::string &column);
		void clear();

	private:
		std::string getValidKey(const std::string &partialKey);
		int getIndex(const std::string & selectedKey);
		std::vector<int> getIndexes(const std::vector<std::string> &selectedKeys);
		void byName(const std::string &name, std::vector<vsite::Country>::iterator &first, std::vector<vsite::Country>::iterator &last);
		void byColumn(const int &index, const double & minValue, const double &maxValue, std::vector<vsite::Country>::iterator &first, std::vector<vsite::Country>::iterator & last);
		void sortByColumn(const int &index, std::vector<vsite::Country>::iterator & first, std::vector<vsite::Country>::iterator &last);
		void sortByName(std::vector<vsite::Country>::iterator & first, std::vector<vsite::Country>::iterator & last);
		void toOutput(const int &index, const std::vector<vsite::Country>::iterator &first, const std::vector<vsite::Country>::iterator &last);
		void toOutput(const vector<int> &indexes, const std::vector<vsite::Country>::iterator &first, const std::vector<vsite::Country>::iterator &last);

		const std::vector<std::string> &keys;
		std::vector<vsite::Country> &countries;
		std::ostream &output;
		std::vector<vsite::Country>::iterator selectionFirst, selectionLast;
	};
}