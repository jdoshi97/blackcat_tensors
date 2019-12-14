/*

 * TypeTraits.h
 *
 *  Created on: Jun 30, 2019
 *	  Author: joseph
 */

#ifndef BLACKCAT_TYPETRAITS_H_
#define BLACKCAT_TYPETRAITS_H_

#include "Common.h"
#include "ConstexprIf.h"
#include "Bind.h"

namespace BC {

namespace streams {

	//forward declare stream for common_traits
	template<class SystemTag>
	class Stream;
}

namespace traits {

using namespace BC::traits::common;

namespace detail { template<class> struct DISABLE; }

template<bool x,class T>
using only_if = conditional_t<x, T, detail::DISABLE<T>>;

template<int x> struct Integer { static constexpr int value = x; };
template<class T, class... Ts> using front_t = T;

template<class...> using void_t = void;
template<class...> static constexpr bool true_v  = true;
template<class...> static constexpr bool false_v = false;


/**
 * true_call and false_call are
 * CUDA-supported alternatives for true_v and false_v
 * -- CUDA does not support using 'host_variables' on the device
 *    (including static-constexpr values)
 */
template<class T>
BCINLINE static constexpr bool true_call() { return true; }

template<class T>
BCINLINE static constexpr bool false_call() { return false; }

template<class...> using void_t = void;
template<class...> using true_t  = true_type;
template<class...> using false_t = false_type;

template<bool cond>
using truth_type = conditional_t<cond, true_type, false_type>;

template<bool cond>
using not_type = conditional_t<cond, false_type, true_type>;

//--------------------------------
template<class From, class To>
struct propagate_const {
	using type = To;
};

template<class From, class To>
struct propagate_const<const From, To> {
	using type = const To;
};

template<class From, class To>
using propagate_const_t = typename propagate_const<From, To>::type;

//----------------------------------

template<template<class> class func, class T, class voider=void>
struct is_detected: false_type {};

template<template<class> class func, class T>
struct is_detected<func, T, enable_if_t<true_v<func<T>>>>: true_type {};

template<template<class> class func, class T>
static constexpr bool is_detected_v = is_detected<func, T>::value;

//----------------------------------

template<
	template<class> class func,
	class TestType,
	class DefaultType=void,
	class enabler=void>
struct conditional_detected {
	using type = DefaultType;
};

template<
	template<class> class func,
	class TestType,
	class DefaultType>
struct conditional_detected<
		func,
		TestType,
		DefaultType,
		enable_if_t<is_detected_v<func,TestType>>> {

	using type = func<TestType>;
};

template<template<class> class func, class TestType, class DefaultType>
using conditional_detected_t =
		typename conditional_detected<func, TestType, DefaultType>::type;

//----------------------------------


template<class T> BCINLINE
static constexpr BC::size_t  max(const T& x) { return x; }

template<class T> BCINLINE
static constexpr BC::size_t  min(const T& x) { return x; }

template<class T, class... Ts> BCINLINE
static constexpr size_t max(const T& x, const Ts&... xs) {
	return x > max(xs...) ? x : max(xs...);
}

template<class T, class... Ts> BCINLINE
static constexpr size_t min(const T& x, const Ts&... xs) {
	return x < min(xs...) ? x : min(xs...);
}

//----------------------------------

template<template<class> class Function, class... Ts>
struct all: true_type {};

template<template<class> class Function, class T, class... Ts>
struct all<Function, T, Ts...>:
	conditional_t<Function<T>::value, all<Function, Ts...>, false_type> {};

template<template<class> class Function, class... Ts>
static constexpr bool all_v = all<Function, Ts...>::value;

template<template<class> class Function, class... Ts>
struct any: false_type {};

template<template<class> class Function, class T, class... Ts>
struct any<Function, T, Ts...>:
	conditional_t<Function<T>::value, true_type, any<Function, Ts...>> {};

template<template<class> class Function, class... Ts>
static constexpr bool any_v = any<Function, Ts...>::value;

template<template<class> class Function, class... Ts>
struct none:
	conditional_t<any<Function, Ts...>::value, false_type, true_type> {};

template<template<class> class Function, class... Ts>
static constexpr bool none_v = none<Function, Ts...>::value;

// ---------------------

template<class T, class... Ts>
class sequence_of {
	template<class U> using is_same_ = std::is_same<U, T>;
public:
	static constexpr bool value = all_v<is_same_, Ts...>;
};

template<class... Ts>
static constexpr bool sequence_of_v = sequence_of<Ts...>::value;


template<class T, class... Ts>
class sequence_contains {
	template<class U> using is_same_ = std::is_same<U, T>;
public:
	static constexpr bool value = any_v<is_same_, Ts...>;
};

template<class... Ts>
static constexpr bool sequence_contains_v = sequence_contains<Ts...>::value;

// -------- new addition, other code will use static_cast
template<class T>
using query_derived_type = typename T::derived_type;

template<template<class...> class Type, class Derived, class... Ts>
auto& derived_cast(const Type<Derived, Ts...>& t) {
	using derived_type = conditional_detected_t<
			query_derived_type, Type<Derived,Ts...>, Derived>;

	return static_cast<const derived_type&>(t);
}

template<template<class...> class Type, class Derived, class... Ts>
auto& derived_cast(Type<Derived, Ts...>& t) {
	using derived_type = conditional_detected_t<
			query_derived_type, Type<Derived,Ts...>, Derived>;

	return static_cast<derived_type&>(t);
}

template<class Type, class=std::enable_if_t<is_detected_v<query_derived_type, Type>>>
const auto& derived_cast(const Type& t) {
	return static_cast<const typename Type::derived_type&>(t);
}

template<class Type, class=std::enable_if_t<is_detected_v<query_derived_type, Type>>>
auto& derived_cast(Type& t) {
	return static_cast<typename Type::derived_type&>(t);
}

//---------------------
//Only required for NVCC (can't pass std::pair to cuda kernels)
template<class T, class U>
struct Pair {
	T first;
	U second;
};

template<class T, class U> BCINLINE
Pair<T, U> make_pair(T t, U u) { return Pair<T, U>{t, u}; }

//---------------------

template<class T> BCINLINE
T& auto_remove_const(const T& param) {
	return const_cast<T&>(param);
}

template<class T> BCINLINE
T&& auto_remove_const(const T&& param) {
	return const_cast<T&&>(param);
}

template<class T> BCINLINE
T* auto_remove_const(const T* param) {
	return const_cast<T*>(param);
}

template<class T>
using apply_const_t = conditional_t<is_const<T>::value, T, const T>;


template<class T> BCINLINE
apply_const_t<T>& auto_apply_const(T& param) { return param; }
template<class T> BCINLINE
apply_const_t<T>&& auto_apply_const(T&& param) { return param; }
template<class T> BCINLINE
apply_const_t<T>* auto_apply_const(T* param) { return param; }


template<class T> using query_value_type = typename T::value_type;
template<class T> using query_allocator_type = typename T::allocator_type;
template<class T> using query_system_tag = typename T::system_tag;
template<class T> using query_get_stream = decltype(std::declval<T>().get_stream);
template<class T> using query_get_allocator = decltype(std::declval<T>().get_allocator);

class None {};

template<class T>
class common_traits {


