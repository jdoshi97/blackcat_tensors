/*
 * BC_Expression_Binary.h
 *
 *  Created on: Dec 1, 2017
 *      Author: joseph
 */
#ifndef EXPRESSION_BINARY_POINTWISE_SAME_H_
#define EXPRESSION_BINARY_POINTWISE_SAME_H_

#include "Expression_Base.h"
#include "Expression_Binary_Functors.h"

namespace BC {


template<class T, class op, class lv, class rv>
class binary_expression;

template<class T, class operation, class lv, class rv>
struct binary_expression : public expression<T, binary_expression<T, operation, lv, rv>> {

	operation oper;

	lv left;
	rv right;

	static constexpr int RANK() { return lv::RANK(); }


	template<class L, class R>
	__BCinline__  binary_expression(L l, R r) :
			left(l), right(r) {
	}

	__BCinline__  auto operator [](int index) const -> decltype(oper(left[index], right[index])) {
		return oper(left[index], right[index]);
	}

	__BCinline__  int rank() const { return right.rank(); }
	__BCinline__  int rows() const { return right.rows(); };
	__BCinline__  int cols() const { return right.cols(); };
	__BCinline__  int size() const { return right.size(); };
	__BCinline__  int LD_rows() const { return right.LD_rows(); }
	__BCinline__ int LD_cols() const { return right.LD_cols(); }
	__BCinline__ int dimension(int i)		const { return right.dimension(i); }
	__BCinline__ const auto innerShape() const { return right.innerShape(); }
	__BCinline__ const auto outerShape() const { return right.outerShape(); }

	void printDimensions() 		const { right.printDimensions();   }
	void printLDDimensions()	const { right.printLDDimensions(); }


};


}

#endif /* EXPRESSION_BINARY_POINTWISE_SAME_H_ */

