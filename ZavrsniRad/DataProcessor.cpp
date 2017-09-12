#include "DataProcessor.h"

namespace vsite {

	// PUBLIC MEMBERS

	DataProcessor::DataProcessor(const std::vector<std::string> &columns, std::vector<vsite::Country> &countries) : keys(columns), countries(countries), output(std::cout), selectionFirst(countries.begin()), selectionLast(countries.end()) {
	}

	DataProcessor::DataProcessor(const std::vector<std::string> &columns, std::vector<vsite::Country> &countries, std::ostream &os) : keys(columns), countries(countries), output(os), selectionFirst(countries.begin()), selectionLast(countries.end()) {
	}

	void DataProcessor::selectByName(const std::string &name) {
		if (name.empty()) {
			output << "No data to display." << endl;
			return;
		}
		byName(name, selectionFirst, selectionLast);
		if (selectionFirst != selectionLast) {
			sortByName(selectionFirst, selectionLast);
		}
		else {
			output << "No countries match selection." << endl;
		}
	}

	void DataProcessor::selectByColumnValue(const std::string &column, const double &min, const double &max) {
		int index;
		try {
			index = getIndex(column);
		}
		catch (const std::invalid_argument &e) {
			output << e.what() << endl;
			return;
		}
		byColumn(index, min, max, selectionFirst, selectionLast);
		if (selectionFirst != selectionLast) {
			sortByColumn(index, selectionFirst, selectionLast);
		}
		else {
			output << "No countries match selection." << endl;
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
			output << e.what() << endl;
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

	double DataProcessor::average(const std::string &column) {
		int index;
		try {
			index = getIndex(column);
		}
		catch (const std::invalid_argument &e) {
			output << e.what() << endl;
			return numeric_limits<double>::quiet_NaN();
		}
		int countriesWithValues = 0;
		countriesWithValues = count_if(selectionFirst, selectionLast,
			[index](const vsite::Country &country) {
			return !country.values.at(index).isUndefined();
			});
		if (countriesWithValues == 0) {
			output << "Selection contains no values." << endl;
			return numeric_limits<double>::quiet_NaN();
		}
		vsite::Field sum = std::accumulate(selectionFirst, selectionLast, vsite::Field(0.0),
			[index](const vsite::Field &left, const vsite::Country &right) {
				return left + right.values.at(index);
			});
		return sum / countriesWithValues;
	}

	void DataProcessor::clear() {
		selectionFirst = countries.begin();
		selectionLast = countries.end();
	}


	// PRIVATE MEMBERS

	// Partitions elements in range first to last by country name and sets iterator last to partition point 
	void DataProcessor::byName(const std::string &name, std::vector<vsite::Country>::iterator &first, std::vector<vsite::Country>::iterator &last) {
		std::string loweredInputName(name);
		std::transform(loweredInputName.begin(), loweredInputName.end(), loweredInputName.begin(), ::tolower);
		last = std::partition(first, last,
			[&loweredInputName](const vsite::Country &country) {
			std::string loweredCountryName = country.name;
			std::transform(loweredCountryName.begin(), loweredCountryName.end(), loweredCountryName.begin(), ::tolower);
			return loweredCountryName.find(loweredInputName) != std::string::npos;
		});
	}

	// Partitions elements in range first to last by column name and sets iterator last to partition point 
	void DataProcessor::byColumn(const int &index, const double &min, const double &max, std::vector<vsite::Country>::iterator &first, std::vector<vsite::Country>::iterator &last) {	
		last = std::partition(first, last,
			[&index, &min, &max](const vsite::Country &country) {
			return country.values.at(index) >= min && country.values.at(index) <= max;
		});
	}

	void DataProcessor::sortByColumn(const int &index, std::vector<vsite::Country>::iterator &first, std::vector<vsite::Country>::iterator &last) {
		std::sort(first, last,
			[&index](const vsite::Country &left, const vsite::Country &right) {
			return left.values.at(index) < right.values.at(index);
		});
	}

	void DataProcessor::sortByName(std::vector<vsite::Country>::iterator &first, std::vector<vsite::Country>::iterator &last) {
		std::sort(first, last,
			[](const vsite::Country &left, const vsite::Country &right) {
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
			output << resized << ' ';
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
			if (std::find(validKeys.cbegin(), validKeys.cend(), keys.at(i)) != validKeys.cend())
				indexes.push_back(i - 1);
		}
		return indexes;
	}
}