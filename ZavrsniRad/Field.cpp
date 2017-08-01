#include "Field.h"

namespace vsite {

	Field::Field() : undefined(true) {
	}

	Field::Field(double input) : value(input), undefined(false)	{
	}

	Field::Field(const Field &other) : value(other.value), undefined(other.undefined) {
	}

	Field Field::operator+(const Field &other) const {
		if (undefined && other.undefined)
			return Field();
		else if (undefined)
			return Field(other);
		else if (other.undefined)
			return *this;
		else return Field(value + other.value);
	}

	bool Field::operator==(const Field &other) const {
		return value == other.value && undefined == other.undefined;
	}

	bool Field::operator!=(const Field &other) const {
		return !(*this == other);
	}

	bool Field::operator<(const Field &other) const {
		return !undefined & !other.undefined & (value < other.value);
	}

	Field::operator double() const {
		if (undefined)
			return std::numeric_limits<double>::quiet_NaN();
		return
			value;
	}

	std::ostream& operator<<(std::ostream &os, const vsite::Field &field) {
		if (!field.undefined)
			os << field.value;
		return os;
	}

	std::istream& operator>>(std::istream &is, Field &field) {
		std::string text;
		is >> text;
		try { field.value = std::stod(text); }
		catch (const std::invalid_argument) {}
		field.undefined = std::isnan(field.value);
		return is;
	}

}
