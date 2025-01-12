#pragma once

#include<iostream> 

#include "Key.h"
#include "Ciphertext.h"
#include "Plaintext.h"
#include "SecretKey.h"

using namespace std;

class DecryptionManager {
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

	SecretKey secretKey;

	DecryptionManager() {
	}  // 構造函數的聲明
	// 其他成員函數的聲明
	void init(long logN, long logQ, double sigma, long h);

	double* decode(Plaintext &msg);
	double decodeSingle(Plaintext &msg);

	Plaintext decryptMsg(SecretKey &secretKey, Ciphertext &cipher);
	double* decrypt(SecretKey &secretKey, Ciphertext &cipher);
	double decryptSingle(SecretKey &secretKey, Ciphertext &cipher);

};

