/*
 * Generator.h
 *
 *  Created on: Feb 22, 2018
 *      Author: joseph
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_


#include "InputLayer.h"
#include "OutputLayer.cu"
#include "Defaults.h"


#include "LayerChain.cu"


namespace BC {

template< class head>
void initializerTail(head net, int x) {
	net.data_ = new typename decltype(net.data_)::type(x);

}

template< class head, class... integers>
void initializer(head net, int x, int y, integers... params) {
	net.data_ = new typename  decltype(net.data_)::type(x, y);

	if (sizeof...(params) > 1)
	initializer(net.next(), y, params...);
	else
		initializerTail(net.next(), params...);
}
template< class head, class... integers>
void initializerHead(head net, int x, integers... params) {

	net.data_ = new  typename  decltype(net.data_)::type(x);
	initializer(net.next(), x, params...);
}


template<template<class> class... layers, class... integers>
auto generateNetwork(integers... structure) {
	auto net = LayerChain<BASE, InputLayer, layers..., OutputLayer>(structure...);
//	initializerHead(net, structure...);

	return net;
}



}



#endif /* GENERATOR_H_ */