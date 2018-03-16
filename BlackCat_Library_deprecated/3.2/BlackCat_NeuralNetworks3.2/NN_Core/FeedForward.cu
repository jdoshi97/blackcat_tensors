/*
 * FeedForward.cu
 *
 *  Created on: Jan 28, 2018
 *      Author: joseph
 */

#ifndef FEEDFORWARD_CU_
#define FEEDFORWARD_CU_

#include "Layer.cu"

namespace BC {

struct FeedForward : Layer<FeedForward> {

public:
	int INPUTS;
	int OUTPUTS;

	 const Scalar<fp_type> lr = 0.03; //fp_type == floating point


	mat w_gradientStorage;
	vec b_gradientStorage;

	mat w;

	vec x;
	vec y;
	vec b;

	vec dx;

	/*
	 * *Note: the operator == represents a delayed evaluation assignment operator.
	 * 	It is mathematically equivalent to the operator= (copy operator) however it does not get evaluated until an
	 * 	actual operator=, this allows for chaining together multiple complex assignment expressions.
	 * 	It also allows for passing an expression with an assignment and delaying the operation for more optimizations
	 *
	 */
	FeedForward(int inputs, int outputs) :
			INPUTS(inputs), OUTPUTS(outputs),
			w_gradientStorage(outputs, inputs),
			b_gradientStorage(outputs),
			w(outputs, inputs),
			b(outputs),
			x(inputs),
			y(outputs),
			dx(inputs) {

		w.randomize(-4, 4);
		b.randomize(-4, 4);
		w_gradientStorage.zero();
		b_gradientStorage.zero();
	}

	template<class T> auto forwardPropagation(const vec_expr<T>& in) {
		auto x_t = x == in;
		std::cout << "returning fp" << std::endl;
		return g(w * x_t + b);
	}
	template<class T> auto forwardPropagation_Express(const vec_expr<T>& x) const {
		return g(w * x + b);
	}

	template<class T> auto backPropagation(const vec_expr<T>& dy) {
		w_gradientStorage -= dy * x.t();
		b_gradientStorage -= dy;

		return dx == w.t() * dy % gd(x);							//** is point_wise multiply
	}

	void updateWeights() {
		w += w_gradientStorage * lr;
		b += b_gradientStorage * lr;
	}

	void clearBPStorage() {
		w_gradientStorage.zero();
		b_gradientStorage.zero();
	}

	int getClass() {
		return LAYER_TYPE::FeedForward_;
	}
	void write(std::ofstream& is) {
		is << INPUTS << ' ';
		is << OUTPUTS << ' ';
		w.write(is);
		b.write(is);
		x.write(is);
		y.write(is);
		dx.write(is);
		w_gradientStorage.write(is);
		b_gradientStorage.write(is);

	}
	void read(std::ifstream& os) {
		os >> INPUTS;
		os >> OUTPUTS;

		w.read(os);
		b.read(os);
		x.read(os);
		y.read(os);
		dx.read(os);
		w_gradientStorage.read(os);
		b_gradientStorage.read(os);

	}

	//multithreading stuff -----------------------------
	void updateWeights(const FeedForward& ff) {
		w += ff.w_gradientStorage * lr;
		b += ff.b_gradientStorage * lr;
	}
	void fastCopy(const FeedForward& ff) {
		w = ff.w;
		b = ff.b;
	}

};
}



#endif /* FEEDFORWARD_CU_ */