/*
 * Tensor_Core.h
 *
 *  Created on: Dec 30, 2017
 *      Author: joseph
 */

#ifndef TENSOR_CORE_H_
#define TENSOR_CORE_H_

#include "Tensor_Operations_impl.h"

namespace BC {

template<class,class,class> class Tensor_Core;
using MTF::ifte;
using MTF::prim;
using MTF::same;

template<class scalar, class functor, class evaluation, class math_lib>
struct _TRAITS {
		using scalar_type 		= scalar;
		using functor_type 		= functor;
		using evaluation_type 	= evaluation;
		using math_library 		= math_lib;
};


	template <class T, class functor, class derived, class mathlibrary>
	struct Tensor_Operations
			:
			  public Tensor_Operations_impl<
			  derived,
			  _TRAITS<
			  	  typename MTF::determine_scalar<T>::type,
			  	  functor,
			  	  typename MTF::determine_evaluation<derived>::type,
			  	  mathlibrary>> {


			using parent_class = Tensor_Operations_impl<
					  derived,

					  _TRAITS<
					  	  typename MTF::determine_scalar<T>::type,
					  	  functor,
					  	  typename MTF::determine_evaluation<derived>::type,
					  	  mathlibrary>>;

			using functor_type = typename parent_class::functor_type;
			using parent_class::operator=;

	};

}

//template<class> struct shell;
//template<template<class...> class body, class... innards> struct shell<body<innards...>> {
//	template<class... types>
//	using type = body<types...>;
//};
//
//template<class> struct tensorTraits;
//template<template<class,class>class tensor, class T, class ml>
//struct tensorTraits<tensor<T, ml>> {
//	using scalar = typename MTF::determine_scalar<T>::type;
//	using functor = ifte<prim<T>, Tensor_Core<T, ml, tensor<T, ml>>, T>;
//	using evaluation = ifte<same<functor, T>::conditional, typename shell<tensor<T, ml>>::type<scalar, ml>, tensor<T, ml>&>;
//};




#endif /* TENSOR_CORE_H_ */
