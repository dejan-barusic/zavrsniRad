#pragma once
#include <iostream>
#include <string>

namespace vsite {

	class Field	{

	public:
		Field();
		Field(double);
		Field(const Field&);
		bool operator==(const Field&);
		bool operator!=(const Field&);
		bool operator<(const Field&);
		operator double() const;
		friend std::ostream& operator<<(std::ostream &os, const Field &field);
		friend std::istream& operator>>(std::istream &is, Field &field);
		bool isUndefined() const;
		friend Field operator+(const Field &left, const Field &right);

	private:
		double value;
		bool undefined;
	};
}