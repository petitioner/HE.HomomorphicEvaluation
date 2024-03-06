#pragma once

#include<iostream>

#include "Key.h"
#include "Ciphertext.h"

using namespace std;

class SubstractionManager {
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

	Key encryptionKey;

	SubstractionManager() {
	}  // 構造函數的聲明
	// 其他成員函數的聲明
	void init(long logN, long logQ, double sigma, long h);

	Ciphertext sub(Ciphertext &cipher1, Ciphertext &cipher2);

	void subAndEqual(Ciphertext &cipher1, Ciphertext &cipher2);

	void subAndEqual2(Ciphertext &cipher1, Ciphertext &cipher2);

	Ciphertext subConst(Ciphertext &cipher, double cnst, long logp = -1);
	//Ciphertext addByConstVec(Ciphertext& cipher, double* cnstVec, long slots, long logp);

	void subConstAndEqual(Ciphertext &cipher, double cnst, long logp = -1);

};

