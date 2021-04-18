#include "GFElement.h"
#include "../GaloisField.h"

void GFElement::field_assert(const GFElement& other) const
{
	if (_field != other._field) throw std::exception("the fields are not compatible");
}

GFElement::GFElement(const GFElement& other) :
	_coefficients(other._coefficients),
	_field(other._field)
{}

GFElement::GFElement(GFElement&& other) noexcept :
	_coefficients(std::move(other._coefficients)),
	_field(other._field)
{}

GFElement::GFElement(const CoefficientsVector& coefficients, GaloisField& field) :
	_coefficients(coefficients),
	_field(field)
{}

GFElement::GFElement(CoefficientsVector&& coefficients, GaloisField& field) :
	_coefficients(coefficients),
	_field(field)
{}

GFElement::GFElement(const GFElement& left, const GFElement& right, decltype(polynomial_add) polynomial_operator) :
	_field(left._field),
	_coefficients(polynomial_operator(left._coefficients, right._coefficients, left._field.characteristic()))
{}

GFElement& GFElement::operator=(const GFElement& other)
{
	if (this == &other) return *this;
	_field = other._field;
	_coefficients = other._coefficients;
	return *this;
}

GFElement& GFElement::operator=(GFElement&& other) noexcept
{
	if (this == &other) return *this;
	_field = other._field;
	_coefficients = std::move(other._coefficients);
	return *this;
}

GFElement GFElement::operator+(const GFElement& other) const
{
	field_assert(other);
	return GFElement(*this, other, polynomial_add);
}

GFElement GFElement::operator-(const GFElement& other) const
{
	field_assert(other);
	return GFElement(*this, other, polynomial_sub);
}

GFElement GFElement::operator*(const GFElement& other) const
{
	field_assert(other);
	decltype(auto) product = polynomial_mul(_coefficients, other._coefficients, _field.characteristic());
	decltype(auto) reduction_polynomial = _field.reduction_polynomial();
	decltype(auto) reminder = polynomial_div(product, reduction_polynomial, _field.characteristic()).second;
	return GFElement(CoefficientsVector(reminder.begin(), reminder.begin() + _field.dimension()), _field);
}

bool GFElement::operator==(const GFElement& other) const noexcept
{
	field_assert(other);
	for (size_t i = 0; i < _coefficients.size(); ++i) {
		if (_coefficients[i] != other._coefficients[i]) return false;
	}
	return true;
}

bool GFElement::operator!=(const GFElement& other) const noexcept
{
	return !(*this == other);
}

bool GFElement::operator<(const GFElement& other) const noexcept
{
	field_assert(other);
	for (Int i = _coefficients.size() - 1; i >= 0; --i) {
		if (_coefficients[i] < other._coefficients[i]) return true;
		if (_coefficients[i] > other._coefficients[i]) return false;
	}
	return false;
}

bool GFElement::operator>(const GFElement& other) const noexcept
{
	field_assert(other);
	for (size_t i = _coefficients.size() - 1; i > 0; --i) {
		if (_coefficients[i] > other._coefficients[i]) return true;
		if (_coefficients[i] < other._coefficients[i]) return false;
	}
	return false;
}

bool GFElement::operator<=(const GFElement& other) const noexcept
{
	return !(*this > other);
}

bool GFElement::operator>=(const GFElement& other) const noexcept
{
	return !(*this < other);
}

GFElement GFElement::mod_add(const GFElement& other) const
{
	field_assert(other);
	CoefficientsVector new_coefficients(_coefficients);
	Int carry = 0;
	for (size_t i = 0; i < new_coefficients.size(); ++i) {
		auto sum = new_coefficients[i] + other._coefficients[i] + carry;
		new_coefficients[i] = sum % _field.characteristic();
		carry = sum / _field.characteristic();
	}
	return GFElement(std::move(new_coefficients), _field);
}

std::ostream& operator<<(std::ostream& out, const GFElement& element)
{
	decltype(auto) coefficients = element._coefficients;
	decltype(auto) coefficients_it = coefficients.rbegin();
	out << "(";
	out << *coefficients_it++;
	for (; coefficients_it != coefficients.rend(); ++coefficients_it) {
		out << "," << *coefficients_it;
	}
	out << ")";
	out << "[" << element.to_int() << "]";
	return out;
}

Int GFElement::to_int() const
{
	Int result = 0;
	Int two_powers = 1;
	for (size_t i = 0; i < _coefficients.size(); ++i) {
		result += (_coefficients[i] * two_powers);
		two_powers *= 2;
	}
	return result;
}
