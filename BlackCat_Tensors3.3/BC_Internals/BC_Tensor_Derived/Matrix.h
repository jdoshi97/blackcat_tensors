/*
 * Matrix.h
 *
 *  Created on: Dec 30, 2017
 *      Author: joseph
 */

#ifndef BC_MATRIX_H
#define BC_MATRIX_H
#include "BC_Tensor_Base/TensorBase.h"

namespace BC {

template<class T, class Mathlib>
class Matrix : public TensorBase<Matrix<T, Mathlib>> {

	using parent_class = TensorBase<Matrix<T, Mathlib>>;

public:
	__BCinline__ static constexpr int DIMS() { return 2; }

	using parent_class::operator=;
	using parent_class::operator[];
	using parent_class::operator();

	explicit Matrix(int rows, int cols = 1) : parent_class(std::vector<int> {rows, cols}) {}
	Matrix(const Matrix&  v) : parent_class(v) {}
	Matrix(		 Matrix&& v) : parent_class(v) {}
	Matrix(const Matrix&& v) : parent_class(v) {}

	template<class U> 		  Matrix(const Matrix<U, Mathlib>&  t) : parent_class(t) {}
	template<class U> 		  Matrix(	   Matrix<U, Mathlib>&& t) : parent_class(t) {}
	template<class... params> Matrix(const params&... p) : parent_class(p...) {}

	Matrix& operator =(const Matrix& t)  { return parent_class::operator=(t); }
	Matrix& operator =(const Matrix&& t) { return parent_class::operator=(t); }
	Matrix& operator =(	     Matrix&& t) { return parent_class::operator=(t); }
	template<class U>
	Matrix& operator = (const Matrix<U, Mathlib>& t) { return parent_class::operator=(t); }

	const Matrix<unary_expression_transpose<_scalar<T>, typename parent_class::functor_type>, Mathlib> t() const {
		return Matrix<unary_expression_transpose<_scalar<T>, typename parent_class::functor_type>, Mathlib>(this->data());
	}
};

} //End Namespace BC

#endif /* MATRIX_H */
