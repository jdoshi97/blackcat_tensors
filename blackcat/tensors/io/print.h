/*
 * Print.h
 *
 *  Created on: Jul 29, 2019
 *      Author: joseph
 */

#ifndef BLACKCAT_TENSORS_TENSORS_UTILITY_PRINT_H_
#define BLACKCAT_TENSORS_TENSORS_UTILITY_PRINT_H_

#include <string>

namespace bc {
namespace tensors {
namespace io {

struct features
{
	std::size_t precision = 5;
	bool pretty = false;
	bool sparse = false;

	int indent_lvl = 1;
	const char indent_delim = ' ';

	features(std::size_t precision_,
			bool pretty_=true,
			bool sparse_=true):
		precision(precision_),
		pretty(pretty_),
		sparse(sparse_) {}

	features inc_indent() const {
		features copy = *this;
		copy.indent_lvl++;
		return copy;
	}

	std::string indent() const {
		if (pretty)
			return std::string(indent_lvl, indent_delim);
		else
			return "";
	}

	std::string bracket_wrap(const std::string& str) const {
		if (pretty)
			return "[" + str + "]";
		else
			return str;
	}
};

template<class ValueType>
static std::string format_value(const ValueType& s, features f) {
	std::string fstr  = !f.sparse || std::abs(s) > .1 ? std::to_string(s) : "";
	if (fstr.length() < (unsigned)f.precision)
		return fstr.append(f.precision - fstr.length(), ' ');
	else {
		std::string substr = fstr.substr(0, f.precision);
		if (std::find(substr.begin(), substr.end(), '.') != substr.end()) {
			return fstr.substr(0, f.precision);
		} else {
			return fstr;
		}
	}
}

template<class Tensor>
std::string to_string(
		const Tensor& tensor,
		features f,
		bc::traits::Integer<2>)
{
	std::string s = "";
	if (f.pretty)
		s += "[";

	for (bc::size_t m = 0; m < tensor.rows(); ++m) {

		if (f.pretty && m != 0)
			s += f.indent() + '[';
		else if (f.pretty)
			s += '[';

		for (bc::size_t n = 0; n < tensor.cols(); ++n) {
			s += format_value(tensor[n][m].data()[0], f);

			if (n != tensor.cols() - 1)
				s+=", ";
		}

		if (f.pretty)
			s += "]";
		if (m != tensor.rows()-1)
		s += '\n';
	}

	if (f.pretty)
		s += "]";
	return s;
}


template<class Tensor>
std::string to_string(const Tensor& tensor, features f, bc::traits::Integer<1>)
{
	std::string s = "";

	for (bc::size_t m = 0; m < tensor.rows(); ++m) {
		s += format_value(tensor[m].data()[0], f);

		if(m!=tensor.rows()-1)
			s+=", ";
	}

	return f.bracket_wrap(s);
}

template<class Tensor>
std::string to_string(const Tensor& tensor,  features f, bc::traits::Integer<0>)
{
	std::string value = format_value(tensor.data()[0], f);
	return f.pretty
			? "[" + value + "]"
			: value;
}

template<class Tensor, int X>
std::string to_string(const Tensor& tensor, features f, bc::traits::Integer<X>)
{
	std::string s = "";

	for (auto it = tensor.begin(); it != tensor.end(); ++it) {
		if (it!= tensor.begin())
			s += f.indent();

		s += to_string(*it, f.inc_indent(), bc::traits::Integer<X-1>());

		if (it != tensor.end() - 1)
			s += '\n';
	}
	return f.bracket_wrap(s);
}


}
}
}

#endif /* PRINT_H_ */