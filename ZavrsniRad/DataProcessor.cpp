#include "DataProcessor.h"

namespace vsite {

	DataProcessor::DataProcessor(const std::vector<std::string>& keys, std::vector<vsite::Country2>& countries) : keys(keys), countries(countries), output(std::cout)
	{
	}

	DataProcessor::DataProcessor(const std::vector<std::string>& keys, std::vector<vsite::Country2>& countries, std::ostream & os) : keys(keys), countries(countries), output(os)
	{
	}

	void DataProcessor::out(const std::string &name, const std::vector<std::string>& selectedKeys) {
		if (selectedKeys.empty()) {
			output << endl << "No columns to display." << endl << endl;
			return;
		}
		vector<int> indexes = getIndexes(selectedKeys);

		// odvajanje na particije prema imenu
		std::string loweredInputName(name);
		std::transform(loweredInputName.begin(), loweredInputName.end(), loweredInputName.begin(), ::tolower);
		auto separator = std::partition(countries.begin(), countries.end(),
			[&loweredInputName](vsite::Country2 country) {
			std::string loweredCountryName = country.name;
			std::transform(loweredCountryName.begin(), loweredCountryName.end(), loweredCountryName.begin(), ::tolower);
			return loweredCountryName.find(loweredInputName) != std::string::npos;
		});
		toOutput(countries.begin(), separator, indexes);
	}

	void DataProcessor::toOutput(std::vector<vsite::Country2>::iterator fromCountry, std::vector<vsite::Country2>::iterator toCountry, vector<int> valuesIndexes) {
		const int WIDTH(20);
		// ispis naziva prvog stupca
		{
			string resized(keys.at(0));
			resized.resize(WIDTH, ' ');
			output << resized << ' ';
		}
		// ispis naziva odabranih stupaca
		for (unsigned i = 0; i < valuesIndexes.size(); ++i) {
			string resized(keys.at(valuesIndexes.at(i)));
			resized.resize(WIDTH, ' ');
			output << resized;
		}
		output << endl;
		// ispis redova
		for (auto country = fromCountry; country != toCountry; ++country) {
			string wname(country->name);
			wname.resize(WIDTH, ' ');
			output << wname << ' ';

			for (unsigned i = 0; i < valuesIndexes.size(); ++i) {
				output << std::resetiosflags(std::ios::adjustfield);
				output << std::setiosflags(std::ios::left);
				output.width(WIDTH);
				output << country->values.at(valuesIndexes.at(i) - 1);
			}
			output << endl;
		}
		output << endl;
	}

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

	// traženje indeksa stupaca
	std::vector<int> DataProcessor::getIndexes(const std::vector<std::string> &selectedKeys) {
		std::vector<int> indexes;
		std::vector<std::string> validKeys;
		for (auto i = selectedKeys.cbegin(); i != selectedKeys.cend(); ++i) {
			std::string validKey = getValidKey(*i);
			if(!validKey.empty())
				validKeys.push_back(validKey);
		}
		for (unsigned i = 0; i < keys.size(); ++i) {
			if (find(validKeys.cbegin(), validKeys.cend(), keys.at(i)) != validKeys.cend())
				indexes.push_back(i);
		}
		return indexes;
	}
}