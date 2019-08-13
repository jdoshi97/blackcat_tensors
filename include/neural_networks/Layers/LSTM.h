/*
 * LSTM.h
 *
 *  Created on: Aug 3, 2019
 *	  Author: joseph
 */

#ifndef BLACKCAT_TENSORS_NEURALNETWORKS_LSTM_H_
#define BLACKCAT_TENSORS_NEURALNETWORKS_LSTM_H_

#include "OutputLayer.h"
#include "Recurrent.h"

namespace BC {
namespace nn {

using BC::algorithms::reference_list;

template<class SystemTag,
		class ValueType,
		class ForgetGateNonlinearity=BC::Logistic,
		class WriteGateNonlinearity=BC::Tanh,
		class InputGateNonlinearity=BC::Logistic,
		class OutputGateNonlinearity=BC::Logistic,
		class CellStateNonLinearity=BC::Tanh>
struct LSTM : public Layer_Base {

	using system_tag = SystemTag;
	using value_type = ValueType;
	using allocator_type = BC::Allocator<SystemTag, ValueType>;

	using mat = BC::Matrix<value_type, allocator_type>;
	using vec = BC::Vector<value_type, allocator_type>;

	using greedy_evaluate_delta = std::true_type;
	using forward_requires_outputs = std::true_type;
	using backward_requires_outputs = std::true_type;


private:

	ValueType lr = Layer_Base::default_learning_rate;

	CellStateNonLinearity  c_g;
	ForgetGateNonlinearity f_g;
	WriteGateNonlinearity z_g;
	InputGateNonlinearity i_g;
	OutputGateNonlinearity o_g;

	//back-propagation index, relative to the size of the number of stored back-prop activations
	unsigned t_minus_index = 0;

	mat wf, wz, wi, wo;
	mat wf_gradients, wz_gradients, wi_gradients, wo_gradients;

	mat rf, rz, ri, ro;
	mat rf_gradients, rz_gradients, ri_gradients, ro_gradients;

	vec bf, bz, bi, bo;
	vec bf_gradients, bz_gradients, bi_gradients, bo_gradients;

	mat dc, df, dz, di, do_;
	mat c;

	std::vector<mat> cs, fs, zs, is, os;

private:

	auto& time_index(std::vector<mat>& intermediate, int modifier=0) {
		return intermediate[intermediate.size() - 1 - t_minus_index + modifier];
	}

public:

	LSTM(int inputs, BC::size_t  outputs):
		Layer_Base(__func__, inputs, outputs),
		wf(outputs, inputs),
		wz(outputs, inputs),
		wi(outputs, inputs),
		wo(outputs, inputs),

		wf_gradients(outputs, inputs),
		wz_gradients(outputs, inputs),
		wi_gradients(outputs, inputs),
		wo_gradients(outputs, inputs),

		rf(outputs, outputs),
		rz(outputs, outputs),
		ri(outputs, outputs),
		ro(outputs, outputs),

		rf_gradients(outputs, outputs),
		rz_gradients(outputs, outputs),
		ri_gradients(outputs, outputs),
		ro_gradients(outputs, outputs),

		bf(outputs),
		bz(outputs),
		bi(outputs),
		bo(outputs),

		bf_gradients(outputs),
		bz_gradients(outputs),
		bi_gradients(outputs),
		bo_gradients(outputs) {

		wf.randomize(-1, 0);
		wz.randomize(-.1, .1);
		wi.randomize(-.1, .1);
		wo.randomize(0, .5);

		rf.randomize(-1, 1);
		rz.randomize(-.1, .1);
		ri.randomize(-.1, .1);
		ro.randomize(0, .5);

		bf.randomize(-1, 1);
		bz.randomize(-.1, .1);
		bi.randomize(-.1, .1);
		bo.randomize(0, .5);

		zero_gradients();
	}



