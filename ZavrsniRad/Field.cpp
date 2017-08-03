#include "Field.h"

namespace vsite {

	Field::Field() : undefined(true) {
	}

	Field::Field(double input) : value(input), undefined(false)	{
	}

	Field::Field(const Field &other) : value(other.value), undefined(other.undefined) {
	}

	Field& Field::operator+(const Field &other) {
		if (undefined & other.undefined)
			return Field();
		if (undefined & !other.undefined)
			return Field(other);
		if (!undefined & other.undefined)
			return Field(*this);
		value += other.value;
		return *this;
	}

	bool Field::operator==(const Field &other) {
		return value == other.value & undefined == other.undefined;
	}

	bool Field::operator!=(const Field &other) {
		return !(*this == other);
	}

	bool Field::operator<(const Field &other) {
		if (undefined & other.undefined)
			return false;
		if (undefined & !other.undefined)
			return true;
		if (!undefined & other.undefined)
			return false;
		return value < other.value;
	}

	Field::operator double() const {
		if (undefined)
			return std::numeric_limits<double>::quiet_NaN();
		return
			value;
	}

	bool Field::isUndefined() {
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
		catch (const std::invalid_argument) {}
		field.undefined = std::isnan(field.value);
		return is;
	}

}
