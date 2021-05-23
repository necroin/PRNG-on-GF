#pragma once
#ifndef _GENERATOR_H_
#define _GENERATOR_H_
#include "../Field/GaloisField.h"
#include <set>


class Generator {
private:
	GaloisField _field;
	std::vector<GFElement> _even_A;
	std::vector<GFElement> _even_C;
	std::vector<GFElement> _odd_A;
	std::vector<GFElement> _odd_C;
	std::vector<GFElement> _Q;
	std::vector<GFElement> _start_Q;
	Int _N;
	bool _even_iteration = false;
private:
	void reset();
	void even_iteration(std::vector<GFElement>& new_Q);
	void odd_iteration(std::vector<GFElement>& new_Q);
	void check_endless_loop(std::vector<GFElement>& Q);
public:
	Generator(GaloisField& field, Int N, std::vector<GFElement> A, std::vector<GFElement> C, std::vector<GFElement> star_Q = std::vector<GFElement>());
public:
	std::vector<GFElement> generate();
	void print_state(std::ostream& out = std::cout);
	void print_all_current_cycle_states(std::ostream& out = std::cout);
	std::vector<std::vector<std::vector<GFElement>>> generate_all_cycles();
	void print_all_cycles(std::ostream& out = std::cout, bool show_states = false);
public:
	static GFElement field_add(const GFElement& left, const GFElement& right);
	static GFElement mod_add(const GFElement& left, const GFElement& right);
	void set_even_iteration_add(decltype(&mod_add) add);
	void set_odd_iteration_add(decltype(&mod_add) add);
private:
	decltype(&mod_add) _even_iteration_add = mod_add;
	decltype(&mod_add) _odd_iteration_add = mod_add;
public:
	void set_even_A(std::vector<GFElement> A);
	void set_even_C(std::vector<GFElement> C);
	void set_odd_A(std::vector<GFElement> A);
	void set_odd_C(std::vector<GFElement> C);
};
#endif