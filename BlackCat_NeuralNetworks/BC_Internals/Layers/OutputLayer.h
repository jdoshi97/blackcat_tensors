/*
 * FeedForward.cu
 *
 *  Created on: Jan 28, 2018
 *      Author: joseph
 */

#ifndef OUTPUTas_CU
#define OUTPUTas_CU

#include "../../BC_Internals/Layers/Layer_Base.h"

namespace BC {
namespace NN {

struct OutputLayer : Layer_Base {

	mat_view x;

public:

	OutputLayer(int inputs) : Layer_Base(inputs, inputs) {}

	const auto& forward_propagation() {
		return x;
	}
	auto forward_propagation_express() {
		return x;
	}
	template<class t>
	auto back_propagation(const expr::mat<t>& exp) {
		return x - exp;
	}


	void set_batch_size(int) {}
	void update_weights() {}
	void clear_stored_gradients() {}
	void write(std::ofstream& is) {}
	void read(std::ifstream& os) {}
	void set_learning_rate(fp_type learning_rate) {}

	auto& outputs() { return x; }
	auto& inputs()  { return x; }
	auto& deltas()  { return NULL_TENSOR; }
	auto& weights()	{ return NULL_TENSOR; }
	auto& bias()	{ return NULL_TENSOR; }


	template<class tensor, class deltas> void set_activation(tensor&, deltas&) {}
	template<class tensor> void set_weight(tensor&) {}
	template<class tensor> void set_bias(tensor&) {}
};

}
}



#endif /* FEEDFORWARD_CU_ */
