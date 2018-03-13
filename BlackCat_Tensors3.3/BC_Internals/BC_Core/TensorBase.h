
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
#include "Implementation_Core/Tensor_Initializer.h"
#include "Implementation_Core/Determiners.h"

namespace BC {
template<class,class> class Scalar;

template<class derived>
class TensorBase :
		public Tensor_Operations <derived>,
		public Tensor_Utility<derived>,
		public TensorInitializer<derived>
{

protected:

	template<class> struct DISABLED;

	using self 			= TensorBase<derived>;
	using math_parent  	= Tensor_Operations<derived>;
	using initializer 	= TensorInitializer<derived>;
	using functor_type 	= _functor<derived>;
	using Mathlib 		= _mathlib<derived>;
	using scal			= _scalar<derived>;

	template<class    U> using deriv   = typename shell_of<derived>::template type<U, Mathlib>;
	template<class... U> using functor = typename shell_of<functor_type>::template type<U...>;

	static constexpr int RANK 		= ranker<derived>::type::inner_rank;
	static constexpr int LD_RANK 	= ranker<derived>::type::outer_rank;
	static constexpr bool GENUINE_TENSOR = isCore<_functor<derived>>::conditional;

//	functor_type black_cat_array;

public:
	using math_parent::operator=;
	template<class... params>
	explicit TensorBase(const params&... p) : initializer(p...) {}

	operator const derived& () const { return static_cast<const derived&>(*this); }
	operator	   derived& () 		 { return static_cast< derived&>(*this); }

	derived& operator =(const derived& tensor) {
		this->assert_same_size(tensor);
		Mathlib::copy(this->data(), tensor.data(), this->size());
		return *this;
	}
	derived& operator =(_scalar<derived> scalar) {
		this->fill(scalar);
		return *this;
	}

	int rank() const { return this->black_cat_array.rank(); }
	int size() const { return this->black_cat_array.size(); }
	int rows() const { return this->black_cat_array.rows(); }
	int cols() const { return this->black_cat_array.cols(); }
	int LD_rows() const { return this->black_cat_array.LD_rows(); }
	int LD_cols() const { return this->black_cat_array.LD_cols(); }
	void resetShape(std::vector<int> sh) { this->black_cat_array.resetShape(sh); }

	int dimension(int i)		const { return this->black_cat_array.dimension(i); }
	void printDimensions() 		const { this->black_cat_array.printDimensions();   }
	void printLDDimensions()	const { this->black_cat_array.printLDDimensions(); }

	const auto innerShape() const 			{ return this->black_cat_array.innerShape(); }
	const auto outerShape() const 			{ return this->black_cat_array.outerShape(); }


	const functor_type& _data() const { return this->black_cat_array; }
		  functor_type& _data()		  { return this->black_cat_array; }

private:
	const auto slice(int i) const { return this->black_cat_array.slice(i); }
		  auto slice(int i) 	  { return this->black_cat_array.slice(i); }

	const auto scalar_bit(int i) const { return Tensor_Scalar<functor_type>(&this->data()[i], this->data()); }
		  auto scalar_bit(int i)	   { return Tensor_Scalar<functor_type>(&this->data()[i], this->data()); }

public:
		  auto operator [] (int i) 		 { return typename base<RANK>::template slice<decltype(slice(0)), Mathlib>(slice(i)); }
	const auto operator [] (int i) const { return typename base<RANK>::template slice<decltype(slice(0)), Mathlib>(slice(i)); }


	const auto getScalar(int i) const {
		return base<0>::type<Tensor_Scalar<functor_type>, Mathlib>(scalar_bit(i));
	}

	auto getScalar(int i) {
		return base<0>::type<Tensor_Scalar<functor_type>, Mathlib>(scalar_bit(i));
	}

	const auto operator() (int i) const { return getScalar(i); }
		  auto operator() (int i) 	    { return getScalar(i); }

	const auto operator() () const { return *this; }
		  auto operator() () 	   { return *this; }

	template<class... integers> const auto operator() (int i, integers... ints) const { return (*this)[i](ints...); }
	template<class... integers> 	  auto operator() (int i, integers... ints) 	  { return (*this)[i](ints...); }

	template<class var>
	static std::vector<int> shapeOf(const var& v) {
		std::vector<int> sh(v.rank());
		for (int i = 0; i < v.rank(); ++i) {
			sh[i] = v.dimension(i);
		}
		return sh;
	}
};

}


#endif /* TENSOR_BASE_H_ */

