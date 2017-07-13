// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "Country.h"
#include "Tokenizer.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <list>
#include <array>
#include <algorithm>
#include <numeric>

std::list<vsite::Country> selectCountriesByPartialName(std::list<vsite::Country>, std::string);
std::list<vsite::Country> selectCountriesByData(std::list<vsite::Country>, std::string, double);
std::list<vsite::Country> selectCountriesByData(std::list<vsite::Country>, std::string, double, double);
vsite::Country findSingleCountryWithMinValue(std::list<vsite::Country>, std::string);
std::string getValidKey(std::vector<std::string>, std::string);
std::list<vsite::Country> sortCountriesByData(std::list<vsite::Country>, std::string);
bool checkSorting(std::list<vsite::Country>, std::string);
std::list<vsite::Country> reverseOrder(std::list<vsite::Country>);
double calculateAvarageByKey(std::list<vsite::Country>, std::string);
void displayCountriesNames(std::list<vsite::Country>);
void displayCountriesData(std::list<vsite::Country>, std::vector<std::string>);