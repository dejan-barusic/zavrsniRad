#include <iostream>
#include <string>

namespace vsite {

#pragma once
	class Field
	{
	public:
		Field();
		Field(double);
		Field(const Field&);
		Field operator+(const Field&) const;
		bool operator==(const Field&) const;
		bool operator!=(const Field&) const;
		bool operator<(const Field&) const;
		friend std::ostream& operator<<(std::ostream &os, const Field &field);
		friend std::istream& operator>>(std::istream &is, Field &field);
	private:
		double value;
		bool undefined;
	};

}