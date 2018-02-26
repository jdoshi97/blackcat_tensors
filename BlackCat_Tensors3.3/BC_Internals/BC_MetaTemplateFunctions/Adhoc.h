/*
 * BC_MetaTemplate_EssentialMethods.h
 *
 *  Created on: Dec 11, 2017
 *      Author: joseph
 */

#ifndef ADHOC_H_
#define ADHOC_H_

#include "Simple.h"
#include <type_traits>

namespace BC {
	namespace MTF {

		//EXPRESSION_TYPE----------------------------------------------------------------------------------------------
		template<class, class>  struct expression_substitution;

		template<class sub, template<class, class> class derived, class scalar_type, class ml>
		struct expression_substitution<sub, derived<scalar_type, ml>>{
				using type = derived<sub, ml>;
		};
		//SCALAR_TYPE----------------------------------------------------------------------------------------------
		template<class T>
		struct determine_scalar {
				using type = T;
		};
		template<template<class...> class tensor, class T, class... set>
		struct determine_scalar<tensor<T, set...>> {
				using type = typename determine_scalar<T>::type;
		};
		template<class front, class... T> struct determine_mathlib_impl {
			using type = typename determine_mathlib_impl<T...>::type;
		};
		template<class front> struct determine_mathlib_impl<front> {
			using type = front;
		};
		template<class> struct determine_mathlib;
		template<template<class...> class tensor, class... traits> struct determine_mathlib<tensor<traits...>> {
			using type = typename determine_mathlib_impl<traits...>::type;
		};


		//DETERMINE IF ITS A CORE_TYPE (DETERMINE IF THE METHOD SHOULD GAIN ACCESS TO UTILITY METHODS)-------------------------------------

		template<class,class,class> struct Tensor_Core;
		template<class> struct Tensor_Slice;

		template<class T> 	struct isCore
		{ static constexpr bool conditional = MTF::isPrimitive<T>::conditional; };
		template<class a, class b, class c>
		struct isCore<Tensor_Core<a, b, c>> { static constexpr bool conditional = true;  };
		template<class a> struct isCore<Tensor_Slice<a>> { static constexpr bool conditional = true;  };

		template<class T> constexpr bool is_core = isCore<T>::conditional;

	}
}
#endif /* ADHOC_H_ */
