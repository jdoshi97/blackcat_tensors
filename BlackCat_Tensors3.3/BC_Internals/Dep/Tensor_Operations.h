///*
// * Tensor_Core.h
// *
// *  Created on: Dec 30, 2017
// *      Author: joseph
// */
//
//#ifndef TENSOR_CORE_H_
//#define TENSOR_CORE_H_
//
//#include "Tensor_Operations_impl.h"
//#include "Determiners.h"
//
//namespace BC {
//
//using MTF::is_core;
//using MTF::isPrim;
//
//template<class scalar, class functor, class evaluation, class math_lib>
//struct _TRAITS {
//		using scalar_type 		= scalar;
//		using functor_type 		= functor;
//		using evaluation_type 	= evaluation;
//		using math_library 		= math_lib;
//};
//
////, , derived,
//	template <class derived>
//	struct Tensor_Operations :Tensor_Operations_impl<derived> {
////			:
////			  public Tensor_Operations_impl<
////			  derived,
////			  _TRAITS<
////			  _scalar<derived>,
////			  _functor<derived>,
////			  _evaluation<derived>,
////			  _mathlib<derived>>> {
////
////
////			using parent_class = Tensor_Operations_impl<
////					  derived,
////					  _TRAITS<
////					  _scalar<derived>,
////					  _functor<derived>,
////					  _evaluation<derived>,
////					  _mathlib<derived>>>;
//
//			using parent_class = Tensor_Operations_impl<derived>;
//			using functor_type = typename parent_class::functor_type;
//			using parent_class::operator=;
//
//	};
//
//}
//
//#endif /* TENSOR_CORE_H_ */
