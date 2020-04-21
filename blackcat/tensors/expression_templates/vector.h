#ifndef BLACKCAT_TENSORS_EXPRESSION_TEMPLATES_VECTOR_H_
#define BLACKCAT_TENSORS_EXPRESSION_TEMPLATES_VECTOR_H_

#include "array_kernel_array.h"

#ifdef __CUDACC__
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#else
#include <vector>
#endif

namespace bc {
namespace tensors {
namespace exprs {

template<class ValueType, class AllocatorType>
struct Vector {

	using value_type = ValueType;
	using allocator_type = AllocatorType;
	using system_tag = typename bc::allocator_traits<allocator_type>::system_tag;
	using stream_type = bc::streams::Stream<system_tag>;

	static constexpr int tensor_dim = 1;
	static constexpr int tensor_iterator_dim = 1;

private:
	#ifdef __CUDACC__
	using array_type = std::conditional_t<
			std::is_same<system_tag, bc::host_tag>::value,
			thrust::host_vector<value_type, allocator_type>,
			thrust::device_vector<value_type, allocator_type>>;
	#else
	using array_type = std::vector<value_type, allocator_type>;
	#endif

	using expression_template_type = Kernel_Array<bc::Shape<1>, value_type, system_tag>;
	using const_expression_template_type = Kernel_Array<bc::Shape<1>, const value_type, system_tag>;

	array_type m_vector;
	stream_type m_stream;

public:
	using expression_template_array_type = std::true_type;

	Vector(const array_type& vector): m_vector(vector) {}
	Vector(array_type&& vector): m_vector(vector) {}
	Vector(bc::size_t size): m_vector(size) {}

	auto get_shape() const { return bc::Shape<1>( m_vector.size());}
	size_t size() const { return m_vector.size(); }
	size_t rows() const { return m_vector.size(); }
	size_t cols() const { return 1; }
	size_t dim(int i) const { return i == 0 ? rows() : 1; }
	size_t outer_dim() const { return rows(); }
	size_t leading_dim(int i=0) const { return 1;}
	auto inner_shape() const { return bc::Dim<1> { size() }; }

	const_expression_template_type expression_template() const {
		return const_expression_template_type(bc::Shape<1>(size()), data());
	}

	expression_template_type expression_template() {
		return expression_template_type(bc::Shape<1>(size()), data());
	}

	const stream_type& get_stream() const { return m_stream; }
	      stream_type& get_stream()       { return m_stream; }

	allocator_type get_allocator() const {
		return m_vector.get_allocator();
	}

	//TODO deprecate this requirement
	void deallocate() {}

	/*
	* std::vector methods
	*/

	value_type* data() const {
		//calling 'data' causes compile issues for thrust
		//TODO fix the requirement of the cast.
		//(internally not handling constness correctly),
		return (value_type*)&m_vector.front();
	}

	value_type* data() {
		return &m_vector.front();
	}

	void clear() {
		m_vector.clear();
	}

	void reserve(bc::size_t reserve_sz) {
		m_vector.reserve(reserve_sz);
	}

	void resize(bc::size_t new_sz) {
		m_vector.resize(new_sz);
	}

	void push_back(value_type&& value) {
		m_vector.push_back(value);
	}

	void push_back(const value_type& value) {
		m_vector.push_back(value);
	}

	void emplace_back(value_type&& value) {
		m_vector.emplace_back(value);
	}

	void emplace_back(const value_type& value) {
		m_vector.emplace_back(value);
	}

	//TODO add
	//insert, emplace
	void pop_back() {
		m_vector.pop_back();
	}

	auto& at(bc::size_t index) const {
		return m_vector.at(index);
	}

	auto& at(bc::size_t index) {
		return m_vector.at(index);
	}

	bool empty() const {
		return m_vector.empty();
	}

	std::size_t max_size() const {
		return m_vector.max_size();
	}

	std::size_t capacity() const {
		return m_vector.capacity();
	}

	void shrink_to_fit() {
		m_vector.shrink_to_fit();
	}
};



}
}
}

#endif
