/*
 * Tensor_Initializer.h
 *
 *  Created on: Mar 3, 2018
 *      Author: joseph
 */

#ifndef TENSOR_INITIALIZER_H_
#define TENSOR_INITIALIZER_H_

#include "BC_Internal_Types/Array.h"
#include "BC_Internal_Types/Array_Scalar.h"
#include "BC_Internal_Types/Array_Slice.h"
#include "BC_Internal_Types/Array_RowVector.h"
#include "BC_Internal_Types/Array_Chunk.h"
#include "BC_Internal_Types/Array_Reshape.h"

namespace BC {
namespace Base {
//-------------------------------------SPECIALIZATION FOR EXPRESSION TENSORS OR TENSORS OF NON_OWNERSHIP/CREATION-------------------------------------//
template<class derived, class expression_tensor = void>
class Tensor_Initializer :  public _functor<derived> {

	using self 			= Tensor_Initializer<derived>;

	using parent 		= _functor<derived>;
	using mathlib_t 	= _mathlib<derived>;
	using scalar		= _scalar<derived>;

public:

	 Tensor_Initializer(const  derived&  tensor) : parent(		    tensor.internal()){}
	 Tensor_Initializer(	   derived&& tensor) : parent(std::move(tensor.internal())){}

	template<class... params> explicit Tensor_Initializer(const  params&... p) : parent(p...) {}

	 const parent& internal() const { return static_cast<const parent&>(*this); }
	 	   parent& internal() 	  	{ return static_cast<	   parent&>(*this); }


	~Tensor_Initializer() {
		internal().destroy();
	}
};
//-------------------------------------SPECIALIZATION FOR TENSORS THAT CONTROL / DELETE THEIR ARRAY-------------------------------------//


template<class derived>
struct Tensor_Initializer<derived, std::enable_if_t<is_array_core<_functor<derived>>()>> : public  _functor<derived> {

	using self 		= Tensor_Initializer<derived, std::enable_if_t<is_array_core<_functor<derived>>()>>;

	using parent 		= _functor<derived>;
	using mathlib_t 	= _mathlib<derived>;
	using scalar		= _scalar<derived>;

private:

	auto& as_derived() 			   { return static_cast<	  derived&>(*this); }
	const auto& as_derived() const { return static_cast<const derived&>(*this); }

public:

	 const parent& internal() const { return static_cast<const parent&>(*this); }
	 	   parent& internal() 	  	{ return static_cast<	   parent&>(*this); }

	Tensor_Initializer(derived&& tensor) : parent(std::move(tensor.internal())) { tensor.internal().array 	= nullptr; }

	Tensor_Initializer(const derived& tensor) : parent(tensor.inner_shape()) {
		this->as_derived() = tensor;
	}
	template<class T>
	Tensor_Initializer(T dimensions): parent(dimensions) {}

	template<class T> using derived_alt = typename MTF::shell_of<derived>::template  type<T, mathlib_t>;

	template<class U>
	Tensor_Initializer(const derived_alt<U>&  tensor) : parent(tensor.inner_shape()) {
		this->as_derived() = tensor;
	}

	~Tensor_Initializer() {
		internal().destroy();
	}
};
}
}



#endif /* TENSOR_INITIALIZER_H_ */
