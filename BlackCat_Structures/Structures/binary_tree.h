/*
 * binary_tree.h
 *
 *  Created on: Apr 18, 2018
 *      Author: joseph
 */

#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_

#include "BC_Collections.h"

namespace BC {
namespace Structure {

template<class T, class deleter = default_deleter>
class binary_tree : Collection<T, deleter> {

	struct node {
		static constexpr deleter destroy = deleter();

		T data;
		node* left;
		node* right;

		node(T d, node* l=nullptr, node* r=nullptr) :
			data(d), left(l), right(r) {}
		~node() {
			deleter()(data);
		}
	};

	int sz = 0;
	node* head = nullptr;


public:
	binary_tree() = default;
	virtual ~binary_tree() { clear(); }
	virtual bool empty() const { return !sz; }
	virtual int size()  const { return sz; }
	virtual bool add(T data) override {

		if (!head) {
			head = new node(data);
			++sz;
			return true;
		}
		node* ref = head;

		while (ref) {
			if (data < ref->data)
				if (ref -> left)
					ref = ref->left;
				else
					ref -> left = new node(data);
			else if (data > ref->data)
				if (ref -> right)
					ref = ref->right;
				else
					ref -> right = new node(data);
			else if (data == ref->data)
				return false;
		}
		++sz;
		return true;
	}
private:
	node* left_most_child(node* ref){
		while (ref->left && ref->right) {
			while (ref->left)
				ref = ref->left;
			if (ref->right)
				ref = ref->right;
		}

		return ref;
	}
	node* right_most_child(node* ref){
		while (ref->left && ref->right) {
			while (ref->right)
				ref = ref->right;
			if (ref->left)
				ref = ref->left;

		}

		return ref;
	}

	//naive implementation, always concats left most side
	node* remove_impl(T data, node* ref) {
		if (!ref)
			return nullptr;
		if (ref->data == data){
			node* link;

			if (!ref->left)
				link =  ref->right;
			else if(!ref->right)
				link = ref->left;
			else
				link = left_most_child(ref);

			delete ref;
			return link;
		}
		if (data < ref->data)
			ref->left = remove_impl(data, ref->left);
		else if (data > ref->data)
			ref->right = remove_impl(data, ref->right);

		return ref;
	}

public:
	virtual bool remove(T data) {
		remove_impl(data, head);
		return true;
	}

private:
	void clear_impl(node* ref) {
		if (ref->left)
			clear_impl(ref->left);

		if (ref->right)
			clear_impl(ref->right);

		delete ref;
	}
public:
	virtual void clear() override {
		clear_impl(head);
		head = nullptr;
		sz = 0;
	}
private:
	void print_impl(node* ref) {
		if (!ref)
			return;

		print_impl(ref->left);
		std::cout << ref->data << std::endl;
		print_impl(ref->right);
	}
public:
	void print() {
		print_impl(head);
	}
};







}

}



#endif /* BINARY_TREE_H_ */
