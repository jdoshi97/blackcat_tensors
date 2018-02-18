/*
 * Tensor_Base.h
 *
 *  Created on: Jan 6, 2018
 *      Author: joseph
 */

#ifndef TENSOR_BASE_H_
#define TENSOR_BASE_H_


#include "Implementation_Core/Tensor_Operations.h"
#include "Implementation_Core/Tensor_Utility.h"
#include "Implementation_Core/Tensor_Core.h"
#include "../BC_MathLibraries/Mathematics_CPU.h"
#include "../BC_MathLibraries/Mathematics_GPU.cu"


namespace BC {

using MTF::prim; //isPrimitive
using MTF::ifte; //if then else

template<class T, class derived, class Mathlib, class voider = void>
class TensorBase :
				public Tensor_Operations <T, ifte<prim<T>, Tensor_Core<T, Mathlib, derived>, T>, derived, Mathlib>,
				public Tensor_Utility    <T, derived, Mathlib, prim<T>>

{

protected:
	using math_parent  = Tensor_Operations<T, ifte<prim<T>, Tensor_Core<T, Mathlib, derived>, T>, derived, Mathlib>;
	using functor_type =  ifte<prim<T>, Tensor_Core<T, Mathlib, derived>, T>;
	functor_type black_cat_array;

public:
	template<class... params>
	TensorBase(const params&... p) : black_cat_array(p...) {}
	TensorBase(		 derived&& tensor) : black_cat_array(tensor.black_cat_array){}
	TensorBase(const derived&  tensor) : black_cat_array(tensor.black_cat_array){}

	using math_parent::operator=;

	operator  const derived&() const { return static_cast<const derived&>(*this); }
	operator  		derived&() 		 { return static_cast<	    derived&>(*this); }

	int rank() const { return black_cat_array.rank(); }
	int size() const { return black_cat_array.size(); }
	int rows() const { return black_cat_array.rows(); }
	int cols() const { return black_cat_array.cols(); }
	int LD_rows() const { return black_cat_array.LD_rows(); }
	int LD_cols() const { return black_cat_array.LD_cols(); }

	int dimension(int i)		const { return black_cat_array.dimension(i); }
	void printDimensions() 		const { black_cat_array.printDimensions();   }
	void printLDDimensions()	const { black_cat_array.printLDDimensions(); }

	const int* InnerShape() const 			{ return black_cat_array.InnerShape(); }
	const int* OuterShape() const 			{ return black_cat_array.OuterShape(); }
	const functor_type& _data() const { return black_cat_array; }
		  functor_type& _data()		  { return black_cat_array; }
};

}


#endif /* TENSOR_BASE_H_ */

