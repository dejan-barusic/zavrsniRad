#include <string>
#include <vector>

#pragma once
namespace vsite {

	using namespace std;

	class Tokenizer
	{
	public:
		static vector<string> tokenize(const string input, char delimiter);
	private:
		Tokenizer();
		~Tokenizer();
	};
}