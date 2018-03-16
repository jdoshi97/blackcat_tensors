#ifndef EXPRESSION_BINARY_DOTPRODUCT_CU_
#define EXPRESSION_BINARY_DOTPRODUCT_CU_

#include "Expression_Base.h"
#include "Expression_Binary_Dotproduct_impl.h"
#include "BlackCat_Internal_Definitions.h"
#include <memory>

namespace BC {

/*
 * a = M x K
 * b = K x N
 * c = M x N
 */
//det_Eval

template<class T, class lv, class rv, class Mathlib>
struct binary_expression_dotproduct : expression<T, binary_expression_dotproduct<T, lv, rv, Mathlib>> {

	using scalar_type = T;
	lv left;
	rv right;

	static constexpr int RANK() { return rv::RANK(); }

	const int M = left.rows();
	const int N = right.cols();
	const int K = left.cols();
	const int LDA = left.LD_rows();
	const int LDB = right.LD_rows();
	const int LDC = M;

	const int lv_size = M * K;
	const int rv_size = N * K;
	const int eval_size = M * N ;
	int is[2] = {M, N};
	int os[2] = {M, N*M};

	static constexpr bool transA = det_eval<lv>::transposed;
	static constexpr bool transB = det_eval<rv>::transposed;
	static constexpr bool lv_scalar = det_eval<lv>::scalar;
	static constexpr bool rv_scalar = det_eval<rv>::scalar;
	static constexpr bool lv_eval = det_eval<lv>::evaluate;
	static constexpr bool rv_eval = det_eval<rv>::evaluate;

	struct deleter {
		template<class param>
		void operator () (param& p) {
			Mathlib::destroy(p);
		}
	};

	std::shared_ptr<scalar_type> array;
	scalar_type* array_ptr;

	__attribute__((always_inline))
	binary_expression_dotproduct(lv left, rv right) :
	left(left), right(right), LDA(left.LD_rows()), LDB(right.LD_rows()) {

		Mathlib::initialize(array_ptr,eval_size);
		array = std::shared_ptr<scalar_type>(array_ptr, deleter());
		eval();
	}
		__BCinline__ const T& operator [](int index) const {
			return array_ptr[index];
		}
		__BCinline__ T& operator [](int index) {
			return array_ptr[index];
		}

	__BCinline__ int size() const { return eval_size;}
	__BCinline__ int rows() const { return M;}
	__BCinline__ int cols() const { return N;}
	__BCinline__ int rank() const { return right.rank(); }
	__BCinline__ int LD_rows() const { return M; }
	__BCinline__ int LD_cols() const { return eval_size; }
	__BCinline__ int dimension(int i)		const { return i== 0 ? M : i == 1 ? N : 1; }
	__BCinline__ const auto innerShape() 	const { return is; }
	__BCinline__ const auto outerShape() 	const { return os; }



	void printDimensions() 		const { std::cout<<"[" << M << "][" <<N  <<"]" << std::endl; }
	void printLDDimensions()	const { std::cout<<"[" << M << "][" <<eval_size  <<"]" << std::endl; }


public:

	void eval() {

		T* A = nullptr;
		T* B = nullptr;
		T* alpha = nullptr;
		T* alpha2 = nullptr;

		if (lv_eval) {
			Mathlib::initialize(A, lv_size);
			Mathlib::copy(A, left, lv_size);
		} else {
			A = det_eval<lv>::getArray(left);
		}
		if (rv_eval) {
			Mathlib::initialize(B, rv_size);
			Mathlib::copy(B, right, rv_size);
		} else {
			B = det_eval<rv>::getArray(right);
		}
			alpha = det_eval<lv>::getScalar(left);		//nullptr if !lv_scalar
			alpha2 = det_eval<rv>::getScalar(right);	//nullptr if !rv_scalar


			//if scalars on both sides we need to convert them into a single scalar (for it to work with blas)
		if (lv_scalar && rv_scalar){
			T* tmp;
			Mathlib::initialize(tmp, 1);
			Mathlib::scalarMul(tmp, alpha, alpha2);
			Mathlib::MatrixMul(transA, transB, A, B, array_ptr, M, N, K, tmp, nullptr, LDA, LDB, LDC);
			Mathlib::destroy(tmp);

		} else if (rv_scalar)
			 Mathlib::MatrixMul(transA, transB, A, B, array_ptr, M, N, K, alpha2, nullptr, LDA, LDB, LDC);
		 else if (lv_scalar)
			 Mathlib::MatrixMul(transA, transB, A, B, array_ptr, M, N, K, alpha, nullptr, LDA, LDB, LDC);
		 else
			 Mathlib::MatrixMul(transA, transB, A, B, array_ptr, M, N, K, nullptr, nullptr, LDA, LDB, LDC);

		if (lv_eval)
			Mathlib::destroy(A);
		if (rv_eval)
			Mathlib::destroy(B);

	}
	};
}

#endif /* EXPRESSION_BINARY_DOTPRODUCT_CU_ */