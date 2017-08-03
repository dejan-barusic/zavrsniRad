#include "DataProcessor.h"

namespace vsite {

	DataProcessor::DataProcessor(const std::vector<std::string> &keys, std::vector<vsite::Country> &countries) : keys(keys), countries(countries), output(std::cout), selectionFirst(countries.begin()), selectionLast(countries.end()) {
	}

	DataProcessor::DataProcessor(const std::vector<std::string> &keys, std::vector<vsite::Country> &countries, std::ostream &os) : keys(keys), countries(countries), output(os), selectionFirst(countries.begin()), selectionLast(countries.end()) {
	}

	void DataProcessor::selectByName(const std::string &name) {
		if (name.empty()) {
			output << endl << "No data to display." << endl << endl;
			return;
		}
		byName(name, selectionFirst, selectionLast);
		if (selectionFirst != selectionLast) {
			sortByName(selectionFirst, selectionLast);
		}
		else {
			output << endl << "No countries match selection." << endl << endl;
		}
	}

	void DataProcessor::selectByColumnValue(const std::string &column, const double &min, const double &max) {
		int index;
		try {
			index = getIndex(column);
		}
		catch (const std::invalid_argument &e) {
			output << std::endl << e.what() << std::endl << std::endl;
			return;
		}
		byColumn(index, min, max, selectionFirst, selectionLast);
		if (selectionFirst != selectionLast) {
			sortByColumn(index, selectionFirst, selectionLast);
		}
		else {
			output << endl << "No countries match selection." << endl << endl;
		}
	}

	void DataProcessor::outSelection() {
		vector<int> emptyIndexes{};
		toOutput(emptyIndexes, selectionFirst, selectionLast);
	}

	void DataProcessor::outSelection(const std::string &column) {
		int index;
		try {
			index = getIndex(column);
		}
		catch (const std::invalid_argument &e) {
			output << std::endl << e.what() << std::endl << std::endl;
			return;
		}
		toOutput(index, selectionFirst, selectionLast);
	}

	void DataProcessor::outSelection(const std::vector<std::string> &columns) {
		toOutput(getIndexes(columns), selectionFirst, selectionLast);
	}

	void DataProcessor::outColumnNames() {
		for (auto i = keys.cbegin(); i != keys.cend(); ++i)
			output << *i << std::endl;
	}

	void DataProcessor::sortAlphabetically() {
		sortByName(selectionFirst, selectionLast);
	}

	void DataProcessor::sortByColumnName(const std::string &column) {
		int index;
		try {
			index = getIndex(column);
		}
		catch (const std::invalid_argument &e) {
			output << std::endl << e.what() << std::endl << std::endl;
			return;
		}
		sortByColumn(index, selectionFirst, selectionLast);
	}

	void DataProcessor::clear() {
		selectionFirst = countries.begin();
		selectionLast = countries.end();
	}

	// Partitions elements in range first to last by country name and sets iterator last to partition point 
	void DataProcessor::byName(const std::string &name, std::vector<vsite::Country>::iterator &first, std::vector<vsite::Country>::iterator &last) {
		std::string loweredInputName(name);
		std::transform(loweredInputName.begin(), loweredInputName.end(), loweredInputName.begin(), ::tolower);
		last = std::partition(first, last,
			[&loweredInputName](vsite::Country country) {
			std::string loweredCountryName = country.name;
			std::transform(loweredCountryName.begin(), loweredCountryName.end(), loweredCountryName.begin(), ::tolower);
			return loweredCountryName.find(loweredInputName) != std::string::npos;
		});
	}

	// Partitions elements in range first to last by column name and sets iterator last to partition point 
	void DataProcessor::byColumn(const int &index, const double &min, const double &max, std::vector<vsite::Country>::iterator &first, std::vector<vsite::Country>::iterator &last) {	
		last = std::partition(first, last,
			[&index, &min, &max](vsite::Country country) {
			return country.values.at(index) >= min && country.values.at(index) <= max;
		});
	}

	void DataProcessor::sortByColumn(const int &index, std::vector<vsite::Country>::iterator &first, std::vector<vsite::Country>::iterator &last) {
		std::sort(first, last,
			[&index](vsite::Country left, vsite::Country right) {
			return left.values.at(index) < right.values.at(index);
		});
	}

	void DataProcessor::sortByName(std::vector<vsite::Country>::iterator &first, std::vector<vsite::Country>::iterator &last) {
		std::sort(first, last,
			[](vsite::Country left, vsite::Country right) {
			return left.name < right.name;
		});
	}

