/*
 * Matrix.h
 *
 *  Created on: Dec 30, 2017
 *      Author: joseph
 */

#ifndef BC_MATRIX_H
#define BC_MATRIX_H

#include "Vector.h"
#include "TensorBase.h"

namespace BC {
template<class T, class Mathlib>
class Matrix : public TensorBase<Matrix<T, Mathlib>> {

	template<class,class>
	friend class Vector;

	using parent_class = TensorBase<Matrix<T, Mathlib>>;

public:
	using scalar = T;
	using parent_class::operator=;
	using parent_class::operator[];
	using parent_class::operator();

	static constexpr int RANK() { return 2; }

	Matrix(const Matrix&  v) : parent_class(v) {}
	Matrix(		 Matrix&& v) : parent_class(v) {}
	Matrix(const Matrix&& v) : parent_class(v) {}
	explicit Matrix(int rows, int cols = 1) : parent_class(std::vector<int> {rows, cols}) {}

	template<class U> 		  Matrix(const Matrix<U, Mathlib>&  t) : parent_class(t) {}
	template<class U> 		  Matrix(	   Matrix<U, Mathlib>&& t) : parent_class(t) {}
	template<class... params> Matrix(const params&... p) : parent_class(p...) {}

	Matrix& operator =(const Matrix& t)  { return parent_class::operator=(t); }
	Matrix& operator =(const Matrix&& t) { return parent_class::operator=(t); }
	Matrix& operator =(	     Matrix&& t) { return parent_class::operator=(t); }
	template<class U>
	Matrix& operator = (const Matrix<U, Mathlib>& t) { return parent_class::operator=(t); }

	const Matrix<unary_expression_transpose<typename MTF::determine_scalar<T>::type, typename parent_class::functor_type>, Mathlib> t() const {
		return Matrix<unary_expression_transpose<typename MTF::determine_scalar<T>::type, typename parent_class::functor_type>, Mathlib>(this->data());
	}
};

} //End Namespace BC

#endif /* MATRIX_H */
