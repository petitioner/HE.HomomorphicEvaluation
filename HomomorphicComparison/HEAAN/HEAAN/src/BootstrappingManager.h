#pragma once

#include<iostream> 

#include "Key.h"
#include "Ciphertext.h"

#include "EncryptionManager.h"
#include "DecryptionManager.h"
#include "AdditionManager.h"
#include "SubstractionManager.h"
#include "MultiplicationManager.h"
#include "RotationManager.h"

using namespace std;

class BootstrappingManager {
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

	EncryptionManager &encryptionManager;
	DecryptionManager &decryptionManager;
	AdditionManager &additionManager;
	SubstractionManager &substractionManager;
	MultiplicationManager &multiplicationManager;
	RotationManager &rotationManager;

	// 内部类 InnerData，用于存储不同类型的数据： struct  Tuple  Union
	class BootContext {
	public:
		ZZX *pvec; ///< encodings of "diagonal" values of CoeffToSlot matrix
		ZZX *pvecInv; ///< encodings of "diagonal" values of SlotToCoeff matrix

		ZZX p1; ///< auxiliary encoding for EvalExp
		ZZX p2; ///< auxiliary encoding for EvalExp

		long logp; ///< number of quantized bits

		BootContext(ZZX *pvec, ZZX *pvecInv, ZZX p1, ZZX p2, long logp) :
				pvec(pvec), pvecInv(pvecInv), p1(p1), p2(p2), logp(logp) {
		}
	};

	map<long, BootContext> bootContextMap; ///< precomputed bootstrapping auxiliary information

	BootstrappingManager(EncryptionManager &encryptionManager,
			DecryptionManager &decryptionManager,
			AdditionManager &additionManager,
			SubstractionManager &substractionManager,
			MultiplicationManager &multiplicationManager,
			RotationManager &rotationManager) :
			encryptionManager(encryptionManager), decryptionManager(
					decryptionManager), additionManager(additionManager), substractionManager(
					substractionManager), multiplicationManager(
					multiplicationManager), rotationManager(rotationManager) {
	}  // 構造函數的聲明
	// 其他成員函數的聲明
	void init(long logN, long logQ, double sigma, long h);

	void addBootKey(SecretKey &secretKey, long logl, long logp);



	void divByPo2AndEqual(Ciphertext &cipher, long degree);

	void idivAndEqual(Ciphertext &cipher);

	//----------------------------------------------------------------------------------
	//   ADDITIONAL METHODS FOR BOOTSTRAPPING
	//----------------------------------------------------------------------------------

	void normalizeAndEqual(Ciphertext &cipher);

	void coeffToSlotAndEqual(Ciphertext &cipher);

	void slotToCoeffAndEqual(Ciphertext &cipher);

	void exp2piAndEqual(Ciphertext &cipher, long logp);

	void evalExpAndEqual(Ciphertext &cipher, long logT, long logI = 4);

	void bootstrapAndEqual(Ciphertext &cipher, long logq, long logQ, long logT,
			long logI = 4);

};