	// Outputs element value selected by index for each element in range first to last
	void DataProcessor::toOutput(const int &index, const std::vector<vsite::Country>::iterator &first, const std::vector<vsite::Country>::iterator &last) {
		vector<int> indexes;
		indexes.push_back(index);
		toOutput(indexes, first, last);
	}

	// Outputs element values selected by indexes for each element in range first to last
	void DataProcessor::toOutput(const vector<int> &indexes, const std::vector<vsite::Country>::iterator &first, const std::vector<vsite::Country>::iterator &last) {
		if (selectionFirst == selectionLast)
			return;
		const int WIDTH(20);
		// Outputs name of first column
		{
			string resized(keys.at(0));
			resized.resize(WIDTH, ' ');
			output << endl << resized << ' ';
		}
		// Outputs names of selected columns
		for (unsigned i = 0; i < indexes.size(); ++i) {
			string resized(keys.at(indexes.at(i) + 1));
			resized.resize(WIDTH, ' ');
			output << resized;
		}
		output << endl;
		// Outputs rows
		for (auto country = first; country != last; ++country) {
			string wname(country->name);
			wname.resize(WIDTH, ' ');
			output << wname << ' ';
			for (unsigned i = 0; i < indexes.size(); ++i) {
				output << std::resetiosflags(std::ios::adjustfield);
				output << std::setiosflags(std::ios::left);
				output.width(WIDTH);
				output << country->values.at(indexes.at(i));
			}
			output << endl;
		}
		output << endl;
	}

	// Returns valid key for partial key argument
	std::string DataProcessor::getValidKey(const std::string &partialKey) {
		if (partialKey.empty())
			return "";
		std::string lowerKey(partialKey);
		std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), ::tolower);
		auto result = std::find_if(keys.cbegin(), keys.cend(),
			[&lowerKey](std::string validKey) {
			std::transform(validKey.begin(), validKey.end(), validKey.begin(), ::tolower);
			return validKey.find(lowerKey) != std::string::npos;
		});
		return result != keys.end() ? *result : "";
	}

	// Returns index for selected key
	int DataProcessor::getIndex(const std::string &selectedKey) {
		std::string validKey = getValidKey(selectedKey);
		if (validKey.empty())
			throw std::invalid_argument("Not found");
		int index = 0;
		for (unsigned i = 1; i < keys.size(); ++i) {
			if (keys.at(i) == validKey) {
				index = i - 1;
				break;
			}
		}
		return index;
	}

	// Returns indexes for selected keys
	std::vector<int> DataProcessor::getIndexes(const std::vector<std::string> &selectedKeys) {
		std::vector<int> indexes;
		std::vector<std::string> validKeys;
		for (auto i = selectedKeys.cbegin(); i != selectedKeys.cend(); ++i) {
			std::string validKey = getValidKey(*i);
			if(!validKey.empty())
				validKeys.push_back(validKey);
		}
		for (unsigned i = 1; i < keys.size(); ++i) {
			if (find(validKeys.cbegin(), validKeys.cend(), keys.at(i)) != validKeys.cend())
				indexes.push_back(i - 1);
		}
		return indexes;
	}
}




// TO BE IMPLEMENTED

//vsite::Country findSingleCountryWithMinValue(std::list<vsite::Country> countries, std::string validKey) {
//	auto result = std::min_element(countries.cbegin(), countries.cend(),
//		[&validKey](vsite::Country left, vsite::Country right) {
//		return left.data[validKey] < right.data[validKey];
//	});
//	return *result;
//}
//
//
//bool checkSorting(std::list<vsite::Country> countries, std::string validKey) {
//	return std::is_sorted(countries.cbegin(), countries.cend(),
//		[&validKey](vsite::Country left, vsite::Country right) {
//		return left.data[validKey] < right.data[validKey];
//	});
//}
//
//std::list<vsite::Country> reverseOrder(std::list<vsite::Country> countries) {
//	std::reverse(countries.begin(), countries.end());
//	return countries;
//}
//
//double calculateAvarageByKey(std::list<vsite::Country> countries, std::string validKey) {
//	return std::accumulate(countries.cbegin(), countries.cend(), 0.0,
//		[&validKey](double left, vsite::Country right) {
//		return left + right.data[validKey];
//	}) / countries.size();
//}