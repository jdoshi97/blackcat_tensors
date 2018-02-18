/*
 * Transpose_Wrapper.h
 *
 *  Created on: Dec 19, 2017
 *      Author: joseph
 */
#ifdef  __CUDACC__
#ifndef EXPRESSION_UNARY_MATRIXTRANSPOSITION_H_
#define EXPRESSION_UNARY_MATRIXTRANSPOSITION_H_
#include <iostream>
#include <cmath>
#include "Expression_Base.cu"
#include "../BC_MetaTemplateFunctions/Adhoc.h"
#include "BlackCat_Internal_Definitions.h"

namespace BC {
template<class T, class functor_type>
struct unary_expression_transpose : expression<T, unary_expression_transpose<T, functor_type>>
{

	const functor_type& array;
	const bool vector = rows() == 1 || cols() == 1;

	__BC_gcpu__ int rows() const { return array.cols(); }
	__BC_gcpu__ int cols() const { return array.rows(); }
	__BC_gcpu__ int size() const { return array.size(); }
	__BC_gcpu__ int LD_rows() const { return array.LD_rows(); }
	__BC_gcpu__ auto data() const -> decltype(array.data()) { return array.data(); }
	__BC_gcpu__ auto data() -> decltype(array.data()) { return array.data(); }

	void printDimensions() const {
		std::cout << "[" << rows() << "]" << "[" << cols() << "]" << std::endl;
	}
	unary_expression_transpose(const functor_type& p)
	: array(p) {}



private:
	template<class U> __BC_gcpu__
	static int bc_floor(U number) {
		return ((int)number) > number ? (int)(number - .5) : number; //if casting the number to an integer is greater (if it rounded up) subtract .5 and the round again
	}

public:


	__attribute__((always_inline)) __BC_gcpu__  auto operator [](int index) -> decltype(array[index]) {


		if (vector)
			return array[index];
		else
			return array[(int)(index / rows()) + (index % rows()) * LD_rows()];
	}
	__attribute__((always_inline))  __BC_gcpu__ auto operator[](int index) const  -> const decltype(array[index])  {

		if (vector)
			return array[index];
		else
			return array[(int)(index / rows()) + (index % rows()) * LD_rows()];
	}
};
}
#endif /* EXPRESSION_UNARY_MATRIXTRANSPOSITION_H_ */
#endif
