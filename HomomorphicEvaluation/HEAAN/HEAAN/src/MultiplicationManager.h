#pragma once

#include<iostream> 

#include "Key.h"
#include "Ciphertext.h"
#include "SecretKey.h"

using namespace std;

class MultiplicationManager {
public:
	long logN; ///< log of N
	long logQ; ///< log of Q
	double sigma; ///< standard deviation for Gaussian distribution
	long h; ///< parameter for HWT distribution

	long N; ///< N is a power-of-two that corresponds to the ring Z[X]/(X^N + 1)
	long Nh; ///< Nh = N/2
	long logNh; ///< logNh = logN - 1
	long M; ///< M = 2N
	long logQQ; ///< log of PQ

	ZZ Q; ///< Q corresponds to the highest modulus
	ZZ QQ; ///< PQ = Q * Q

	Key multiplicationKey;

	MultiplicationManager() {
	}  // 構造函數的聲明
	// 其他成員函數的聲明
	void init(long logN, long logQ, double sigma, long h);

	void addMultKey(SecretKey &secretKey);

	Ciphertext mult(Ciphertext &cipher1, Ciphertext &cipher2);

	void multAndEqual(Ciphertext &cipher1, Ciphertext &cipher2);

	void imultAndEqual(Ciphertext &cipher);

	Ciphertext multByConst(Ciphertext &cipher, double cnst, long logp);

	Ciphertext multByConstVec(Ciphertext &cipher, double *cnstVec, long slots,
			long logp);

	void multByConstAndEqual(Ciphertext &cipher, double cnst, long logp);

	/************ for bootstrapping ****************/
	Ciphertext multByPoly(Ciphertext &cipher, ZZX &poly, long logp);
	void multByPolyAndEqual(Ciphertext &cipher, ZZX &poly, long logp);
	void multByConstAndEqual(Ciphertext &cipher, RR &cnst, long logp);

};

