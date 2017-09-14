#include "Field.h"

namespace vsite {

	Field::Field() : undefined(true) {
	}

	Field::Field(double input) : value(input), undefined(false)	{
	}

	Field::Field(const Field &other) : value(other.value), undefined(other.undefined) {
	}

	Field operator+(const Field &left, const Field &right) {
	if (left.undefined && right.undefined)
		return Field();
	if (left.undefined && !right.undefined)
		return Field(right);
	if (!left.undefined && right.undefined)
		return Field(left);
	return Field(left.value + right.value);
	}

	bool Field::operator==(const Field &other) {
		return value == other.value && undefined == other.undefined;
	}

	bool Field::operator!=(const Field &other) {
		return !(*this == other);
	}

	bool Field::operator<(const Field &other) {
		if (undefined && other.undefined)
			return false;
		if (undefined && !other.undefined)
			return true;
		if (!undefined && other.undefined)
			return false;
		return value < other.value;
	}

	Field::operator double() const {
		if (undefined)
			return std::numeric_limits<double>::quiet_NaN();
		return
			value;
	}

	bool Field::isUndefined() const {
		return undefined;
	}

	std::ostream& operator<<(std::ostream &os, const vsite::Field &field) {
		if (!field.undefined)
			os << field.value;
		else
			os << "[NO DATA]";
		return os;
	}

	std::istream& operator>>(std::istream &is, Field &field) {
		std::string text;
		is >> text;
		try { field.value = std::stod(text); }
		catch (const std::invalid_argument) {
			field.undefined = true;
			return is;
		}
		field.undefined = std::isnan(field.value);
		return is;
	}
}
