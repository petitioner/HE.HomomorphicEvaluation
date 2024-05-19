#pragma once

#include<iostream> 

#include "Key.h"
#include "Ciphertext.h"
#include "SecretKey.h"

using namespace std;

class RotationManager {
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

	Key conjugationKey;
	map<long, Key> leftRotKeyMap; ///< contain left rotation keys, if generated

	RotationManager() {
	}  // 構造函數的聲明
	// 其他成員函數的聲明
	void init(long logN, long logQ, double sigma, long h);

	void addConjKey(SecretKey &secretKey);
	void addLeftRotKey(SecretKey &secretKey, long rot);

	Ciphertext leftRotateFast(Ciphertext &cipher, long rotSlots);
	void leftRotateAndEqualFast(Ciphertext &cipher, long rotSlots);

	Ciphertext conjugate(Ciphertext &cipher);
	void conjugateAndEqual(Ciphertext &cipher);

};

