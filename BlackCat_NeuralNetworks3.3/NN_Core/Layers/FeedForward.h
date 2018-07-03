/*
 * FeedForward.cu
 *
 *  Created on: Jan 28, 2018
 *      Author: joseph
 */

#ifndef FEEDFORWARD_CU_
#define FEEDFORWARD_CU_

#include "Layer.h"
#include <mutex>

namespace BC {
namespace NN {

template<class derived>
struct FeedForward : public Layer<derived> {
public:

	using Layer<derived>::lr;					//the learning rate

	mat w_gradientStorage;		//weight gradient storage
	vec b_gradientStorage;		//bias gradient storage

	mat y;							//outputs

	mat w;							//weights
	vec b;							//biases
	mat& x = this->prev().y;

	FeedForward(int inputs) :
			Layer<derived>(inputs),
			w(this->OUTPUTS, inputs),
			b(this->OUTPUTS),

			w_gradientStorage(this->OUTPUTS, this->INPUTS),
			b_gradientStorage(this->OUTPUTS)
	{
		w.randomize(-2, 2);
		b.randomize(-1, 1);
		init_storages();
	}

	template<class expr> auto forward_propagation(const f_mat<expr>& x) {
		y = g(w * x + b);
		return this->next().forward_propagation(y);
	}
	template<class expr> auto back_propagation(const f_mat<expr>& dy_) {
		mat dy = dy_; //cache the values (avoid recomputing dy_)

		w_gradientStorage -= dy * x.t();
		b_gradientStorage -= dy;

		return this->prev().back_propagation(w.t() * dy % gd(x));
	}
	template<class expr> auto forward_propagation_express(const f_mat<expr>& x) const {
		return this->next().forward_propagation_express(g(w * x + b));
	}

	void update_weights() {
		w += w_gradientStorage * lr;
		b += b_gradientStorage * lr;

		this->next().update_weights();
	}

	void clear_stored_delta_gradients() {
		w_gradientStorage = 0;	//gradient lists
		b_gradientStorage = 0; //.for_each(zero);	//gradient list

		this->next().clear_stored_delta_gradients();
	}

	void set_batch_size(int x) {
		y = mat(this->OUTPUTS, x);
		this->next().set_batch_size(x);
	}

	void write(std::ofstream& os) {
		w.write(os);
		b.write(os);

		this->next().write(os);
	}
	void read(std::ifstream& is) {
		w.read(is);
		b.read(is);

		this->next().read(is);
	}
	void init_storages() {
		w_gradientStorage = mat(this->OUTPUTS, this->INPUTS);
		b_gradientStorage = vec(this->OUTPUTS);

		w_gradientStorage = 0;
		b_gradientStorage = 0;
	}

};
}
}

#endif /* FEEDFORWARD_CU_ */
