#include "stdafx.h"


int main()
{
	using namespace std;

	list<vsite::Country> countries;
	ifstream file("zavrsnidb.csv");
	string row;
	vector<string> keys, values;

	if (file.good()) {
		getline(file, row);
		keys = vsite::Tokenizer::tokenize(row, ';');
	}

	getline(file, row);

	while (file.good())	{
		std::getline(file, row);
		values = vsite::Tokenizer::tokenize(row, ';');
		try {
			if(keys.size() == values.size())
				countries.push_back(vsite::Country(keys, values));
			else
				throw invalid_argument("wrong number of data fields for country " + values[0]);
		}
		catch (const invalid_argument& ia) {
			cout << "Error while creating country data: " << ia.what() << endl;
		}
	}

    return 0;
}

