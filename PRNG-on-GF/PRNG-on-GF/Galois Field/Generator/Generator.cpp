#include "Generator.h"

void Generator::reset()
{
	_Q = _start_Q;
	_even_iteration = false;
}

void Generator::even_iteration(std::vector<GFElement>& new_Q)
{
	new_Q.emplace_back(_even_iteration_add(_even_A[0] * _Q[_Q.size() - 1], _even_C[0]));
	for (size_t i = 1; i < _Q.size(); ++i) {
		new_Q.emplace_back(_even_iteration_add(_even_A[i] * _Q[_Q.size() - 1] + _Q[i - 1], _even_C[i]));
	}
}

void Generator::odd_iteration(std::vector<GFElement>& new_Q)
{
	new_Q.emplace_back(_odd_iteration_add(_odd_A[0] * _Q[_Q.size() - 1], _odd_C[0]));
	for (size_t i = 1; i < _Q.size(); ++i) {
		new_Q.emplace_back(_odd_iteration_add(_odd_A[i] * _Q[_Q.size() - 1] + _Q[i - 1], _odd_C[i]));
	}
}

void Generator::check_endless_loop(std::vector<GFElement>& Q)
{
	static int repeat = 0;
	static const int max_repeat = 10;
	static std::vector<GFElement> prev_Q;
	if (Q == prev_Q) ++repeat;
	else repeat = 0;
	if (repeat == max_repeat) throw std::exception("endless loop");
	prev_Q = Q;
}

Generator::Generator(GaloisField& field, Int N, std::vector<GFElement> A, std::vector<GFElement> C, std::vector<GFElement> star_Q) :
	_field(field),
	_N(N),
	_even_A(A), _odd_A(A),
	_even_C(C), _odd_C(C)
{
	if (A.size() != N || C.size() != N) {
		throw std::exception("dimensions of the model do not match each other");
	}
	if (star_Q.size() == N) _start_Q = star_Q;
	else _start_Q = std::vector(N, GFElement(CoefficientsVector(field.dimension(), 0), field));
	reset();
}

std::vector<GFElement> Generator::generate()
{
	std::vector<GFElement> new_Q;
	if (_even_iteration) even_iteration(new_Q);
	else odd_iteration(new_Q);
	_even_iteration = !_even_iteration;
	_Q = std::move(new_Q);
	return _Q;
}

void Generator::print_state(std::ostream& out)
{
	for (auto&& q : _Q) {
		out << q.to_int() << " ";
	}
	out << std::endl;
}

void Generator::print_all_current_cycle_states(std::ostream& out)
{
	decltype(auto) prev_Q = _Q;
	decltype(auto) prev_even_flag = _even_iteration;

	reset();
	print_state(out);
	while (generate() != _start_Q) {
		print_state(out);
	}
	_Q = prev_Q;
	_even_iteration = prev_even_flag;
}

std::vector<std::vector<std::vector<GFElement>>> Generator::generate_all_cycles()
{
	decltype(auto) prev_Q = _Q;
	decltype(auto) prev_even_flag = _even_iteration;
	decltype(auto) all_field_polynomials = _field.all_field_elements(); 

	std::set<std::vector<GFElement>> all_polynomials_combinations_set;

	{
		auto combination = std::vector<GFElement>(_N, all_field_polynomials.front());
		auto max_combination = std::vector<GFElement>(_N, all_field_polynomials.back());

		while (combination != max_combination) {
			all_polynomials_combinations_set.insert(combination);
			for (auto&& q : combination) {
				if (q == all_field_polynomials.back()) q = all_field_polynomials.front();
				else {
					q = all_field_polynomials[q.to_int() + 1];
					break;
				}
			}
		}
		all_polynomials_combinations_set.insert(combination);
	}


	std::vector<std::vector<std::vector<GFElement>>> all_cycles;

	while (!all_polynomials_combinations_set.empty()) {
		auto start_Q = *all_polynomials_combinations_set.begin();
		_Q = start_Q;
		_even_iteration = false;
		all_cycles.emplace_back(std::vector<std::vector<GFElement>>());
		decltype(auto) current_cycle = all_cycles.back();
		current_cycle.emplace_back(_Q);
		all_polynomials_combinations_set.erase(_Q);
		while (generate() != start_Q) {
			check_endless_loop(_Q);
			current_cycle.emplace_back(_Q);
			all_polynomials_combinations_set.erase(_Q);
		}
	}

	_Q = prev_Q;
	_even_iteration = prev_even_flag;
	return all_cycles;
}

void Generator::print_all_cycles(std::ostream& out, bool show_states)
{
	out << "n = " << _field.dimension() << ", N = " << _N << std::endl;
	auto print_ch = [&out](std::string name, const std::vector<GFElement>& elements) {
		out << name << " -> { ";
		auto it = elements.begin();
		out << (*it++).to_int();
		for (; it != elements.end(); ++it) {
			out << "," << (*it).to_int();
		}
		out << " }" << std::endl;
	};
	print_ch("even A", _even_A);
	print_ch(" odd A", _odd_A);
	print_ch("even C", _even_C);
	print_ch(" odd C", _odd_C);
	out << std::endl;
	decltype(auto) all_cycles = generate_all_cycles();
	Int cycle_counter = 0;
	for (auto&& cycle : all_cycles) {
		++cycle_counter;
		out << "Cycle -> " << cycle_counter << std::endl;
		out << "States count -> " << cycle.size() << std::endl;
		if (show_states) {
			for (auto&& combination : cycle) {
				for (auto&& q : combination) {
					out << q.to_int() << " ";
				}
				out << std::endl;
			}
		}
		out << std::endl;
	}
}

GFElement Generator::field_add(const GFElement& left, const GFElement& right)
{
	return left + right;
}

GFElement Generator::mod_add(const GFElement& left, const GFElement& right)
{
	return left.mod_add(right);
}

void Generator::set_even_iteration_add(decltype(&mod_add) add)
{
	_even_iteration_add = add;
}

void Generator::set_odd_iteration_add(decltype(&mod_add) add)
{
	_odd_iteration_add = add;
}

void Generator::set_even_A(std::vector<GFElement> A)
{
	if (A.size() != _N) {
		throw std::exception("dimensions of the vectors do not match each other");
	}
	_even_A = A;
}

void Generator::set_even_C(std::vector<GFElement> C)
{
	if (C.size() != _N) {
		throw std::exception("dimensions of the vectors do not match each other");
	}
	_even_C = C;
}

void Generator::set_odd_A(std::vector<GFElement> A)
{
	if (A.size() != _N) {
		throw std::exception("dimensions of the vectors do not match each other");
	}
	_odd_A = A;
}

void Generator::set_odd_C(std::vector<GFElement> C)
{
	if (C.size() != _N) {
		throw std::exception("dimensions of the vectors do not match each other");
	}
	_odd_C = C;
}
