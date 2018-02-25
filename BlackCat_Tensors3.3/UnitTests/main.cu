#include <iostream>
#include "../BlackCat_Tensors.h"
#include "SpeedTests.h"
using BC::Vector;
using BC::Matrix;
using BC::Scalar;
using BC::Cube;

auto test() {

	Matrix<double> a(3, 2);
	Matrix<double> b(2 ,3);
	Matrix<double> d(2, 3);
	Matrix<double> e(3, 2);
	Matrix<double> c(2, 2);

	for (int i = 0; i < 6; ++i)  {
		b.data()[i] = i + 7;
		a.data()[i] = i + 1;
	}

	std::cout << std::endl;






	d = a.t();
	e = b.t();


	a.print();
	b.print();
	d.print();
	e.print();

	std::cout << " simple dot product " << std::endl;
	c = d * e;
	c.print();
	///all the permutations of optimized dotproduct
	c = a.t() * b.t();
	c = a.t() * e;
	c = d * b.t();
	c = d * Scalar<double>(2) * e;
	c = Scalar<double>(2) * d * e;
	c = d * e * Scalar<double>(2); ////This is the only version that is not accounted for (it is also the least common notation)
	c = d * Scalar<double>(2) * e;

	c.print();

	Scalar<double> A(2);
	Scalar<double> B(2);

	c.print();

	c = a.t() * b.t();

	A.print();

	c = a.t() * A * (b.t() * A);

	c.print();

	Cube<double> cu(2,3, 4);
	cu.zero();
	cu.print();


	std::cout << " sub vector test " << std::endl;

	d.print();
	std::cout << " adding d[1] = d[1] + d[1]" << std::endl;
	d[1] = d[1] + d[1];
	d.print();
	std::cout << " adding d[1][1] = d[1][1] + d[1][1] " << std::endl;


	d[1][1] = d[1][1] + d[1][1];
	d.print();

}


int main() {

//	speedTestDelayedEval<128,     100000>();
//	speedTestDelayedEval<256,     100000>();
//	speedTestDelayedEval<512,     100000>();
//	speedTestDelayedEval<1024,    100000>();
//	speedTestDelayedEval<2048,    100000>();
//	speedTestDelayedEval<5096,    100000>();
//	speedTestDelayedEval<10000,   100000>();
//	speedTestDelayedEval<20000,   100000>();
//	speedTestDelayedEval<40000,   100000>();
//	speedTestDelayedEval<80000,   100000>();
//	speedTestDelayedEval<100000,  100000>();
		speedTestsScalarIteration<128,     100000>();
		speedTestsScalarIteration<256,     100000>();
		speedTestsScalarIteration<512,     100000>();
		speedTestsScalarIteration<1024,    100000>();
		speedTestsScalarIteration<2048,    100000>();
		speedTestsScalarIteration<5096,    100000>();
		speedTestsScalarIteration<10000,   100000>();
		speedTestsScalarIteration<20000,   100000>();
		speedTestsScalarIteration<40000,   100000>();
		speedTestsScalarIteration<80000,   100000>();
		speedTestsScalarIteration<100000,  100000>();
//	MatrixPointwise();
	test();

	std::cout << " success  main"<< std::endl;


	return 0;
}
