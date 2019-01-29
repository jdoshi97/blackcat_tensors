/*
 * host.h
 *
 *  Created on: Dec 3, 2018
 *      Author: joseph
 */

#ifndef BC_BLAS_HOST_H_
#define BC_BLAS_HOST_H_

#include <cblas.h>
//#include <mkl_cblas.h> //TODO create/ifdef wrapper for MKL

namespace BC {
namespace blas {
/*
 * creates a BLAS wrapper for BC_Tensors
 * -> uses generic function names but without the prefix of s/d for precision type.
 *  The automatic template deduction chooses the correct path
 *
 *  (this is to enable BC_Tensors's to not have to specialize the template system)
 */

struct Host {

    /*
     * a = M x K
     * b = K x N
     * c = M x N
     */

	template<class T, class enabler = void>
	struct get_value_impl {
	    static auto impl(T scalar) {
	        return scalar;
	    }
	};
	template<class T>
	struct get_value_impl<T, std::enable_if_t<!std::is_same<decltype(std::declval<T&>()[0]), void>::value>>  {
	    static auto impl(T scalar) {
	        return scalar[0];
	    }
	};

	template<class T>
	static auto get_value(T scalar) {
		return get_value_impl<T>::impl(scalar);
	}

	template<class U, class T, class V>
	static void scalar_mul(U& eval, const T& a, const V& b) {
		eval = get_value(a) * get_value(b);
	}
	template<class U, class T, class V>
	static void scalar_mul(U* eval, const T& a, const V& b) {
		eval[0] = get_value(a) * get_value(b);
	}


	template<class T>
	static T static_allocate(T value) {
		return T(value);
	}

    static void gemm(bool transA, bool transB, BC::size_t  m, BC::size_t  n, BC::size_t  k,
            const float alpha, const float* A, BC::size_t  lda,
                                const float* B, BC::size_t  ldb,
            const float beta,           float* C, BC::size_t  ldc) {

        auto TRANS_A =  transA ? CblasTrans : CblasNoTrans;
        auto TRANS_B =  transB ? CblasTrans : CblasNoTrans;

        cblas_sgemm(CblasColMajor, TRANS_A, TRANS_B, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
    }

    static void gemm(bool transA, bool transB, BC::size_t  m, BC::size_t  n, BC::size_t  k,
            const double alpha, const double* A, BC::size_t  lda,
                                 const double* B, BC::size_t  ldb,
            const double beta,        double* C, BC::size_t  ldc) {

        auto TRANS_A =  transA ? CblasTrans : CblasNoTrans;
        auto TRANS_B =  transB ? CblasTrans : CblasNoTrans;

        cblas_dgemm(CblasColMajor, TRANS_A, TRANS_B, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
    }

    //y := alpha*A*x + beta*y,   or   y := alpha*A**T*x + beta*y,
    static void gemv(bool transA, BC::size_t  m, BC::size_t  n,
            const double alpha, const double* A, BC::size_t  lda,
                                 const double* X, BC::size_t  incX,
            const double beta,        double* Y, BC::size_t  incY) {

        auto TRANS_A =  transA ? CblasTrans : CblasNoTrans;

        cblas_dgemv(CblasColMajor, TRANS_A, m, n, alpha, A, lda, X, incX, beta, Y, incY);
    }
    static void gemv(bool transA, BC::size_t  m, BC::size_t  n,
            const float alpha, const float* A, BC::size_t  lda,
                                const float* X, BC::size_t  incX,
            const float beta,              float* Y, BC::size_t  incY) {

        auto TRANS_A =  transA ? CblasTrans : CblasNoTrans;

        cblas_sgemv(CblasColMajor, TRANS_A, m, n, alpha, A, lda, X, incX, beta, Y, incY);
    }

    static void ger(int m, BC::size_t  n,
            const double alpha,
                                 const double* X, BC::size_t  incX,
                                 const double* Y, BC::size_t  incY,
                                  double* A, BC::size_t  lda) {

        cblas_dger(CblasColMajor, m, n, alpha, X, incX, Y, incY, A, lda);
    }
    static void ger(int m, BC::size_t  n,
            const float alpha,
                                 const float* X, BC::size_t  incX,
                                 const float* Y, BC::size_t  incY,
                                  float* A, BC::size_t  lda) {

        cblas_sger(CblasColMajor, m, n, alpha, X, incX, Y, incY, A, lda);
    }


    static void dot(int n, double* A, const double* x, BC::size_t  incX, const double* y, BC::size_t  incY) {
        *A = cblas_ddot(n, x, incX, y, incY);
    }
    static void dot(int n, float* A, const float* x, BC::size_t  incX, const float* y, BC::size_t  incY) {
        *A = cblas_sdot(n, x, incX, y, incY);
    }
};
}

}


#endif /* HOST_H_ */