	template<template<class> class Query>
	using self_defines = is_detected<Query,T>;

	template<template<class> class Query, class Default=None>
	using type_or_default = traits::conditional_detected_t<Query, T, None>;

public:

	using defines_value_type = self_defines<query_value_type>;
	using defines_allocator_type = self_defines<query_allocator_type>;
	using defines_system_tag = self_defines<query_system_tag>;
	using defines_get_stream = self_defines<query_get_stream>;
	using defines_get_allocator = self_defines<query_get_allocator>;

	using type = T;

	using value_type = type_or_default<query_value_type>;
	using allocator_type = type_or_default<query_allocator_type>;
	using system_tag = type_or_default<query_system_tag>;

	template<class Other>
	using compatible_system_tag_with = std::is_same<system_tag,
			typename common_traits<Other>::system_tag>;

	static auto select_on_get_stream(const T& type)
	{
		return traits::constexpr_ternary<defines_get_stream::value>(
				BC::traits::bind([](const auto& type) {
						return type.get_stream();
				}, type),
				[]() {
					return BC::streams::Stream<system_tag>();
				}
		);
	}

	static auto select_on_get_allocator(const T& type)
	{
		return traits::constexpr_ternary<defines_get_allocator::value>(
				BC::traits::bind([](const auto& type) {
						return type.get_allocator();
				}, type),
				[]() {
					return allocator_type();
				}
		);
	}
};

template<class T>
struct Type : common_traits<T> {};


}

using traits::common_traits; //import common_traits into BC namespace

}


#endif /* TYPETRAITS_H_ */
