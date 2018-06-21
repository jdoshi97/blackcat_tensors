
#ifndef EXPRESSION_BINARY_DOTPRODUCT_CU_
#define EXPRESSION_BINARY_DOTPRODUCT_CU_


#include "Array_Base.h"
#include "Expression_Base.h"
#include "BlackCat_Internal_Definitions.h"
#include "Parse_Tree_BLAS_Branch_Evaluator.h"
#include "Parse_Tree_Evaluator.h"

namespace BC {
namespace oper {
template<class ml> class dotproduct : public BLAS_FUNCTION {};
}
namespace internal {

/*
 * a = M x K
 * b = K x N
 * c = M x N
 */


template<class lv, class rv, class mathlib>
struct binary_expression<lv, rv, oper::dotproduct<mathlib>>
: expression_base<binary_expression<lv, rv,  oper::dotproduct<mathlib>>>, BLAS_FUNCTION {

	using scalar_type = _scalar<lv>;

	static_assert(std::is_same<_scalar<lv>, _scalar<rv>>::value, "MATRIX MULTIPLICATION ONLY AVAILABLE TO SAME TYPE TENSORS (FLOAT/DOUBLE)");

	__BCinline__ static constexpr int DIMS() { return rv::DIMS(); }
	__BCinline__ static constexpr int ITERATOR() { return 0; }

	lv left;
	rv right;

	 binary_expression(lv left, rv right) : left(left), right(right) {}

	__BCinline__ const auto inner_shape() const { return l_array([&](int i) { return i == 0 ? left.rows() : i == 1 ? right.cols() : 1; }); }
	__BCinline__ const auto outer_shape() const { return l_array([&](int i) { return i == 0 ? left.rows() : i == 1 ? right.cols() * left.rows() : 1; }); }

	__BCinline__ int M() const { return left.rows();  }
	__BCinline__ int N() const { return right.cols(); }
	__BCinline__ int K() const { return left.cols();  }



template<class core, int alpha_mod, int beta_mod>
void eval(injection_wrapper<core, alpha_mod, beta_mod> injection_values) const {

	static constexpr bool transA = det_eval<lv>::transposed;
	static constexpr bool transB = det_eval<rv>::transposed;
	static constexpr bool lv_scalar = det_eval<lv>::scalar;
	static constexpr bool rv_scalar = det_eval<rv>::scalar;
	static constexpr bool lv_eval = det_eval<lv>::evaluate;
	static constexpr bool rv_eval = det_eval<rv>::evaluate;

	using lv_A = decltype(branched<mathlib, true>::evaluate(det_eval<lv>::get_array(left)));
	using rv_B = decltype(branched<mathlib, true>::evaluate(det_eval<rv>::get_array(right)));

	//get the data of the injection --> injection_wrapper simply stores the alpha/beta scalar modifiers
	auto& injection = injection_values.data();

	//evaluate the left and right branches (computes only if necessary)
	lv_A A = branched<mathlib, true>::evaluate(det_eval<lv>::get_array(left));
	rv_B B = branched<mathlib, true>::evaluate(det_eval<rv>::get_array(right));

	//get the left and right side scalar values
	scalar_type* alpha_lv = det_eval<lv>::get_scalar(left);
	scalar_type* alpha_rv = det_eval<rv>::get_scalar(right);

	//initialize the alpha and beta scalars,
	scalar_type* alpha = mathlib::static_initialize(1, (scalar_type)alpha_mod);
	scalar_type* beta = mathlib::static_initialize(1, (scalar_type)beta_mod);

	//compute the scalar values if need be
	if (lv_scalar)
		mathlib::scalar_mul(alpha, alpha, alpha_lv);
	if (rv_scalar)
		mathlib::scalar_mul(alpha, alpha, alpha_rv);

	//call matrix_mul
	mathlib::gemm(transA, transB, A, B, injection, M(), N(), K(), alpha, beta, left.ld1(), right.ld1(), injection.ld1());


	//destroy all the temporaries
	if (lv_eval) cc(A).destroy();
	if (rv_eval) cc(B).destroy();
	mathlib::destroy(beta);
	mathlib::destroy(alpha);
}
};

}
}
//		if (transA)
//		std::cout << "A is transposed" << transA << std::endl;
//		if (transB)
//		std::cout <<"B is transposed" << transB << std::endl;
//		if (lv_scalar)
//		std::cout << "A has scalar " <<lv_scalar << std::endl;
//		if (rv_scalar)
//		std::cout <<"B has scalar" << rv_scalar << std::endl;
//		if (lv_eval)
//		std::cout << "A instant eval" <<lv_eval << std::endl;
//		if(rv_eval)
//		std::cout <<"B instant eval " << rv_eval << std::endl;

#endif /* EXPRESSION_BINARY_DOTPRODUCT_CU_ */
