// ZavrsniRad.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	std::list<vsite::Country> countries;
	std::ifstream file("zavrsnidb.csv");
	std::string row;

	std::getline(file, row);
	std::getline(file, row);

	while (file.good())	{
		std::getline(file, row);
		countries.push_back(vsite::Country(row));
	}

    return 0;
}