	template<class X>
	auto forward_propagation(const X& x) {
		if (cs.empty()) {
			cs.push_back(c);
		}

		t_minus_index = 0;

		fs.push_back( f_g(wf * x + bf) );
		is.push_back( i_g(wi * x + bi) );
		zs.push_back( z_g(wz * x + bz) );
		os.push_back( o_g(wo * x + bo) );

		auto& f = fs.back();
		auto& z = zs.back();
		auto& i = is.back();
		auto& o = os.back();

		c %= f; 	//%= element-wise assign-multiplication
		c += z % i; //%  element-wise multiplication
		cs.push_back(c);

		return c_g(c) % o;
	}

	template<class X, class Y>
	auto forward_propagation(const X& x, const Y& y) {
		if (cs.empty()) {
			cs.push_back(c);
		}

		t_minus_index = 0;

		fs.push_back( f_g(wf * x + rf * y + bf) );
		zs.push_back( z_g(wz * x + rz * y + bz) );
		is.push_back( i_g(wi * x + ri * y + bi) );
		os.push_back( o_g(wo * x + ro * y + bo) );

		auto& f = fs.back();
		auto& z = zs.back();
		auto& i = is.back();
		auto& o = os.back();

		c %= f; 	//%= element-wise assign-multiplication
		c += z % i; //%  element-wise multiplication
		cs.push_back(c);

		return c_g(c) % o;
	}

	template<class X, class Y, class Delta>
	auto back_propagation(const X& x, const Y& y,  const Delta& delta_outputs) {
		//LSTM Backprop reference
		//Reference: https://arxiv.org/pdf/1503.04069.pdf
		//delta_t+1 (deltas haven't been updated from previous step yet)

		//If time_index == 0, than deltas are set to 0
		if (t_minus_index != 0) {
			rz_gradients -= dz * y.t();
			rf_gradients -= df * y.t();
			ri_gradients -= di * y.t();
			ro_gradients -= do_ * y.t();
		}

		mat& f = time_index(fs);
		mat& z = time_index(zs);
		mat& i = time_index(is);
		mat& o = time_index(os);
		mat& cm1 = time_index(cs, -1);
		mat& c = time_index(cs);

		mat dy = delta_outputs +
				rz.t() * dz +
				ri.t() * di +
				rf.t() * df +
				ro.t() * do_;

		do_ = dy % c_g(c) % o_g.cached_dx(o);

		if (t_minus_index != 0) {
			dc = dy % o % c_g.dx(c) + dc % time_index(fs, 1);
		} else {
			dc = dy % o % c_g.dx(c);
		}

		df = dc % cm1  % f_g.cached_dx(f);
		di = dc % z % i_g.cached_dx(i);
		dz = dc % i % z_g.cached_dx(z);

		wz_gradients -= dz * x.t();
		wf_gradients -= df * x.t();
		wi_gradients -= di * x.t();
		wo_gradients -= do_ * x.t();

		bz_gradients -= dz;
		bf_gradients -= df;
		bi_gradients -= di;
		bo_gradients -= do_;

		//Increment the current index
		//of the internal cell-state
		t_minus_index++;

		return wz.t() * dz +
				wi.t() * dz +
				wf.t() * df +
				wo.t() * do_;
	}

	void update_weights() {
		ValueType lr = this->lr / this->batch_size();

		wz += wz_gradients * lr;
		wf += wf_gradients * lr;
		wi += wi_gradients * lr;
		wo += wo_gradients * lr;

		rz += rz_gradients * lr;
		rf += rf_gradients * lr;
		ri += ri_gradients * lr;
		ro += ro_gradients * lr;

		bz += bz_gradients * lr;
		bf += bf_gradients * lr;
		bi += bi_gradients * lr;
		bo += bo_gradients * lr;

		zero_gradients();
		clear_bp_storage();
	}

	void set_batch_size(int bs) {
		for (auto& delta: reference_list(dc, df, dz, di, do_, c)) {
			delta = mat(this->output_size(), bs);
		}

		zero_deltas();
	}

	void clear_bp_storage() {
		//remove all but the last element
		for (auto& bp_storage : reference_list(cs, fs, zs, is, os)) {
			bp_storage.erase(bp_storage.begin(), bp_storage.end()-1);
		}
	}

	void zero_deltas() {
		for (auto& delta : reference_list(dc, df, di, dz, do_)) {
			delta.zero();
		}
	}

