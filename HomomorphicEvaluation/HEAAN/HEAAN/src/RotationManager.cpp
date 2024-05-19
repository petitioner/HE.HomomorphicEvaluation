
#include "RotationManager.h"
#include "EvaluatorUtils.h"
#include "NumUtils.h"
#include "Ring.h"


#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>



void RotationManager::init(long logN, long logQ, double sigma, long h) {
	N = 1 << logN;
	Nh = N >> 1;
	logNh = logN - 1;
	M = N << 1;
	logQQ = logQ << 1;
	Q = power2_ZZ(logQ);
	QQ = power2_ZZ(logQQ);

	this->sigma = sigma;
	this->h = h;
	this->logN = logN;
	this->logQ = logQ;
}

void RotationManager::addConjKey(SecretKey &secretKey) {
	ZZX ex, ax, bx, sxconj;

	Ring::conjugate(sxconj, secretKey.sx, N);
	Ring::leftShiftAndEqual(sxconj, logQ, QQ, N);
	NumUtils::sampleUniform2(ax, N, logQQ);
	NumUtils::sampleGauss(ex, N, sigma);
	Ring::addAndEqual(ex, sxconj, QQ, N);
	Ring::mult(bx, secretKey.sx, ax, QQ, N);
	Ring::sub(bx, ex, bx, QQ, N);

	conjugationKey = Key(ax, bx);
}

void RotationManager::addLeftRotKey(SecretKey &secretKey, long rot) {
	ZZX ex, ax, bx, sxrot;
	long rotGroup = 1;
	long fivePows = 1; //idx
	for (long j = 0; j <= rot; ++j) {
		rotGroup = fivePows;
		fivePows *= 5;
		fivePows %= M;
	}
	Ring::inpower(sxrot, secretKey.sx, rotGroup, Q, N);
	Ring::leftShiftAndEqual(sxrot, logQ, QQ, N);
	NumUtils::sampleUniform2(ax, N, logQQ);
	NumUtils::sampleGauss(ex, N, sigma);
	Ring::addAndEqual(ex, sxrot, QQ, N);
	Ring::mult(bx, secretKey.sx, ax, QQ, N);
	Ring::sub(bx, ex, bx, QQ, N);

	leftRotKeyMap.insert(pair<long, Key>(rot, Key(ax, bx)));
}

Ciphertext RotationManager::leftRotateFast(Ciphertext &cipher, long rotSlots) {
	ZZ q = ZZ(1) << cipher.logq;
	ZZ qQ = ZZ(1) << (cipher.logq + logQ);

	ZZX bxrot, ax, bx;
	Key key = leftRotKeyMap.at(rotSlots);
	long rotGroup = 1;
	long fivePows = 1; //idx
	for (long j = 0; j <= rotSlots; ++j) {
		rotGroup = fivePows;
		fivePows *= 5;
		fivePows %= M;
	}
	Ring::inpower(bxrot, cipher.bx, rotGroup, Q, N);
	Ring::inpower(bx, cipher.ax, rotGroup, Q, N);

	Ring::mult(ax, bx, key.ax, qQ, N);
	Ring::multAndEqual(bx, key.bx, qQ, N);

	Ring::rightShiftAndEqual(ax, logQ, N);
	Ring::rightShiftAndEqual(bx, logQ, N);

	Ring::addAndEqual(bx, bxrot, q, N);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots,
			cipher.isComplex, cipher.N);
}

void RotationManager::leftRotateAndEqualFast(Ciphertext &cipher,
		long rotSlots) {
	ZZ q = ZZ(1) << cipher.logq;
	ZZ qQ = ZZ(1) << (cipher.logq + logQ);
	ZZX bxrot;
	Key key = leftRotKeyMap.at(rotSlots);
	long rotGroup = 1;
	long fivePows = 1; //idx
	for (long j = 0; j <= rotSlots; ++j) {
		rotGroup = fivePows;
		fivePows *= 5;
		fivePows %= M;
	}
	Ring::inpower(bxrot, cipher.bx, rotGroup, Q, N);
	Ring::inpower(cipher.bx, cipher.ax, rotGroup, Q, N);

	Ring::mult(cipher.ax, cipher.bx, key.ax, qQ, N);
	Ring::multAndEqual(cipher.bx, key.bx, qQ, N);

	Ring::rightShiftAndEqual(cipher.ax, logQ, N);
	Ring::rightShiftAndEqual(cipher.bx, logQ, N);

	Ring::addAndEqual(cipher.bx, bxrot, q, N);
}

Ciphertext RotationManager::conjugate(Ciphertext &cipher) {
	ZZ q = ZZ(1) << cipher.logq;
	ZZ qQ = ZZ(1) << (cipher.logq + logQ);

	ZZX bxconj, ax, bx;
	//Key key = keyMap.at(CONJUGATION);

	Ring::conjugate(bxconj, cipher.bx, N);
	Ring::conjugate(bx, cipher.ax, N);

	Ring::mult(ax, bx, conjugationKey.ax, qQ, N);
	Ring::multAndEqual(bx, conjugationKey.bx, qQ, N);

	Ring::rightShiftAndEqual(ax, logQ, N);
	Ring::rightShiftAndEqual(bx, logQ, N);

	Ring::addAndEqual(bx, bxconj, q, N);

	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots,
			cipher.isComplex, cipher.N);
}

void RotationManager::conjugateAndEqual(Ciphertext &cipher) {
	ZZ q = ZZ(1) << cipher.logq;
	ZZ qQ = ZZ(1) << (cipher.logq + logQ);
	ZZX bxconj;
	//Key key = keyMap.at(CONJUGATION);

	Ring::conjugate(bxconj, cipher.bx, N);
	Ring::conjugate(cipher.bx, cipher.ax, N);

	Ring::mult(cipher.ax, cipher.bx, conjugationKey.ax, qQ, N);
	Ring::multAndEqual(cipher.bx, conjugationKey.bx, qQ, N);

	Ring::rightShiftAndEqual(cipher.ax, logQ, N);
	Ring::rightShiftAndEqual(cipher.bx, logQ, N);

	Ring::addAndEqual(cipher.bx, bxconj, q, N);
}


