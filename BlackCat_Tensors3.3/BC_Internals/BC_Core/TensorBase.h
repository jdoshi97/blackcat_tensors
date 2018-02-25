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
#include "Implementation_Core/Tensor_Core.cu"

namespace BC {

using MTF::ifte;		//if then else
using MTF::isPrim;
using MTF::shell_of;
using MTF::isCore;

template<class,class> class DISABLED;
template<class,class> class Scalar;
template<class,class> class Vector;
template<class,class> class Matrix;
template<class,class> class Cube;

template<int> struct base;

template<> struct base<0> { template<class t, class m> using type = Scalar<t,m>;  template<class t, class m> using slice = DISABLED<t, m>; };
template<> struct base<1> { template<class t, class m> using type = Vector<t, m>; template<class t,class m> using slice = Scalar<t, m>; };
template<> struct base<2> { template<class t, class m> using type = Matrix<t, m>; template<class t,class m> using slice = Vector<t, m>; };
template<> struct base<3> { template<class t, class m> using type = Cube<t, m>;   template<class t,class m> using slice = Matrix<t, m>; };


template<class T, class derived, class Mathlib, class R>
class TensorBase :
				public Tensor_Operations <T, ifte<isPrim<T>, Tensor_Core<T, Mathlib, R>, T>, derived, Mathlib>,
				public Tensor_Utility    <T, derived, Mathlib, isPrim<T> || isCore<T>::conditional>

{

protected:
	static constexpr int RANK = R::inner_rank;
	static constexpr int LD_RANK = R::outer_rank;
	using self = TensorBase<T, derived, Mathlib, R>;
	using math_parent  = Tensor_Operations<T, ifte<isPrim<T>, Tensor_Core<T, Mathlib, R>, T>, derived, Mathlib>;
	using _functor_type = ifte<isPrim<T>, Tensor_Core<T, Mathlib, R>, T>;
	using functor_type =  _functor_type;

	template<class> struct DISABLED;
	static constexpr bool GENUINE_TENSOR = isPrim<T> || isCore<T>::conditional;
	functor_type black_cat_array;

public:
	using math_parent::operator=;
	operator  const derived&() const { return static_cast<const derived&>(*this); }
	operator  		derived&() 		 { return static_cast<	    derived&>(*this); }


	template<class atLeastOneParam, class... params> TensorBase(const atLeastOneParam& alop, const params&... p) : black_cat_array(alop, p...) { }

	template<class    U> using deriv   = typename shell_of<derived>::type<U, Mathlib>;
	template<class... U> using functor = typename shell_of<functor_type>::type<U...>;

	template<class var>
	static std::vector<int> shapeOf(const var& v) {
		std::vector<int> sh(v.rank());
		for (int i = 0; i < v.rank(); ++i){
			sh[i] = v.dimension(i);
		}
		return sh;
	}
	template<class U>
	TensorBase(const deriv<U>& tensor) : black_cat_array(shapeOf(tensor)) {
		this->assert_same_size(tensor);
		Mathlib::copy(this->black_cat_array, tensor.data(), this->size());
	}
	template<class... U> TensorBase(const deriv<functor<U...>>& tensor) : black_cat_array(tensor.black_cat_array) {}

	TensorBase(		 derived&& tensor) : black_cat_array(tensor.black_cat_array){}
	TensorBase(const derived&  tensor) : black_cat_array(tensor.black_cat_array){}

	int rank() const { return black_cat_array.rank(); }
	int size() const { return black_cat_array.size(); }
	int rows() const { return black_cat_array.rows(); }
	int cols() const { return black_cat_array.cols(); }
	int LD_rows() const { return black_cat_array.LD_rows(); }
	int LD_cols() const { return black_cat_array.LD_cols(); }
	void resetShape(std::vector<int> sh) { black_cat_array.resetShape(sh); }

	int dimension(int i)		const { return black_cat_array.dimension(i); }
	void printDimensions() 		const { black_cat_array.printDimensions();   }
	void printLDDimensions()	const { black_cat_array.printLDDimensions(); }

	const auto InnerShape() const 			{ return black_cat_array.InnerShape(); }
	const auto OuterShape() const 			{ return black_cat_array.OuterShape(); }


	const functor_type& _data() const { return black_cat_array; }
		  functor_type& _data()		  { return black_cat_array; }

	derived& operator =(const derived& tensor) {
		this->assert_same_size(tensor);
		Mathlib::copy(this->data(), tensor.data(), this->size());
		return *this;
	}


	const auto slice(int i) const { return black_cat_array.slice(i); }
		  auto slice(int i) 	  { return black_cat_array.slice(i); }

		  auto operator [] (int i) 		 { return typename base<RANK>::slice<decltype(slice(0)), Mathlib>(slice(i)); }
	const auto operator [] (int i) const { return typename base<RANK>::slice<decltype(slice(0)), Mathlib>(slice(i)); }

	const auto& operator() (int i) const { return this->data()[i]; }
		  auto& operator() (int i) 	     { return this->data()[i]; }

};

}


#endif /* TENSOR_BASE_H_ */

