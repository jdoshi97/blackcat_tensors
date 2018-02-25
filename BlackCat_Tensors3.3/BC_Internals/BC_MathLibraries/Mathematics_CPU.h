/*
 * BC_Mathematics_CPU.h
 *
 *  Created on: Dec 1, 2017
 *      Author: joseph
 */

#ifndef MATHEMATICS_CPU_H_
#define MATHEMATICS_CPU_H_

#include <cmath>
#include <iostream>
#include <string>

#include "BC_PrintFunctions.h"
#include "cblas.h"

namespace BC {

static constexpr int MAX_UNROLLED = 256;
static constexpr int DEFAULT_ROLL = 8;

class CPU {
public:
	template<typename T>
	static void initialize(T*& t, int sz) {
		t = new T[sz];
	}
	template<typename T>
	static void unified_initialize(T*& t, int sz) {
		t = new T[sz];
	}
	//The tensor library doesn't differentiate method calls between gpu and cpu libraries, ergo you must override certain methods.
	template<class T, class U>
	static void HostToDevice(T& t, U& u, int size) {
		copy(t, u, size);
	}
	template<class T, class U>
	static void DeviceToHost(T& t, U& u, int size) {
		copy(t, u, size);
	}
	template<class T, class U>
	static void HostToDevice(T* t, U* u, int size) {
		copy(t, u, size);
	}
	template<class T, class U>
	static void DeviceToHost(T* t, U* u, int size) {
		copy(t, u, size);
	}
	template<typename T>
	static void destroy(T* t) {
		delete[] t;
	}
	template<typename T, typename J, int roll = DEFAULT_ROLL>
	static void fill(T& t, const J j, int sz) {
		for (int i = 0; i < sz; ++i) {
			t[i] = j;
		}
	}

	template<typename T>
	static void eval(T& t, int sz) {
		for (int i = 0; i < sz; ++i) {
			t[i];
		}
	}
	template<typename T, typename J>
	static void set_heap(T *t, J *j) {
		*t = *j;
	}
	template<typename T, typename J>
	static void set_stack(T *t, J j) {
		*t = j;
	}

	template<typename T>
	static void zero(T& t, int sz) {
		fill(t, 0, sz);
	}





	template<typename T, typename J>
	static void copy(T& t, const J& j, int sz) {
		if (sz < 8192) {
			copy_single_thread(t, j, sz);
			return;
		}
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = j[i];
		}
#pragma omp barrier
	}


	template<typename T, typename J, int roll = DEFAULT_ROLL>
	static void copy_single_thread(T& t, const J& j, int sz) {
		for (int i = 0; i < sz; ++i) {
			t[i] = j[i];
		}
	}
	template<typename T, typename J>
	static void copyHostToDevice(T t, J j, int sz) {
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = j[i];
		}
#pragma omp barrier
	}

	template<typename T, typename J>
	static void randomize(T& t, J lower_bound, J upper_bound, int sz) {
#pragma omp parallel for
		for (int i = 0; i < sz; ++i) {
			t[i] = ((double) (rand() / ((double) RAND_MAX + 1)) * (upper_bound - lower_bound)) + lower_bound;
		}
#pragma omp barrier
	}
	template<class T, class RANKS>
	static void print(const T ary, const RANKS ranks, int order, int print_length) {
		BC::print(ary, ranks, order, print_length);
	}

	template<class T, class RANKS>
	static void printSparse(const T ary, const RANKS ranks, int order, int print_length) {
		BC::printSparse(ary, ranks, order, print_length);
	}



	/*
	 * a = M x K
	 * b = K x N
	 * c = M x N
	 */

	//
