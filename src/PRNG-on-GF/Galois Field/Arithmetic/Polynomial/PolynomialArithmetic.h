#pragma once
#ifndef _POLYNOMIALARITHMETIC_H_
#define _POLYNOMIALARITHMETIC_H_
#include "../Modular/ModularArithmetic.h"
#include <vector>
#include <list>

using CoefficientsVector = std::vector<Int>;

CoefficientsVector polynomial_add(const CoefficientsVector& left, const CoefficientsVector& right, const Int mod);
CoefficientsVector polynomial_sub(const CoefficientsVector& left, const CoefficientsVector& right, const Int mod);
CoefficientsVector polynomial_mul(const CoefficientsVector& left, const CoefficientsVector& right, const Int mod);
std::pair<CoefficientsVector, CoefficientsVector> polynomial_div(const CoefficientsVector& left, const CoefficientsVector& right, const Int mod);

Int polynomial_degree(const CoefficientsVector& polynomial_coefficients);

namespace prim_poly {
	using PolynomialsList = std::list<CoefficientsVector>;

	namespace detail {
		CoefficientsVector to_binary(Int number, Int size, const CoefficientsVector& begin_value = CoefficientsVector());

		CoefficientsVector power2(Int degree, Int size);

		CoefficientsVector binary_xor(const CoefficientsVector& left, const CoefficientsVector& right);
		std::pair<CoefficientsVector, CoefficientsVector> div_alg(const CoefficientsVector& left, const CoefficientsVector& right);

		namespace debug {
			void plynomial_print(const CoefficientsVector& polynomial_coefficients);
		}
	};


	PolynomialsList poly_gen(Int degree);

	bool check_irr(const CoefficientsVector& polynomial);
	bool check_div(const CoefficientsVector& left, const CoefficientsVector& right);
	Int poly_order(const CoefficientsVector& polynomial);

	PolynomialsList find_prim(Int degree);
};
#endif