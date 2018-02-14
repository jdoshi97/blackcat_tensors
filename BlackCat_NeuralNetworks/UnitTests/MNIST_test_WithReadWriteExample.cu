#include "../BlackCat_NeuralNetworks.h"
#include "BlackCat_Tensors.h"

#include <fstream>
#include <iostream>
#include <string>

using BC::vec;
typedef std::vector<vec> data;

namespace BC {
namespace MNIST_Test_ReadWrite {
typedef vec tensor;

tensor expandOutput(int val, int total) {
	//Convert a value to a 1-hot output vector
	tensor out(total);
	out.zero();
	out[val] = (fp_type)1.0;
	return out;
}

tensor&  normalize(tensor& tens, fp_type max, fp_type min) {
	//generic feature scaling (range of [0,1])
	tens -= Scalar<fp_type, BC::ml>(min);
	tens /= Scalar<fp_type, BC::ml>(max - min);

	return tens;
}

void generateAndLoad(data& input_data, data& output_data, std::ifstream& read_data, int MAXVALS) {
	unsigned vals = 0;

	//load data from CSV (the first number if the correct output, the remaining 784 columns are the pixels)
	while (read_data.good() && vals < MAXVALS) {
		std::string output;
		std::getline(read_data, output, ',');
		int out = std::stoi(output);

		tensor input(784);
		input.read(read_data, false);
		output_data.push_back(expandOutput(out, 10));

		normalize(input, 255, 0);
		input_data.push_back(input);
		++vals;
	}
	std::cout << " return -- finished creating data set " << std::endl;
}

int percept_MNIST() {

	const int TRAINING_EXAMPLES = 40;
	const int TRAINING_ITERATIONS = 100;

	//Generate the layers (params are: inputs, outputs)
	FeedForward f1(784, 400);
	FeedForward f2(400, 10);
	OutputLayer o3(10);


//	tup.get<1>().forwardPropagation(vec(10));

	//Create the neural network
	auto network = generateNetwork(f1, f2, o3);

	data inputs;
	data outputs;

	data testInputs;
	data testOutputs;

	//load data
	std::cout << "loading data..." << std::endl << std::endl;
	std::ifstream in_stream("///home/joseph///Downloads///train.csv");
	std::string tmp; std::getline(in_stream, tmp, '\n'); //remove headers

	//Load training examples (taken from kaggle digit recognizer train.csv)
	std::cout << " generating and loading data from csv to tensors" << std::endl;
	generateAndLoad(inputs, outputs, in_stream, TRAINING_EXAMPLES);
	in_stream.close();

	//Train
	std::cout << " training..." << std::endl;

	for (int i = 0; i < TRAINING_ITERATIONS; ++i) {
		std::cout << " iteration =  " << i << std::endl;
		for (int j = 0; j < inputs.size(); ++j) {
			network.forwardPropagation(inputs[j]);
			network.backPropagation(outputs[j]);

			//this is just the batch size
			if (j % 100 == 0) {
			network.updateWeights();
			network.clearBPStorage();
			}
		}
	}

	std::cout << " opening NN_ReadWriteText.txt" << std::endl;
	std::cout << " --> writing to file " << std::endl;
	std::ofstream os("NN_ReadWriteTest.txt");
	network.write(os);
	std::cout << " write successful " << std::endl;
	os.close();


	//These dimensions will be overwritten by reading from a file//
	FeedForward f(1,1);
	FeedForward f_(1,1);
	OutputLayer o(1);


	std::cout << " opening NN_ReadWriteText.txt" << std::endl;
	std::cout << " --> reading from file " << std::endl;
	std::ifstream is("NN_ReadWriteTest.txt");

	auto network2 = generateNetwork(f, f_, o);
	network2.read(is);
	is.close();



	std::cout << "read successful! \n \n \n " << std::endl;
	std::cout << " testing the network that was read from file..." << std::endl;

	for (int i = 0; i < 10; ++i) {
		std::cout << "data[i] = ==== -----------------------------------------------" <<std::endl;

		std::cout << " output " << std::endl;
		outputs[i].print();
		std::cout << "prediction " << std::endl;
		network2.forwardPropagation(inputs[i]).print(); //testing network2 not 1
		std::cout << "-------------------COMPARETO-----------------------------------------------" <<std::endl;
		std::cout << " output " << std::endl;
		outputs[i].print();
		std::cout << "prediction " << std::endl;
		network.forwardPropagation(inputs[i]).print(); //testing network2 not 1
		std::cout << "--------------------------------------------------------------------------------------------" <<std::endl;

	}


	std::cout << " done training " << std::endl;
	return 0;
}
}
}


//int main() {
//	BC::MNIST_Test_ReadWrite::percept_MNIST();
//	std::cout << "success" << std::endl;
//}

