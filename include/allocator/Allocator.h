/*
 * Allocators.h
 *
 *  Created on: Dec 4, 2018
 *      Author: joseph
 */

#ifndef BC_ALLOCATOR_ALLOCATOR_H_
#define BC_ALLOCATOR_ALLOCATOR_H_

namespace BC {
namespace allocator {

template<class derived>
struct AllocatorBase {
	AllocatorBase() {
	}
};

template<class custom_allocator, class _system_tag=host_tag>
struct CustomAllocator
		: custom_allocator,
		  AllocatorBase<CustomAllocator<custom_allocator>> {
			using system_tag = _system_tag;

			CustomAllocator() = default;
			using custom_allocator::custom_allocator;
		};

template<class allocator, class=void>
struct has_system_tag : std::false_type {};

template<class allocator>
struct has_system_tag<allocator, std::enable_if_t<!std::is_void<typename allocator::system_tag>::value>> : std::true_type {};


}
}


#include "Host.h"
#include "Device.h"
#include "Device_Managed.h"

namespace BC {

class host_tag;
class device_tag;

namespace allocator {

#ifdef __CUDACC__
	template<class system_tag, class value_type>
	using implementation =
			std::conditional_t<
				std::is_same<host_tag, system_tag>::value,
					Host<value_type>,
					Device<value_type>>;

#else
	template<
		class system_tag, class value_type,
		class=std::enable_if<std::is_same<system_tag, host_tag>::value>
	>
	using implementation = Host<value_type>;
#endif




}
}

#endif
