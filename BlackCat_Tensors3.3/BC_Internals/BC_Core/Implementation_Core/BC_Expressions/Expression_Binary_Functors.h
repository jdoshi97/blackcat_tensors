/*
 * BC_Expression_Binary_Functors.h
 *
 *  Created on: Dec 1, 2017
 *      Author: joseph
 */
#ifndef EXPRESSION_BINARY_FUNCTORS_H_
#define EXPRESSION_BINARY_FUNCTORS_H_

#include "BlackCat_Internal_Definitions.h"
#include <iostream>

namespace BC {
template<class T> struct rm_const { using type = T; };
template<class T> struct rm_const<const T&> { using type = T&; };

		struct assign {

		template<class lv, class rv> __BCinline__  auto& operator ()(lv& l, rv r) const {
			return (const_cast<typename rm_const<lv&>::type>(l) = r);
		}
	};

	struct combine {

		template<class lv, class rv> __BCinline__  auto operator ()(lv l, rv r) const {
			return l;
		}
	};


	struct add {

		template<class lv, class rv> __BCinline__  auto operator ()(lv l, rv r) const {
			return l + r;
		}
	};

	struct mul {
		template<class lv, class rv> __BCinline__  auto operator ()(lv l, rv r) const {
			return l * r;
		}
	};

	struct sub {
		template<class lv, class rv> __BCinline__  auto operator ()(lv l, rv r) const {
			return l - r;
		}
	};

	struct div {
		template<class lv, class rv> __BCinline__  auto operator ()(lv l, rv r) const {
			return l / r;
		}
	};
	struct add_assign {

		template<class lv, class rv> __BCinline__  auto operator ()(lv& l, rv r) const {
			return l += r;
		}
	};

	struct mul_assign {
		template<class lv, class rv> __BCinline__  auto operator ()(lv& l, rv r) const {
			return l *= r;
		}
	};

	struct sub_assign {
		template<class lv, class rv> __BCinline__  auto operator ()(lv& l, rv r) const {
			return l -= r;
		}
	};

	struct div_assign {
		template<class lv, class rv> __BCinline__  auto operator ()(lv& l, rv r) const {
			return l /= r;
		}
	};
}


#endif /* EXPRESSION_BINARY_FUNCTORS_H_ */
