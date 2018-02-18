/*
 * Expression_Binary_Dotproduct.cu
 *
 *  Created on: Jan 9, 2018
 *      Author: joseph
 */
#ifdef  __CUDACC__
#ifndef EXPRESSION_BINARY_DOTPRODUCT_CU_
#define EXPRESSION_BINARY_DOTPRODUCT_CU_

#include "Expression_Binary_Dotproduct_impl.cu"
#include "Expression_Base.cu"
#include "BlackCat_Internal_Definitions.h"
#include "../BC_MetaTemplateFunctions/Adhoc.h"
#include <iostream>
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
	using scalar_type = typename MTF::determine_scalar<T>::type;

	const lv& left;
	const rv& right;

	const int M = left.rows();
	const int N = right.cols();
	const int K = left.cols();
	const int LDA;// = left.LD_rows();
	const int LDB;// = right.LD_rows();
	const int LDC = M;

	static constexpr bool transA = det_eval<lv>::transposed;
	static constexpr bool transB = det_eval<rv>::transposed;
	static constexpr bool lv_scalar = det_eval<lv>::scalar;
	static constexpr bool rv_scalar = det_eval<rv>::scalar;
	static constexpr bool lv_eval = det_eval<lv>::evaluate;
	static constexpr bool rv_eval = det_eval<rv>::evaluate;

	const int lv_size = M * K;
	const int rv_size = N * K;
	const int eval_size = M * N ;

	int size() const { return eval_size;}
	int rows() const { return M;}
	int cols() const { return N;}

	void printDimensions() const {
		std::cout << "dotproduct dim: " << std::endl;
		left.printDimensions();
		right.printDimensions();
	}

	std::shared_ptr<scalar_type> array;
	scalar_type* array_ptr = array.get();

	__attribute__((always_inline))
	binary_expression_dotproduct(const lv& left, const rv& right) :
	left(left), right(right), LDA(left.LD_rows()), LDB(right.LD_rows()) {

		Mathlib::initialize(array_ptr,eval_size);
		array = std::shared_ptr<scalar_type>(array_ptr);
		eval();
	}


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
		if (lv_scalar) {
			alpha = det_eval<lv>::getScalar(left);
		}
		if (rv_scalar) {
			alpha2 = det_eval<rv>::getScalar(right);
		}

		if (lv_scalar && rv_scalar){
			T* tmp;
			Mathlib::initialize(tmp, 1);
			Mathlib::scalarMul(tmp, alpha, alpha2);
			Mathlib::MatrixMul(transA, transB, A, B, array_ptr, M, N, K, tmp, nullptr, LDA, LDB, LDC);
			Mathlib::destroy(tmp);

		} else if (rv_scalar)
			 Mathlib::MatrixMul(transA, transB, A, B, array_ptr, M, N, K, alpha2, nullptr, LDA, LDB, LDC);
		 else
			 Mathlib::MatrixMul(transA, transB, A, B, array_ptr, M, N, K, nullptr, nullptr, LDA, LDB, LDC);

		if (!lv_eval)
			Mathlib::destroy(A);
		if (!rv_eval)
			Mathlib::destroy(B);

	}

		__attribute__((always_inline))  __BC_gcpu__
		 const T operator [](int index) const {
			return array_ptr[index];
		}
		__attribute__((always_inline))  __BC_gcpu__
			T operator [](int index) {
			return array_ptr[index];
		}
	};
}

#endif /* EXPRESSION_BINARY_DOTPRODUCT_CU_ */
#endif


//				Some printouts for debugging
//
//		std::cout << "dotproduct stats --------------------------------------------------------------------------" << std::endl;
//				std::cout << " m n k = " << M << "  "<< N << " " << K << std::endl;
//
//				if (lv_needs_to_be_evaluated) {
//					if (self_eval<lv>::conditioanl) {
//						std::cout << " lv self eval " << std::endl;
//					}
//					std::cout << " lv was evaluated " << std::endl;
//				}
//				if (rv_needs_to_be_evaluated) {
//					if (self_eval<rv>::conditioanl) {
//						std::cout << " rv self eval " << std::endl;
//					}
//					std::cout << " lv was evaluated " << std::endl;
//				}
//				if (transA) {
//					std::cout << " A - fast trans " << std::endl;
//				}
//				if (transB) {
//					std::cout << " B - fast trans " << std::endl;
//				}
//				if (evaluate<lv>::scalar) {
//					std::cout << " lv scalar detected " << std::endl;
//				}
//				if (evaluate<rv>::scalar) {
//					std::cout << " rv scalar detected " << std::endl;
//				}
//				if (scal_A && scal_B)
//				std::cout << "scalars = " << *scal_A <<  " " << *scal_B << std::endl;
//				std::cout << " --------------------------------------------------------------------------" << std::endl;


