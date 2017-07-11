#include "Tokenizer.h"


namespace vsite {

	vector<string> Tokenizer::tokenize(const string input, char delimiter)
	{
		vector<string> tokens{};
		auto fbegin = input.cbegin();
		auto fend = input.cbegin();

		for (auto i = input.cbegin(); i != input.cend(); ++i) {
			if (*i == delimiter) {
				tokens.push_back(string(fbegin, fend));
				++fend;
				fbegin = fend;
			}
			else
				++fend;
		}

		if(fbegin < fend)
			tokens.push_back(string(fbegin, fend));
		
		return tokens;
	}


	Tokenizer::Tokenizer()
	{
	}


	Tokenizer::~Tokenizer()
	{
	}
}