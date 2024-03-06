#pragma once

#include "Common.h"
#include "Ciphertext.h"
#include "Key.h"
#include "Plaintext.h"
#include "SecretKey.h"

#include <complex>

#include "EncryptionManager.h"
#include "DecryptionManager.h"
#include "AdditionManager.h"
#include "SubstractionManager.h"
#include "MultiplicationManager.h"
#include "RotationManager.h"
#include "BootstrappingManager.h"

using namespace std;
using namespace NTL;

class Scheme {
private:
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

	EncryptionManager encryptionManager;
	DecryptionManager decryptionManager;
	AdditionManager additionManager;
	SubstractionManager substractionManager;
	MultiplicationManager multiplicationManager;
	RotationManager rotationManager;
	BootstrappingManager *bootstrappingManager;

	SecretKey secretKey;
	//Key encryptionKey;
	//Key multiplicationKey;
	//Key conjugationKey;
//可以把 leftRotKeyMap 和 bootContextMap 放在scheme中 然後rotationManager bootstrappingManager用引用來管理這兩個Map 這樣的好處是可以通過scheme的get set方法很容易的管理旋轉公開密鑰
	//map<long, Key> leftRotKeyMap; ///< contain left rotation keys, if generated
//map<long, BootContext> bootContextMap; ///< precomputed bootstrapping auxiliary information

	Scheme(long logN, long logQ, double sigma = 3.2, long h = 64);
	Scheme(SecretKey &secretKey, long logN, long logQ, double sigma = 3.2,
			long h = 64);

	//----------------------------------------------------------------------------------
	//   KEYS GENERATION
	//----------------------------------------------------------------------------------

	void addEncKey(SecretKey &secretKey);

	void addConjKey(SecretKey &secretKey);

	void addMultKey(SecretKey &secretKey);

	void addLeftRotKey(SecretKey &secretKey, long rot);

	void addBootKey(SecretKey &secretKey, long logl, long logp);

	//----------------------------------------------------------------------------------
	//   ENCRYPTION & DECRYPTION
	//----------------------------------------------------------------------------------

	Ciphertext encrypt(double *vals, long slots, long logp, long logq);

	double* decrypt(SecretKey &secretKey, Ciphertext &cipher);

	Ciphertext encrypt(double val, long logp, long logq);

	double decryptSingle(SecretKey &secretKey, Ciphertext &cipher);

	//----------------------------------------------------------------------------------
	//   HOMOMORPHIC OPERATIONS
	//----------------------------------------------------------------------------------

	Ciphertext add(Ciphertext &cipher1, Ciphertext &cipher2);

	void addAndEqual(Ciphertext &cipher1, Ciphertext &cipher2);

	Ciphertext addConst(Ciphertext &cipher, double cnst, long logp = -1);

	void addConstAndEqual(Ciphertext &cipher, double cnst, long logp = -1);

	Ciphertext sub(Ciphertext &cipher1, Ciphertext &cipher2);

	void subAndEqual(Ciphertext &cipher1, Ciphertext &cipher2);
	void subAndEqual2(Ciphertext &cipher1, Ciphertext &cipher2);

	Ciphertext subConst(Ciphertext &cipher, double cnst, long logp = -1);
	//Ciphertext addByConstVec(Ciphertext& cipher, double* cnstVec, long slots, long logp);

	void subConstAndEqual(Ciphertext &cipher, double cnst, long logp = -1);

	Ciphertext mult(Ciphertext &cipher1, Ciphertext &cipher2);

	void multAndEqual(Ciphertext &cipher1, Ciphertext &cipher2);

	void imultAndEqual(Ciphertext &cipher);

	Ciphertext multByConst(Ciphertext &cipher, double cnst, long logp);

	Ciphertext multByConstVec(Ciphertext &cipher, double *cnstVec, long slots,
			long logp);

	void multByConstAndEqual(Ciphertext &cipher, double cnst, long logp);

	void multByConstVecAndEqual(Ciphertext &cipher, double *cnst, long slots,
			long logp);

	//----------------------------------------------------------------------------------
	//   ROTATIONS & CONJUGATIONS
	//----------------------------------------------------------------------------------

	Ciphertext leftRotateFast(Ciphertext &cipher, long rotSlots);

	void leftRotateAndEqualFast(Ciphertext &cipher, long rotSlots);

	Ciphertext conjugate(Ciphertext &cipher);

	void conjugateAndEqual(Ciphertext &cipher);

	//----------------------------------------------------------------------------------
	//   ADDITIONAL METHODS FOR BOOTSTRAPPING
	//----------------------------------------------------------------------------------

	void bootstrapAndEqual(Ciphertext &cipher, long logq, long logQ, long logT,
			long logI = 4);

};

