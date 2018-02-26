/*
 * FeedForward.cu
 *
 *  Created on: Jan 28, 2018
 *      Author: joseph
 */

#ifndef OUTPUT_CU
#define OUTPUT_CU

#include "Defaults.h"
namespace BC {

template<class derived>
struct InputLayer {


	auto& next() {
		return static_cast<derived&>(*this).next();
	}
	auto& prev() {
		return static_cast<derived&>(*this).prev();
	}
	const auto& next() const {
		return static_cast<derived&>(*this).next();
	}
	const auto& prev() const {
		return static_cast<derived&>(*this).prev();
	}


public:
const int INPUTS;

	InputLayer(int inputs) : INPUTS(inputs) {
	}

	template<class T> vec forwardPropagation(const vec_expr<T>& x) {
		return this->next().forwardPropagation(x);
	}
	template<class T> vec forwardPropagation_Express(const vec_expr<T>& x) const {
		return this->next().forwardPropagation_Express(x);
	}

	template<class T> vec backPropagation(const vec_expr<T>& dy) {
		return dy;
	}

	void updateWeights() {
		next().updateWeights();
	}
	void clearBPStorage() {
		next().clearBPStorage();
	}

	void write(std::ofstream& is) {
		next().write(is);
	}
	void read(std::ifstream& os) {
		next().read(os);
	}

};
}

#include "LayerChain.cu"



#endif /* FEEDFORWARD_CU_ */