	void zero_gradients() {
		for (auto& grad : reference_list(
				wf_gradients, wz_gradients, wi_gradients, wo_gradients,
				rf_gradients, rz_gradients, ri_gradients, ro_gradients)) {
			grad.zero();
		}

		for (auto& grad : reference_list(
				bf_gradients, bz_gradients, bi_gradients, bo_gradients)) {
			grad.zero();
		}
	}

	void save(int index, std::string directory_name) {
		std::string subdir = "l" + std::to_string(index) + "_" + this->classname();
		std::string fullpath = directory_name + bc_directory_separator() + subdir;
		std::string mkdir = "mkdir " + fullpath;

		int error = system(mkdir.c_str());

#define BC_LSTM_SAVE(weight, type)\
		{\
			std::ofstream os(fullpath + bc_directory_separator() + #weight"."#type);\
			os<< weight.to_raw_string();\
		}
#define BC_LSTM_SAVE_W(weight) BC_LSTM_SAVE(weight, mat)
#define BC_LSTM_SAVE_B(weight) BC_LSTM_SAVE(weight, vec)

		BC_LSTM_SAVE_W(c)

		BC_LSTM_SAVE_W(wf)
		BC_LSTM_SAVE_W(wz)
		BC_LSTM_SAVE_W(wi)
		BC_LSTM_SAVE_W(wo)

		BC_LSTM_SAVE_W(rf)
		BC_LSTM_SAVE_W(rz)
		BC_LSTM_SAVE_W(ri)
		BC_LSTM_SAVE_W(ro)

		BC_LSTM_SAVE_B(bf)
		BC_LSTM_SAVE_B(bz)
		BC_LSTM_SAVE_B(bi)
		BC_LSTM_SAVE_B(bo)

#undef BC_LSTM_SAVE
#undef BC_LSTM_SAVE_W
#undef BC_LSTM_SAVE_B

	}

	void load(int index, std::string directory_name) {
		std::string subdir = "l" + std::to_string(index) + "_" + this->classname();
		std::string fullpath = directory_name + bc_directory_separator() + subdir;

#define BC_LSTM_LOAD_W(weight)\
		weight = BC::io::read_uniform<value_type>(\
		BC::io::csv_descriptor(fullpath + bc_directory_separator() + #weight".mat").header(false), allocator_type());\

#define BC_LSTM_LOAD_B(bias)\
		bias = vec(this->output_size());\
		bias = BC::io::read_uniform<value_type>(\
				BC::io::csv_descriptor(fullpath + bc_directory_separator() + #bias".vec").header(false), allocator_type()).row(0);\

		BC_LSTM_LOAD_W(c)

		BC_LSTM_LOAD_W(wf)
		BC_LSTM_LOAD_W(wz)
		BC_LSTM_LOAD_W(wi)
		BC_LSTM_LOAD_W(wo)

		BC_LSTM_LOAD_W(rf)
		BC_LSTM_LOAD_W(rz)
		BC_LSTM_LOAD_W(ri)
		BC_LSTM_LOAD_W(ro)

		//Required --
		Layer_Base::resize(wf.cols(), wf.rows());

		BC_LSTM_LOAD_B(bf)
		BC_LSTM_LOAD_B(bz)
		BC_LSTM_LOAD_B(bi)
		BC_LSTM_LOAD_B(bo)

#undef BC_LSTM_LOAD_W
#undef BC_LSTM_LOAD_B
	}

};

#ifndef BC_CLING_JIT
template<class ValueType, class SystemTag>
LSTM<SystemTag, ValueType> lstm(SystemTag system_tag, int inputs, int outputs) {
	return LSTM<SystemTag, ValueType>(inputs, outputs);
}
#endif

template<class SystemTag>
auto lstm(SystemTag system_tag, int inputs, int outputs) {
	return LSTM<SystemTag, typename SystemTag::default_floating_point_type>(inputs, outputs);
}

auto lstm(int inputs, int outputs) {
	return LSTM<BLACKCAT_DEFAULT_SYSTEM_T,
			typename BLACKCAT_DEFAULT_SYSTEM_T::default_floating_point_type>(inputs, outputs);
}


}
}



#endif /* LSTM_H_ */
