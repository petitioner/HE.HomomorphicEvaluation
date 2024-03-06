#pragma once

#include<iostream> 

#include "Key.h"
#include "Ciphertext.h"


using namespace std;

class AdditionManager {
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

	AdditionManager() {
	}  // 構造函數的聲明
	// 其他成員函數的聲明
	void init(long logN, long logQ, double sigma, long h);

	Ciphertext add(Ciphertext &cipher1, Ciphertext &cipher2);

	void addAndEqual(Ciphertext &cipher1, Ciphertext &cipher2);

	Ciphertext addConst(Ciphertext &cipher, double cnst, long logp = -1);
	//Ciphertext addByConstVec(Ciphertext& cipher, double* cnstVec, long slots, long logp);

	void addConstAndEqual(Ciphertext &cipher, double cnst, long logp = -1);

	/************ for bootstrapping ****************/
	Ciphertext addConst(Ciphertext &cipher, RR &cnst, long logp = -1);
	void addConstAndEqual(Ciphertext &cipher, RR &cnst, long logp = -1);

};

