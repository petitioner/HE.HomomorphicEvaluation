
#include "SubstractionManager.h"
#include "EvaluatorUtils.h"
#include "Ring.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>



void SubstractionManager::init(long logN, long logQ, double sigma, long h) {
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

Ciphertext SubstractionManager::sub(Ciphertext &cipher1, Ciphertext &cipher2) {
	ZZ q = ZZ(1) << cipher1.logq;
	ZZX ax, bx;
	ZZX ct2ax, ct2bx;
	NTL::negate(ct2ax, cipher2.ax);
	NTL::negate(ct2bx, cipher2.bx);

	Ring::add(ax, cipher1.ax, ct2ax, q, N);
	Ring::add(bx, cipher1.bx, ct2bx, q, N);

	return Ciphertext(ax, bx, cipher1.logp, cipher1.logq, cipher1.slots,
			cipher1.isComplex, cipher1.N);
}

void SubstractionManager::subAndEqual(Ciphertext &cipher1,
		Ciphertext &cipher2) {
	ZZ q = ZZ(1) << cipher1.logq;
	ZZX ct2ax, ct2bx;
	NTL::negate(ct2ax, cipher2.ax);
	NTL::negate(ct2bx, cipher2.bx);

	Ring::addAndEqual(cipher1.ax, ct2ax, q, N);
	Ring::addAndEqual(cipher1.bx, ct2bx, q, N);
}

void SubstractionManager::subAndEqual2(Ciphertext &cipher1,
		Ciphertext &cipher2) {
	ZZ q = ZZ(1) << cipher1.logq;

	Ring::subAndEqual2(cipher1.ax, cipher2.ax, q, N);
	Ring::subAndEqual2(cipher1.bx, cipher2.bx, q, N);
}

Ciphertext SubstractionManager::subConst(Ciphertext &cipher, double cnst,
		long logp) {
	ZZ q = ZZ(1) << cipher.logq;

	ZZX ax = cipher.ax;
	ZZX bx = cipher.bx;

	ZZ cnstZZ =
			logp < 0 ?
					EvaluatorUtils::scaleUpToZZ(-cnst, cipher.logp) :
					EvaluatorUtils::scaleUpToZZ(-cnst, logp);

	AddMod(bx.rep[0], cipher.bx.rep[0], cnstZZ, q);
	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots,
			cipher.isComplex, cipher.N);
}

void SubstractionManager::subConstAndEqual(Ciphertext &cipher, double cnst,
		long logp) {
	ZZ q = ZZ(1) << cipher.logq;
	ZZ cnstZZ =
			logp < 0 ?
					EvaluatorUtils::scaleUpToZZ(-cnst, cipher.logp) :
					EvaluatorUtils::scaleUpToZZ(-cnst, logp);
	AddMod(cipher.bx.rep[0], cipher.bx.rep[0], cnstZZ, q);
}


