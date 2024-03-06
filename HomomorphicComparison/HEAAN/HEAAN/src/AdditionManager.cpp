
#include "AdditionManager.h"
#include "EvaluatorUtils.h"
#include "Ring.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>





void AdditionManager::init(long logN, long logQ, double sigma, long h) {
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

Ciphertext AdditionManager::add(Ciphertext &cipher1, Ciphertext &cipher2) {
	ZZ q = ZZ(1) << cipher1.logq;
	ZZX ax, bx;

	Ring::add(ax, cipher1.ax, cipher2.ax, q, N);
	Ring::add(bx, cipher1.bx, cipher2.bx, q, N);

	return Ciphertext(ax, bx, cipher1.logp, cipher1.logq, cipher1.slots,
			cipher1.isComplex, cipher1.N);
}

void AdditionManager::addAndEqual(Ciphertext &cipher1, Ciphertext &cipher2) {
	ZZ q = ZZ(1) << cipher1.logq;

	Ring::addAndEqual(cipher1.ax, cipher2.ax, q, N);
	Ring::addAndEqual(cipher1.bx, cipher2.bx, q, N);
}

Ciphertext AdditionManager::addConst(Ciphertext &cipher, double cnst,
		long logp) {
	ZZ q = ZZ(1) << cipher.logq;

	ZZX ax = cipher.ax;
	ZZX bx = cipher.bx;

	ZZ cnstZZ =
			logp < 0 ?
					EvaluatorUtils::scaleUpToZZ(cnst, cipher.logp) :
					EvaluatorUtils::scaleUpToZZ(cnst, logp);

	AddMod(bx.rep[0], cipher.bx.rep[0], cnstZZ, q);
	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots,
			cipher.isComplex, cipher.N);
}

void AdditionManager::addConstAndEqual(Ciphertext &cipher, double cnst,
		long logp) {
	ZZ q = ZZ(1) << cipher.logq;
	ZZ cnstZZ =
			logp < 0 ?
					EvaluatorUtils::scaleUpToZZ(cnst, cipher.logp) :
					EvaluatorUtils::scaleUpToZZ(cnst, logp);
	AddMod(cipher.bx.rep[0], cipher.bx.rep[0], cnstZZ, q);
}

/************ for bootstrapping ****************/
Ciphertext AdditionManager::addConst(Ciphertext &cipher, RR &cnst, long logp) {
	ZZ q = ZZ(1) << cipher.logq;

	ZZX ax = cipher.ax;
	ZZX bx = cipher.bx;

	ZZ cnstZZ =
			logp < 0 ?
					EvaluatorUtils::scaleUpToZZ(cnst, cipher.logp) :
					EvaluatorUtils::scaleUpToZZ(cnst, logp);

	AddMod(bx.rep[0], cipher.bx.rep[0], cnstZZ, q);
	return Ciphertext(ax, bx, cipher.logp, cipher.logq, cipher.slots,
			cipher.isComplex, cipher.N);
}

void AdditionManager::addConstAndEqual(Ciphertext &cipher, RR &cnst,
		long logp) {
	ZZ q = ZZ(1) << cipher.logq;
	ZZ cnstZZ =
			logp < 0 ?
					EvaluatorUtils::scaleUpToZZ(cnst, cipher.logp) :
					EvaluatorUtils::scaleUpToZZ(cnst, logp);
	AddMod(cipher.bx.rep[0], cipher.bx.rep[0], cnstZZ, q);
}


