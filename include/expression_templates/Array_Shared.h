/*  Project: BlackCat_Tensors
 *  Author: JosephJaspers
 *  Copyright 2018
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BC_INTERNALS_BC_TENSOR_EXPRESSION_TEMPLATES_ARRAY_SHARED_H_
#define BC_INTERNALS_BC_TENSOR_EXPRESSION_TEMPLATES_ARRAY_SHARED_H_

#include "Array_Base.h"
#include "Array.h"

namespace BC {
namespace et     {

template<int dimension, class scalar, class allocator>
struct Array_Shared
        : Array_Base<Array_Shared<dimension, scalar, allocator>, dimension>,
          Shape<dimension> {

    using scalar_t = scalar;
    using allocator_t = allocator;
    using mathlib_t = typename allocator_t::mathlib_t;

    scalar_t* array = nullptr;

    Array_Shared()                    = default;
    Array_Shared(const Array_Shared& ) = default;
    Array_Shared(       Array_Shared&&) = default;

    auto& operator = (scalar_t* move_array) {
        this->array = move_array;
        return *this;
    }

    void swap_array(Array_Shared& tensor) {
        std::swap(array, tensor.array);
    }

    template<class tensor_t, typename = std::enable_if_t<tensor_t::DIMS() == dimension>>
    Array_Shared(Array_Base<tensor_t, dimension>& tensor)
        :  Shape<dimension>(), array(tensor) {

        this->copy_shape(static_cast<tensor_t&>(tensor));
    }

    void copy_init(const Array_Shared& view) {
        this->copy_shape(view);
        this->array = view.array;
    }

    template<class... integers>
    Array_Shared(int dim1, integers... dims) : Shape<dimension>(dim1, dims...) {}

    __BCinline__  const scalar_t* memptr() const  { return array; }
    __BCinline__          scalar_t* memptr()        { return array; }

    void deallocate() {}
};

    template<int x, class s, class a>
    struct BC_array_move_constructible_overrider<et::Array_Shared<x,s,a>> {
        static constexpr bool boolean = true;
    };

    template<int x, class s, class a>
    struct BC_array_copy_constructible_overrider<et::Array_Shared<x,s,a>> {
        static constexpr bool boolean = true; //shared doesn't actually copy
    };

    template<int x, class s, class a>
    struct BC_array_move_assignable_overrider<et::Array_Shared<x,s,a>> {
        static constexpr bool boolean = true;
    };
    template<int d, class s, class a> struct BC_lvalue_type_overrider<Array_Shared<d,s,a>> {
        static constexpr bool boolean = true;
    };

}
}



#endif /* BC_INTERNALS_BC_TENSOR_EXPRESSION_TEMPLATES_ARRAY_SHARED_H_ */