public:
	template<class U, class T, class V>
	static void scalarMul(U eval, T a, V b) {
		*eval = a[0] * b[0];
	}

	static void MatrixMul(bool transA, bool transB, const float* A, const float* B, float* C, int m, int n, int k,
			const float* scalarA = nullptr, const float* scalarB = nullptr,  int lda = 0, int ldb =0, int ldc =0) {

		 auto TRANS_A =  transA ? CblasTrans : CblasNoTrans;
		 auto TRANS_B =  transB ? CblasTrans : CblasNoTrans;

		  if (lda == 0 ) lda = m;
		  if (ldb == 0 ) ldb = n;
		  if (ldc == 0 ) ldc = m;

	      const float beta   =  scalarB ? *scalarB : 0;
		  const float alpha  =  scalarA ? *scalarA : 1;

		cblas_sgemm(CblasColMajor, TRANS_A, TRANS_B, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
	}
	static void MatrixMul(bool transA, bool transB, const double* A, const double* B, double* C, int m, int n, int k,
			const double* scalarA = nullptr, const double* scalarB = nullptr, int lda = 0, int ldb = 0, int ldc = 0) {

		 auto TRANS_A =  transA ? CblasTrans : CblasNoTrans;
		 auto TRANS_B =  transB ? CblasTrans : CblasNoTrans;

		  if (lda == 0 ) lda = m;
		  if (ldb == 0 ) ldb = n;
		  if (ldc == 0 ) ldc = m;


		const double beta   =  scalarB ? *scalarB : 0.0;
		const double alpha  =  scalarA ? *scalarA : 1.0;
		cblas_dgemm(CblasColMajor, TRANS_A, TRANS_B, m, n, k, alpha, A, lda, B, ldb, beta, C, ldc);
	}

};
}
#endif /* MATHEMATICS_CPU_H_ */
//
//	template<class V, class T, class U>
//	static void x_correlation(V* cor, const T* img, const U* krnl, int imgRank, int* imgD, int* imgLD, int krnlRank, int* krnlD, int* krnlLD, int* corLD) {
//		//corLD rank will always by the same as the img rank
//
//		//if the dimensions of the img are greater than the dimensions of the
//		if (imgRank > krnlRank) {
//			int last = imgRank - 1;
//			for (int i = 0; i < imgD[last]; ++i) {
//				//we do not move the krnl as we are getting the "starting" indexes for each correlation --
//				//we choose this format of changing each correlation into 1d correlation to ensure cache efficiency
//				x_correlation(cor[corLD[last] * i], img[imgLD[last] * i], krnl, imgRank - 1, imgD, imgLD, krnlRank, krnlD, krnlLD);
//			}
//			return;
//		}
//
//		//imgRank and KrnlRank will be the same at this point
//		if (imgRank == 1) {
//			//convert to 1d correlation
//			return x_correlattion1d(cor, img, krnl, imgD[0], krnlD[0]);
//		}
////------------------------------------------------------------------------------------------------------------------------------------
//		//possibly move to another method as imgRank now becomes obsolete (as it is the same as krnlRank)
//		//Handle correlation for dimensionality > 1
//		const int N_POS = imgD[imgRank] + krnlD[imgRank] - 1;
//		const int N_POS_WITHIN_BONDRIES  = imgD[imgRank] - krnlD[imgRank] + 1;
//		const int KRNL_0_POSITIONS = krnlD[imgRank] - 1;
//		const int KRNL_LAST =  krnlD[imgRank] - 1;
//		const int IMG_LAST  = imgD[imgRank] - 1;
//		const int COR_LAST = N_POS  - 1;
//		const int last = imgRank - 1;
//
//
//
//
//		//////THIS CODE DOES NOT WORK YET (DID NOT FINISH WRITING))) ////////////////// ATTEMPT TO HARDCODE FOR SMALLER DIMENSIONS FIRST
//		//////THIS CODE
//
//		///automatically pad for upper bound
//		for (int i = 0; i < KRNL_0_POSITIONS; ++i) {
//			for (int j = 0; j < i + 1; ++j){
//				x_correlation(cor[corLD[last] * i], img[imgLD[last] * i], krnl[krnlLD[last] * i], last, imgD, imgLD, last, krnlD, krnlLD, corLD);
//			}
//		}
////
////		const int BASE  = KRNL_0_POSITIONS; //The base represents the offset of the
////		for (int i = 0; i < N_POS_WITHIN_BONDRIES; ++i) {
////			for (int j = 0; j < krnl_l; ++j) {
////				cor[BASE + i] = krnl[j] * img[i + j];
////
////				x_correlation(cor[corLD[last] * i], img[imgLD[last] * i], krnl[krnlLD[last] * i], last, imgD, imgLD, last, krnlD, krnlLD, corLD);
////
////
////			}
////		}
////
////		///automatically pad for upper bound
////		for (int i = 0; i < KRNL_0_POSITIONS; ++i) {
////			for (int j = 0; j < i + 1; ++j){
////				cor[COR_LAST - j] = krnl[j] * img[IMG_LAST - i + j];
////			}
////		}
//
//
//
//
//
//	}
//
//	template<class V, class T, class U>
//	static void x_correlation1d(V& cor, const T& img, const U& krnl, int img_l, int krnl_l) {
//		///1 dimensional cross correlation (automatically 0 padded)
//		const int N_POS = img_l + krnl_l - 1;
//		const int N_POS_WITHIN_BONDRIES  = img_l - krnl_l + 1;
//		const int KRNL_0_POSITIONS = krnl_l - 1;
//		const int KRNL_LAST =  krnl_l - 1;
//		const int IMG_LAST  = img_l - 1;
//		const int COR_LAST = N_POS  - 1;
//
//		//Handle the issues where the krnl would be outof bounds for the img ( upper bound )
//		for (int i = 0; i < KRNL_0_POSITIONS; ++i) {
//			for (int j = 0; j < i + 1; ++j){
//				cor[j] += krnl[KRNL_LAST - i + j] * img[j];
//			}
//		}
//
//		const int BASE  = KRNL_0_POSITIONS; //The base represents the offset for the output indexes (because we are interperting as padded)
//		for (int i = 0; i < N_POS_WITHIN_BONDRIES; ++i) {
//			for (int j = 0; j < krnl_l; ++j) {
//				cor[BASE + i] += krnl[j] * img[i + j];
//			}
//		}
//
//		//Handle the issues where the krnl would be outof bounds for the img ( lower bound )
//		for (int i = 0; i < KRNL_0_POSITIONS; ++i) {
//			for (int j = 0; j < i + 1; ++j){
//				cor[COR_LAST - j] += krnl[j] * img[IMG_LAST - i + j];
//			}
//		}
//	}
//
//};


