/*
 * Tensor_Slice.cu
 *
 *  Created on: Feb 25, 2018
 *      Author: joseph
 */

#ifndef TENSOR_SLICE_CU_
#define TENSOR_SLICE_CU_

#include <vector>
#include "../../BC_MetaTemplateFunctions/Adhoc.h"
#include "../../BC_Expressions/BlackCat_Internal_Definitions.h"
#include "../../BC_Expressions/Expression_Base.cu"
#include "Determiners.h"
#include <iostream>

namespace BC {

#define __BC_gcpu__ __host__ __device__

template<int inner_rank_, int outer_rank_, class voider>
struct Rank {
	static constexpr int inner_rank = inner_rank_;
	static constexpr int outer_rank = outer_rank_;
};

static constexpr int MAXDIM = 100;
static constexpr int MINDIM = 0;
static const int ONE = 1;

static constexpr int max(int a, int b) { return a > b ? a : b; }
static constexpr int min(int a, int b) { return a < b ? a : b; }
static constexpr int higher(int a) { return max(MAXDIM, max(a + 1, MINDIM)); }
static constexpr int lower(int a) { return min(MAXDIM, min(a - 1, MAXDIM)); }



template<class PARENT>
	struct Tensor_Slice {

	using scalar = _scalar<PARENT>;
	using self = Tensor_Slice<PARENT>;
	using slice_type = Tensor_Slice<self>;

	static constexpr int RANK = lower(PARENT::RANK);
	static constexpr int LAST =  lower(PARENT::LAST);

	const PARENT& parent;
	scalar* array_slice;

	operator 	   scalar*()       { return array_slice; }
	operator const scalar*() const { return array_slice; }

	Tensor_Slice(scalar* array, const PARENT& parent_) : array_slice(array), parent(parent_) {}

	__BC_gcpu__ int rank() const { return RANK; }
	__BC_gcpu__ int size() const { return RANK > 0 ? parent.OuterShape()[LAST] : 1;    }
	__BC_gcpu__ int rows() const { return RANK > 0 ? parent.InnerShape()[0] : 1; }
	__BC_gcpu__ int cols() const { return RANK > 1 ? parent.InnerShape()[1] : 1; }
	__BC_gcpu__ int dimension(int i) const { return RANK > i ? parent.InnerShape()[i] : 1; }

	__BC_gcpu__ int LD_rows() const { return RANK > 0 ? parent.OuterShape()[0] : 1; }
	__BC_gcpu__ int LD_cols() const { return RANK > 1 ? parent.OuterShape()[1] : 1; }
	__BC_gcpu__ int LDdimension(int i) const { return RANK > i + 1 ? parent.OuterShape()[i] : 1; }

	void printDimensions() const { parent.printDimensions(); }
	void printLDDimensions() const { parent.printDimensions(); }

	const auto InnerShape() const 			{ return parent.InnerShape(); }
	const auto OuterShape() const 			{ return parent.OuterShape(); }

			  //the outer_shape is formatted as.... [ld_rows][ld_cols][ld_pages] etc... ergo if our current RANK  == 1 our LD_multiplier would be at index -1
			  // all vectors (aside from row vectors which use a specialized class) have increment of 1 -- erho we choose "just i" as the index opposed to multiplying by the outer shape
		const auto slice(int i) const { return Tensor_Slice<self>(&array_slice[RANK == 1 ? i : (parent.os[LAST - 1] * i)], *this); }
			  auto slice(int i) 	  { return Tensor_Slice<self>(&array_slice[RANK == 1 ? i : (parent.os[LAST - 1] * i)], *this); }

				const scalar* core() const { return array_slice; }
					  scalar* core()  	   { return array_slice; }

		__BC_gcpu__ const auto& operator [] (int i) const{ return array_slice[i]; }
		__BC_gcpu__ auto& operator [] (int i)  	 { return array_slice[i]; }

	};


}



#endif /* TENSOR_SLICE_CU_ */
