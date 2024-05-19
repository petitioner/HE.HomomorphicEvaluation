
#include "Scheme.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>

#include "EvaluatorUtils.h"
#include "NumUtils.h"
#include "Ring.h"
#include "StringUtils.h"

//-----------------------------------------
/*
 Scheme::Scheme(long logN, long logQ, double sigma, long h) : logN(logN), logQ(logQ), sigma(sigma), h(h)  {}
 */
Scheme::Scheme(SecretKey &secretKey, long logN, long logQ, double sigma, long h) :
		secretKey(secretKey), logN(logN), logQ(logQ), sigma(sigma), h(h) {
	N = 1 << logN;
	Nh = N >> 1;
	logNh = logN - 1;
	M = N << 1;
	logQQ = logQ << 1;
	Q = power2_ZZ(logQ);
	QQ = power2_ZZ(logQQ);

	//this->encryptionManager.context = context;
	//this->encryptionManager.encryptionKey = encryptionKey;
	this->encryptionManager.init(logN, logQ, sigma, h);
	this->encryptionManager.addEncKey(secretKey);
	//this->decryptionManager.context = context;
	this->decryptionManager.secretKey = secretKey;
	this->decryptionManager.init(logN, logQ, sigma, h);
	//this->additionManager.context = context;
	this->additionManager.init(logN, logQ, sigma, h);
	//this->substractionManager.context = context;
	this->substractionManager.init(logN, logQ, sigma, h);
	//this->multiplicationManager.context = context;
	//this->multiplicationManager.multiplicationKey = multiplicationKey;
	this->multiplicationManager.init(logN, logQ, sigma, h);
	this->multiplicationManager.addMultKey(secretKey);
	//this->rotationManager.context = context;
	//this->rotationManager.conjugationKey = conjugationKey;
	/*  順序改變會導致bugs產生  */
	this->rotationManager.init(logN, logQ, sigma, h);
	this->rotationManager.addConjKey(secretKey);
	/*  順序改變會導致bugs產生  */

	this->bootstrappingManager = new BootstrappingManager(
			this->encryptionManager, this->decryptionManager,
			this->additionManager, this->substractionManager,
			this->multiplicationManager, this->rotationManager);
	this->bootstrappingManager->init(logN, logQ, sigma, h);
}
;

//----------------------------------------------------------------------------------
//   KEYS GENERATION
//----------------------------------------------------------------------------------

void Scheme::addEncKey(SecretKey &secretKey) {

	return this->encryptionManager.addEncKey(secretKey);
}

void Scheme::addMultKey(SecretKey &secretKey) {

	return this->multiplicationManager.addMultKey(secretKey);
}

void Scheme::addConjKey(SecretKey &secretKey) {

	return this->rotationManager.addConjKey(secretKey);
}

void Scheme::addLeftRotKey(SecretKey &secretKey, long rot) {

	return this->rotationManager.addLeftRotKey(secretKey, rot);
}

void Scheme::addBootKey(SecretKey &secretKey, long logSlots, long logp) {

	return this->bootstrappingManager->addBootKey(secretKey, logSlots, logp);
}

//----------------------------------------------------------------------------------
//   ENCRYPTION & DECRYPTION
//----------------------------------------------------------------------------------

Ciphertext Scheme::encrypt(double *vals, long slots, long logp, long logq) {

	return this->encryptionManager.encrypt(vals, slots, logp, logq);
}

double* Scheme::decrypt(SecretKey &secretKey, Ciphertext &cipher) {

	return this->decryptionManager.decrypt(secretKey, cipher);
}

Ciphertext Scheme::encrypt(double val, long logp, long logq) {

	return this->encryptionManager.encrypt(val, logp, logq);
}

double Scheme::decryptSingle(SecretKey &secretKey, Ciphertext &cipher) {

	return this->decryptionManager.decryptSingle(secretKey, cipher);
}

//----------------------------------------------------------------------------------
//   HOMOMORPHIC OPERATIONS
//----------------------------------------------------------------------------------

Ciphertext Scheme::add(Ciphertext &cipher1, Ciphertext &cipher2) {

	return this->additionManager.add(cipher1, cipher2);
}

void Scheme::addAndEqual(Ciphertext &cipher1, Ciphertext &cipher2) {

	return this->additionManager.addAndEqual(cipher1, cipher2);
}

Ciphertext Scheme::addConst(Ciphertext &cipher, double cnst, long logp) {

	return this->additionManager.addConst(cipher, cnst, logp);
}

void Scheme::addConstAndEqual(Ciphertext &cipher, double cnst, long logp) {

	return this->additionManager.addConstAndEqual(cipher, cnst, logp);
}

Ciphertext Scheme::sub(Ciphertext &cipher1, Ciphertext &cipher2) {

	return this->substractionManager.sub(cipher1, cipher2);
}

void Scheme::subAndEqual(Ciphertext &cipher1, Ciphertext &cipher2) {

	return this->substractionManager.subAndEqual(cipher1, cipher2);
}

void Scheme::subAndEqual2(Ciphertext &cipher1, Ciphertext &cipher2) {

	return this->substractionManager.subAndEqual2(cipher1, cipher2);
}

Ciphertext Scheme::subConst(Ciphertext &cipher, double cnst, long logp) {

	return this->substractionManager.subConst(cipher, cnst, logp);
}

void Scheme::subConstAndEqual(Ciphertext &cipher, double cnst, long logp) {

	return this->substractionManager.subConstAndEqual(cipher, cnst, logp);
}

Ciphertext Scheme::mult(Ciphertext &cipher1, Ciphertext &cipher2) {

	return this->multiplicationManager.mult(cipher1, cipher2);
}

void Scheme::multAndEqual(Ciphertext &cipher1, Ciphertext &cipher2) {

	return this->multiplicationManager.multAndEqual(cipher1, cipher2);
}

void Scheme::imultAndEqual(Ciphertext &cipher) {

	return this->multiplicationManager.imultAndEqual(cipher);
}

Ciphertext Scheme::multByConst(Ciphertext &cipher, double cnst, long logp) {

	return this->multiplicationManager.multByConst(cipher, cnst, logp);
}

Ciphertext Scheme::multByConstVec(Ciphertext &cipher, double *cnstVec,
		long slots, long logp) {

	return this->multiplicationManager.multByConstVec(cipher, cnstVec, slots,
			logp);
}

void Scheme::multByConstAndEqual(Ciphertext &cipher, double cnst, long logp) {

	return this->multiplicationManager.multByConstAndEqual(cipher, cnst, logp);
}

//----------------------------------------------------------------------------------
//   ROTATIONS & CONJUGATIONS
//----------------------------------------------------------------------------------

Ciphertext Scheme::leftRotateFast(Ciphertext &cipher, long rotSlots) {

	return this->rotationManager.leftRotateFast(cipher, rotSlots);
}

void Scheme::leftRotateAndEqualFast(Ciphertext &cipher, long rotSlots) {

	return this->rotationManager.leftRotateAndEqualFast(cipher, rotSlots);
}

Ciphertext Scheme::conjugate(Ciphertext &cipher) {

	return this->rotationManager.conjugate(cipher);
}

void Scheme::conjugateAndEqual(Ciphertext &cipher) {

	return this->rotationManager.conjugateAndEqual(cipher);
}

void Scheme::bootstrapAndEqual(Ciphertext &cipher, long logq, long logQ,
		long logT, long logI) {

	return this->bootstrappingManager->bootstrapAndEqual(cipher, logq, logQ,
			logT, logI);
}


