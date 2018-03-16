//#include "../BlackCat_NeuralNetworks.h"
//#include "../NN_Core/NeuralNetworkTest.h"
//#include "BlackCat_Tensors.h"
//
//#include <fstream>
//#include <iostream>
//#include <string>
//#include <omp.h>
//using BC::vec;
//typedef std::vector<vec> data;
//
//namespace BC {
//namespace MNIST_altTest {
//typedef vec tensor;
//
//tensor expandOutput(int val, int total) {
//	//Convert a value to a 1-hot output vector
//	tensor out(total);
//	out.zero();
//	out[val] = (fp_type)1.0;
//	return out;
//}
//
//tensor&  normalize(tensor& tens, fp_type max, fp_type min) {
//	//generic feature scaling (range of [0,1])
//	tens -= Scalar<fp_type, BC::ml>(min);
//	tens /= Scalar<fp_type, BC::ml>(max - min);
//
//	return tens;
//}
//bool correct(const vec& hypothesis, const vec& output) {
//	int h_id = 0;
//	int o_id = 0;
//
//	double h_max = hypothesis.data()[0];
//	double o_max = output.data()[0];
//
//	for (int i = 1; i < hypothesis.size(); ++i) {
//		if (hypothesis.data()[i] > h_max) {
//			h_max = hypothesis.data()[i];
//			h_id = i;
//		}
//		if (output.data()[i] > o_max) {
//			o_max = output.data()[i];
//			o_id = i;
//		}
//	}
//	return h_id == o_id;
//}
//
//
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
//
//int percept_MNIST() {
//
//	const int TRAINING_EXAMPLES = 2000;
//	const int TRAINING_ITERATIONS = 10;
//
//	//Generate the layers (params are: inputs, outputs)
//	FeedForward f1(784, 250);
//	FeedForward f2(250, 10);
//	OutputLayer o3(10);
//	//Create the neural network
//	auto network = generateNetworkTest(f1, f2, o3);
//
//
//
//	data inputs;
//	data outputs;
//
//	data testInputs;
//	data testOutputs;
//
//	//load data
//	std::cout << "loading data..." << std::endl << std::endl;
//	std::ifstream in_stream("///home/joseph///Downloads///train.csv");
//	std::string tmp; std::getline(in_stream, tmp, '\n'); //remove headers
//
//	//Load training examples (taken from kaggle digit recognizer train.csv)
//	std::cout << " generating and loading data from csv to tensors" << std::endl;
//	generateAndLoad(inputs, outputs, in_stream, TRAINING_EXAMPLES);
//	in_stream.close();
//
//
//	//Train
//	float t;
//	t = omp_get_wtime();
//		printf("\n Calculating... BC_NN training time \n");
//
//
//
//	std::cout << " training..." << std::endl;
//
//	for (int i = 0; i < TRAINING_ITERATIONS; ++i) {
//		std::cout << " iteration =  " << i << std::endl;
//		for (int j = 0; j < inputs.size(); ++j) {
//			network.forwardPropagation(inputs[j]);
//			network.backPropagation(outputs[j]);
//
//			//this is just the batch size
//			if (j % 100 == 0) {
////			std::cout << " update: " << j / 100 << std::endl;
//			network.updateWeights();
//			network.clearBPStorage();
//			}
//		}
//	}
//
//
//	t = omp_get_wtime() - t;
//	printf("It took me %f clicks (%f seconds).\n", t, ((float) t));
//	std::cout << "success " << std::endl;
//
//	float correct_ = 0;
//	for (int i = 0; i < inputs.size(); ++i) {
//		if (correct(network.forwardPropagation(inputs[i]), outputs[i])) {
//			++correct_;
//		}
//	}
//	std::cout << " correct: " << correct_/inputs.size()  <<std::endl;
//
//
//	std::cout << "\n \n \n " << std::endl;
//	std::cout << " testing... " << std::endl;
//
//	for (int i = 0; i < 10; ++i) {
//		std::cout << " output " << std::endl;
//		outputs[i].print();
//		std::cout << "prediction " << std::endl;
//		network.forwardPropagation(inputs[i]).print();
//		std::cout << "-----------------------------------------------------------------------------------------------------------" <<std::endl;
//
//	}
//
//
//	std::cout << " done training " << std::endl;
//	return 0;
//}
//}
//}
//
//
//int main() {
//	BC::MNIST_altTest::percept_MNIST();
//	std::cout << "success" << std::endl;
//}
//