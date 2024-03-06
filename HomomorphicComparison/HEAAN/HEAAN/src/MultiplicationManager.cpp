
#include "MultiplicationManager.h"
#include "EvaluatorUtils.h"
#include "NumUtils.h"
#include "Ring.h"
#include "FFT.h"


#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>



void MultiplicationManager::init(long logN, long logQ, double sigma, long h) {
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

void MultiplicationManager::addMultKey(SecretKey &secretKey) {
	ZZX ex, ax, bx, sxsx;

	Ring::mult(sxsx, secretKey.sx, secretKey.sx, Q, N);
	Ring::leftShiftAndEqual(sxsx, logQ, QQ, N);
	NumUtils::sampleUniform2(ax, N, logQQ);
	NumUtils::sampleGauss(ex, N, sigma);
	Ring::addAndEqual(ex, sxsx, QQ, N);
	Ring::mult(bx, secretKey.sx, ax, QQ, N);
	Ring::sub(bx, ex, bx, QQ, N);

	//keyMap.insert(pair<long, Key>(MULTIPLICATION, Key(ax, bx)));
	multiplicationKey = Key(ax, bx);
}

Ciphertext MultiplicationManager::mult(Ciphertext &cipher1,
		Ciphertext &cipher2) {
	ZZ q = ZZ(1) << cipher1.logq;
	ZZ qQ = ZZ(1) << (cipher1.logq + logQ);

	ZZX axbx1, axbx2, axax, bxbx, axmult, bxmult;
	//Key key = keyMap.at(MULTIPLICATION);

	Ring::add(axbx1, cipher1.ax, cipher1.bx, q, N);
	Ring::add(axbx2, cipher2.ax, cipher2.bx, q, N);
	Ring::multAndEqual(axbx1, axbx2, q, N);

	Ring::mult(axax, cipher1.ax, cipher2.ax, q, N);
	Ring::mult(bxbx, cipher1.bx, cipher2.bx, q, N);

	Ring::mult(axmult, axax, multiplicationKey.ax, qQ, N);
	Ring::mult(bxmult, axax, multiplicationKey.bx, qQ, N);

	Ring::rightShiftAndEqual(axmult, logQ, N);
	Ring::rightShiftAndEqual(bxmult, logQ, N);

	Ring::addAndEqual(axmult, axbx1, q, N);
	Ring::subAndEqual(axmult, bxbx, q, N);
	Ring::subAndEqual(axmult, axax, q, N);
	Ring::addAndEqual(bxmult, bxbx, q, N);

	return Ciphertext(axmult, bxmult, cipher1.logp + cipher2.logp, cipher1.logq,
			cipher1.slots, cipher1.isComplex, cipher1.N);
}

void MultiplicationManager::multAndEqual(Ciphertext &cipher1,
		Ciphertext &cipher2) {
	ZZ q = ZZ(1) << cipher1.logq;
	ZZ qQ = ZZ(1) << (cipher1.logq + logQ);
	ZZX axbx1, axbx2, axax, bxbx;
	//Key key = keyMap.at(MULTIPLICATION);

	Ring::add(axbx1, cipher1.ax, cipher1.bx, q, N);
	Ring::add(axbx2, cipher2.ax, cipher2.bx, q, N);
	Ring::multAndEqual(axbx1, axbx2, q, N);

	Ring::mult(axax, cipher1.ax, cipher2.ax, q, N);
	Ring::mult(bxbx, cipher1.bx, cipher2.bx, q, N);

	Ring::mult(cipher1.ax, axax, multiplicationKey.ax, qQ, N);
	Ring::mult(cipher1.bx, axax, multiplicationKey.bx, qQ, N);

	Ring::rightShiftAndEqual(cipher1.ax, logQ, N);
	Ring::rightShiftAndEqual(cipher1.bx, logQ, N);

	Ring::addAndEqual(cipher1.ax, axbx1, q, N);
	Ring::subAndEqual(cipher1.ax, bxbx, q, N);
	Ring::subAndEqual(cipher1.ax, axax, q, N);
	Ring::addAndEqual(cipher1.bx, bxbx, q, N);

	cipher1.logp += cipher2.logp;
}

void MultiplicationManager::imultAndEqual(Ciphertext &cipher) {
	Ring::multByMonomialAndEqual(cipher.ax, Nh, N);
	Ring::multByMonomialAndEqual(cipher.bx, Nh, N);
}

Ciphertext MultiplicationManager::multByConst(Ciphertext &cipher, double cnst,
		long logp) {
	ZZ q = ZZ(1) << cipher.logq;
	ZZX ax, bx;

	ZZ cnstZZ = EvaluatorUtils::scaleUpToZZ(cnst, logp);

	Ring::multByConst(ax, cipher.ax, cnstZZ, q, N);
	Ring::multByConst(bx, cipher.bx, cnstZZ, q, N);

	return Ciphertext(ax, bx, cipher.logp + logp, cipher.logq, cipher.slots,
			cipher.isComplex, cipher.N);
}

Ciphertext MultiplicationManager::multByConstVec(Ciphertext &cipher,
		double *cnstVec, long slots, long logp) {
	complex<double> *uvals = new complex<double> [slots];
	long i, jdx, idx;
	for (i = 0; i < slots; ++i) {
		uvals[i].real(cnstVec[i]);
	}

	ZZX cmx;
	cmx.SetLength(N);

	long gap = Nh / slots;

	FFT::fftSpecialInv(uvals, slots, M);

	for (i = 0, jdx = Nh, idx = 0; i < slots; ++i, jdx += gap, idx += gap) {
		cmx.rep[idx] = EvaluatorUtils::scaleUpToZZ(uvals[i].real(), logp);
		cmx.rep[jdx] = EvaluatorUtils::scaleUpToZZ(uvals[i].imag(), logp);
	}
	delete[] uvals;
	//return mx;
	//ZZX cmx = encode(cnstVec, slots, logp);

	ZZ q = ZZ(1) << cipher.logq;
	ZZX ax, bx;

	Ring::mult(ax, cipher.ax, cmx, q, N);
	Ring::mult(bx, cipher.bx, cmx, q, N);

	return Ciphertext(ax, bx, cipher.logp + logp, cipher.logq, cipher.slots,
			cipher.isComplex, cipher.N);
}

void MultiplicationManager::multByConstAndEqual(Ciphertext &cipher, double cnst,
		long logp) {
	ZZ q = ZZ(1) << cipher.logq;
	ZZ cnstZZ = EvaluatorUtils::scaleUpToZZ(cnst, logp);

	Ring::multByConstAndEqual(cipher.ax, cnstZZ, q, N);
	Ring::multByConstAndEqual(cipher.bx, cnstZZ, q, N);
	cipher.logp += logp;
}

/************ for bootstrapping ****************/
Ciphertext MultiplicationManager::multByPoly(Ciphertext &cipher, ZZX &poly,
		long logp) {
	ZZ q = ZZ(1) << cipher.logq;
	ZZX ax, bx;

	Ring::mult(ax, cipher.ax, poly, q, N);
	Ring::mult(bx, cipher.bx, poly, q, N);

	return Ciphertext(ax, bx, cipher.logp + logp, cipher.logq, cipher.slots,
			cipher.isComplex, cipher.N);
}

void MultiplicationManager::multByPolyAndEqual(Ciphertext &cipher, ZZX &poly,
		long logp) {
	ZZ q = ZZ(1) << cipher.logq;

	Ring::multAndEqual(cipher.ax, poly, q, N);
	Ring::multAndEqual(cipher.bx, poly, q, N);
	cipher.logp += logp;
}

void MultiplicationManager::multByConstAndEqual(Ciphertext &cipher, RR &cnst,
		long logp) {
	ZZ q = ZZ(1) << cipher.logq;
	ZZ cnstZZ = EvaluatorUtils::scaleUpToZZ(cnst, logp);

	Ring::multByConstAndEqual(cipher.ax, cnstZZ, q, N);
	Ring::multByConstAndEqual(cipher.bx, cnstZZ, q, N);
	cipher.logp += logp;
}


