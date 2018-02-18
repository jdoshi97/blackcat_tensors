
/*
 * BC_Expression_Binary_Pointwise_ScalarL.h
 *
 *  Created on: Dec 2, 2017
 *      Author: joseph
 */
#ifdef  __CUDACC__
#ifndef EXPRESSION_BINARY_POINTWISE_SCALAR_H_
#define EXPRESSION_BINARY_POINTWISE_SCALAR_H_

#include "Expression_Base.cu"
#include "BlackCat_Internal_Definitions.h"

namespace BC {

template<class T, class operation, class lv, class rv>
class binary_expression_scalar_L : expression<T,binary_expression_scalar_L<T, operation, lv, rv>> {
public:

	using this_type = binary_expression_scalar_L<T, operation, lv, rv>;

	operation oper;

	const lv& left;
	const rv& right;


	int rows() const { return right.rows(); };
	int cols() const { return right.cols(); };
	int size() const { return right.size(); };
	int LD_rows() const { return right.LD_rows(); }
	void printDimensions() const { right.printDimensions(); }

	inline __attribute__((always_inline))  __BC_gcpu__ binary_expression_scalar_L(const lv& l, const rv& r) : left(l), right(r) {}
	inline __attribute__((always_inline))  __BC_gcpu__ auto operator [](int index) const { return oper(left[0], right[index]); }
};

template<class T, class operation, class lv, class rv>
class binary_expression_scalar_R : expression<T, binary_expression_scalar_R<T, operation, lv, rv>> {
public:

	using this_type = binary_expression_scalar_R<T, operation, lv, rv>;

	operation oper;

	const lv& left;
	const rv& right;
	int rows() const { return left.rows(); };
	int cols() const { return left.cols(); };
	int size() const { return left.size(); };
	int LD_rows() const { return left.LD_rows(); }
	void printDimensions() const { left.printDimensions(); }


	inline __attribute__((always_inline))  __BC_gcpu__ binary_expression_scalar_R(const lv& l, const rv& r) : left(l), right(r) {}
	inline __attribute__((always_inline))  __BC_gcpu__ auto operator [](int index) const { return oper(left[index], right[0]);}
};

template<class T, class operation, class lv, class rv>
class binary_expression_scalar_LR : expression<T, binary_expression_scalar_LR<T, operation, lv, rv>> {
public:
			const lv& left;
			const rv& right;
			operation oper;

			inline __attribute__((always_inline))  __BC_gcpu__ binary_expression_scalar_LR(const lv& l, const rv& r) : left(l), right(r) {}
			inline __attribute__((always_inline))  __BC_gcpu__ auto operator [](int index) const { return oper(left[0], right[0]); }
};
}

#endif /* EXPRESSION_BINARY_POINTWISE_SCALAR_H_ */
#endif