//	//This doesn't actually give a shit if transposed --- just using this till I integrate blas
//	template<class A, class B, class C, class D, class E>
//	static void MatrixMul(bool transA, bool transB, const A a, const B b,  C c, int m, int n, int k, const D scalarA = nullptr, const E scalarB = nullptr,  int lda = 0, int ldb = 0, int ldc = 0) {
//		if (lda == 0 and ldb == 0 and ldc ==0) {
//			lda = m;
//			ldb = n;
//			ldc = m;
//		}
//
//		typename MTF::remove_mods<A>::type alpha =  scalarA == nullptr  ? 1 : *scalarA;
//		typename MTF::remove_mods<B>::type beta  =  scalarB == nullptr  ? 1 : *scalarB;
//
//#pragma omp parallel for
//		for (int z = 0; z < k; ++z) {
//			for (int x = 0; x < n; ++x) {
//				for (int y = 0; y < m; ++y) {
//					c[z * ldc + y] += (a[x * lda + y]* alpha) * (b[z * ldb + x] * beta);
//
//				}
//			}
//		}
//#pragma omp barrier
//	}
//
//	template<class T>
//	static void MatrixMul(bool transA, bool transB, const T a, const T b,  T c, int m, int n, int k, const T scalarA = nullptr, const T scalarB = nullptr,  int lda = 0, int ldb = 0, int ldc = 0) {
//		if (lda == 0 and ldb == 0 and ldc ==0) {
//			lda = m;
//			ldb = n;
//			ldc = m;
//		}
//
//		typename MTF::remove_mods<T>::type alpha =  scalarA == nullptr  ? 1 : *scalarA;
//		typename MTF::remove_mods<T>::type beta  =  scalarB == nullptr  ? 1 : *scalarB;
//
//#pragma omp parallel for
//		for (int z = 0; z < k; ++z) {
//			for (int x = 0; x < n; ++x) {
//				for (int y = 0; y < m; ++y) {
//					c[z * ldc + y] += (a[x * lda + y] * scalarA) * (b[z * ldb + x] * scalarB);
//
//				}
//			}
//		}
//#pragma omp barrier
//	}
