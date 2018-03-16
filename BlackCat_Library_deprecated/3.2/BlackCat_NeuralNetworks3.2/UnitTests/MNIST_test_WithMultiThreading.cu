//#include "../BlackCat_NeuralNetworks.h"
//#include "BlackCat_Tensors.h"
//
//#include <fstream>
//#include <iostream>
//#include <string>
//#include <omp.h>
//#include <mutex>				//ADD THIS FOR MUTXES (multi-thread locking)
//#include <thread>
//using BC::vec;
//typedef std::vector<vec> data;
//
//namespace BC {
//namespace MNIST_Test_multi_thread {
//typedef vec tensor;
//
//tensor expandOutput(int val, int total) {
//	//Convert a value to a 1-hot output vector
//	tensor out(total);
//	out.zero();
//	out[val] = (fp_type) 1.0;
//	return out;
//}
//
//tensor& normalize(tensor& tens, fp_type max, fp_type min) {
//	//generic feature scaling (range of [0,1])
//	tens -= Scalar<fp_type, BC::ml>(min);
//	tens /= Scalar<fp_type, BC::ml>(max - min);
//
//	return tens;
//}
//
//void generateAndLoad(data& input_data, data& output_data, std::ifstream& read_data, int MAXVALS) {
//	unsigned vals = 0;
//
//	//load data from CSV (the first number if the correct output, the remaining 784 columns are the pixels)
//	while (read_data.good() && vals < MAXVALS) {
//		std::string output;
//		std::getline(read_data, output, ',');
//		int out = std::stoi(output);
//
//		tensor input(784);
//		input.read(read_data, false);
//		output_data.push_back(expandOutput(out, 10));
//
//		normalize(input, 255, 0);
//		input_data.push_back(input);
//		++vals;
//	}
//	std::cout << " return -- finished creating data set " << std::endl;
//}
////
//template<int size>
//struct linear_map {
//
//	unsigned int pthread_id[size];
//	std::mutex mtx;
//
//	void reset() {
//		mtx.lock();
//		curr_size = 0;
//		for (int i = 0; i < size; ++i) {
//			pthread_id[i] = 0.0;
//		}
//		mtx.unlock();
//	}
//
//	int curr_size = 0;
//	int getID(unsigned int ptID) {
//		for (int i = 0; i < curr_size; ++i) {
//			if (pthread_id[i] == ptID) {
//				return i;
//			}
//		}
//
//		return add(ptID);
//	}
//
//	int add(unsigned int ptID) {
//		if (curr_size == size) {
//			throw std::invalid_argument("threads given exceeds limit");
//		}
//
//		mtx.lock();
//		pthread_id[curr_size] = ptID;
//		curr_size++;
//		mtx.unlock();
//		return curr_size - 1;
//	}
//};
//
//
//void train(NeuralNetwork<FeedForward, FeedForward, OutputLayer>& network, const data& inputs, const data& outputs, int start_range, int end_range) {
//	for (int i = start_range; i < end_range; ++i) {
//		network.forwardPropagation(inputs[i]);
//		network.backPropagation(outputs[i]);
//	}
//}
//
//void thread_test(int n) {
//	std::cout << n << std::endl;
//}
//
//int percept_MNIST() {
//
//	const int TRAINING_EXAMPLES = 400;
//	const int TRAINING_ITERATIONS = 10;
//	const int NUMB_THREADS = 2;
//
//	const int BATCH_SIZE = 100 / NUMB_THREADS; //must be divisible
//
//
//    omp_set_num_threads(NUMB_THREADS);
//    linear_map<NUMB_THREADS> thread_map;
//
//
//	//Generate the layers (params are: inputs, outputs)
//	FeedForward f1(784, 250);
//	FeedForward f2(250, 10);
//	OutputLayer o3(10);
//
//	//Create the neural network
//	auto network = generateNetwork(f1, f2, o3);									//Create the current network
//	std::vector<decltype(network)> networkSet(NUMB_THREADS, network);		//Create n copies where n = numb threads used - 1 (as we already created an nn)
////	linear_map<NUMB_THREADS> thread_map;										//create a linear map -> this is just a small object to convert pthreads into integers
//
//	//standard data handling code (same as other examples)
//	data inputs;
//	data outputs;
//
//	data testInputs;
//	data testOutputs;
//
//	//load data
//	std::cout << "loading data..." << std::endl << std::endl;
//	std::ifstream in_stream("///home/joseph///Downloads///train.csv");
//	std::string tmp;
//	std::getline(in_stream, tmp, '\n'); //remove headers
//
//	//Load training examples (taken from kaggle digit recognizer train.csv)
//	std::cout << " generating and loading data from csv to tensors" << std::endl;
//	generateAndLoad(inputs, outputs, in_stream, TRAINING_EXAMPLES);
//	in_stream.close();
//
//	for (int i = 0; i < TRAINING_ITERATIONS; ++i) {
//		std::cout << " iteration = " << i << std::endl;
//		//make sure BATCH_SIZE is a divider of n_inputs
//		for (int n = 0; n < inputs.size(); n+= BATCH_SIZE) {
//
//#pragma omp parallel for schedule(static)
//
//		for (int j = n; j < BATCH_SIZE + n; ++j) {
//			int netId = thread_map.getID(pthread_self());
//			networkSet[netId].forwardPropagation(inputs[j]);
//			networkSet[netId].backPropagation(outputs[j]);
//		}
//
//#pragma omp barrier
//		for (int k = 0; k < NUMB_THREADS; ++k) {
//
//			network.updateWeights(networkSet[k]);
//			networkSet[k].clearBPStorage();
//		}
//		for (int k = 0; k < NUMB_THREADS; ++k) {
//			networkSet[k].fastCopy(network);
//		}
//		thread_map.reset();
//	}
//	}
//	std::cout << "\n \n \n " << std::endl;
//	std::cout << " testing... " << std::endl;
//
//	for (int i = 0; i < 10; ++i) {
//		std::cout << " output " << std::endl;
//		outputs[i].print();
//		std::cout << "prediction " << std::endl;
//		network.forwardPropagation(inputs[i]).print();
//		std::cout << "-----------------------------------------------------------------------------------------------------------" << std::endl;
//
//	}
//
//	std::cout << " done training " << std::endl;
//	return 0;
//}
//}
//}
//
////int main() {
////	BC::MNIST_Test_multi_thread::percept_MNIST();
////	std::cout << "success" << std::endl;
////}
////