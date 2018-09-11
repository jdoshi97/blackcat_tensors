/*
 * Expression_Tree_Functions.h
 *
 *  Created on: Jun 13, 2018
 *      Author: joseph
 */

#ifndef EXPRESSION_TREE_FUNCTIONS_H_
#define EXPRESSION_TREE_FUNCTIONS_H_

#include "Operations/Unary.h"
#include "Operations/Binary.h"
#include "Operations/BLAS.h"
#include "Utility/MetaTemplateFunctions.h"

namespace BC{
namespace internal {
namespace tree {



template<class> struct PRECEDENCE {
	enum traits {
		value = -1,
		alpha_modifier = 0,
		beta_modifier = 0,
		injectable_assignment = false,
		blas_rotation = false,
		injector = false
	};
};
template<> struct PRECEDENCE<oper::add> {
	enum traits {
		value = 0,
		alpha_modifier = 1,
		beta_modifier = 1,
		injectable_assignment = false,
		blas_rotation = true,
		injector = false
	};
};
template<> struct PRECEDENCE<oper::sub> {
	enum traits {
		value = 0,
		alpha_modifier = -1,
		beta_modifier = 1,
		injectable_assignment = false,
		blas_rotation = true,
		injector = false
	};
};
template<> struct PRECEDENCE<oper::mul> {
	enum traits {
		value = 1,
		alpha_modifier = 0,
		beta_modifier = 0,
		injectable_assignment = false,
		blas_rotation = false,
		injector = false
	};
};
template<> struct PRECEDENCE<oper::div> {
	enum traits {
		value = 1,
		alpha_modifier = 0,
		beta_modifier = 0,
		injectable_assignment = false,
		blas_rotation = false,
		injector = false
	};
};
template<> struct PRECEDENCE<oper::add_assign> {
	enum traits {
		value = 0,
		alpha_modifier = 1,
		beta_modifier = 1,
		injectable_assignment = true,
		blas_rotation = false,
		injector = true
	};
};
template<> struct PRECEDENCE<oper::sub_assign> {
	enum traits {
		value = 0,
		alpha_modifier = -1,
		beta_modifier = 1,
		injectable_assignment = true,
		blas_rotation = false,
		injector = true
	};
};
template<> struct PRECEDENCE<oper::assign> {
	enum traits {
		value = 1,
		alpha_modifier = 1,
		beta_modifier = 0,
		injectable_assignment = true,
		blas_rotation = false,
		injector = true
	};
};

template<> struct PRECEDENCE<oper::mul_assign> {
	enum traits {
		value = 1,
		alpha_modifier = 0,
		beta_modifier = 0,
		injectable_assignment = false,
		blas_rotation = false,
		injector = false
	};
};
template<> struct PRECEDENCE<oper::div_assign> {
	enum traits {
		value = 1,
		alpha_modifier = 0,
		beta_modifier = 0,
		injectable_assignment = false,
		blas_rotation = false,
		injector = false
	};
};

template<class T> static constexpr bool is_blas_func() {
	return std::is_base_of<BC::BLAS_FUNCTION, T>::value;
}


template<class T>
static constexpr bool is_linear_op() {
	return MTF::seq_contains<T, oper::add, oper::sub>;
}

template<class T>
static constexpr bool is_nonlinear_op() {
	return  !MTF::seq_contains<T, oper::add, oper::sub> && !is_blas_func<T>();
}
template<class T>
static constexpr bool is_linear_assignment_op() {
	return MTF::seq_contains<T, oper::add_assign, oper::sub_assign>;
}
template<class T>
static constexpr bool is_assignment_op() {
	return MTF::seq_contains<T, oper::assign, oper::add_assign,oper::sub_assign, oper::mul_assign,oper::div_assign>;
}

template<class T>
static constexpr bool is_standard_assignment_op() {
	return MTF::seq_contains<oper::assign>;
}

template<class T>
static constexpr int alpha_of() {
	return PRECEDENCE<std::decay_t<T>>::traits::alpha_modifier;
}
template<class T>
static constexpr int beta_of() {
	return PRECEDENCE<std::decay_t<T>>::traits::beta_modifier;
}
template<class T>
static constexpr int precedence() {
	return PRECEDENCE<std::decay_t<T>>::traits::value;
}

template<class T>
static constexpr int valid_double_inject() {
	return PRECEDENCE<std::decay_t<T>>::traits::blas_rotation;
}

template<class T>
static constexpr bool injectable_assignment() {
	return PRECEDENCE<std::decay_t<T>>::traits::injectable_assignment;
}

//trivial_blas_evaluation -- detects if the tree is entirely +/- operations with blas functions, --> y = a * b + c * d - e * f  --> true, y = a + b * c --> false
template<class op, class core, int a, int b>//only apply update if right hand side branch
auto update_injection(injector<core,a,b> tensor) {
	static constexpr int alpha_modifier = a != 0 ? a * alpha_of<op>() : 1;
	static constexpr int beta_modifier = b != 0 ? b * beta_of<op>() : 1;
	return injector<core, alpha_modifier, beta_modifier>(tensor.data());
}


}
}
}




#endif /* EXPRESSION_TREE_FUNCTIONS_H_ */
