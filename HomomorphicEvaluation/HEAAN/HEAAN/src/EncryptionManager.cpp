
#include "EncryptionManager.h"
#include "EvaluatorUtils.h"
#include "NumUtils.h"
#include "Ring.h"
#include "FFT.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <complex>



void EncryptionManager::init(long logN, long logQ, double sigma, long h) {
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

void EncryptionManager::addEncKey(SecretKey &secretKey) {
	ZZX ex, ax, bx;

	NumUtils::sampleUniform2(ax, N, logQQ);
	NumUtils::sampleGauss(ex, N, sigma);
	Ring::mult(bx, secretKey.sx, ax, QQ, N);
	Ring::sub(bx, ex, bx, QQ, N);

	//keyMap.insert(pair<long, Key>(ENCRYPTION, Key(ax, bx)));
	encryptionKey = Key(ax, bx);
}

Plaintext EncryptionManager::encode(double *vals, long slots, long logp,
		long logq) {
	complex<double> *uvals = new complex<double> [slots];
	long i, jdx, idx;
	copy(vals, vals + slots, uvals);

	ZZX mx;
	mx.SetLength(N);
	long gap = Nh / slots;
	FFT::fftSpecialInv(uvals, slots, M);
	for (i = 0, jdx = Nh, idx = 0; i < slots; ++i, jdx += gap, idx += gap) {
		mx.rep[idx] = EvaluatorUtils::scaleUpToZZ(uvals[i].real(), logp + logQ);
		mx.rep[jdx] = EvaluatorUtils::scaleUpToZZ(uvals[i].imag(), logp + logQ);
	}
	delete[] uvals;
	//return mx;
	//ZZX mx = encode(vals, slots, logp + logQ);
	return Plaintext(mx, logp, logq, slots, false);
}

Plaintext EncryptionManager::encode(double val, long logp, long logq) {
	ZZX mx;
	mx.SetLength(N);
	mx.rep[0] = EvaluatorUtils::scaleUpToZZ(val, logp + logQ);
	return Plaintext(mx, logp, logq, 1, false);
}

Ciphertext EncryptionManager::encryptMsg(Plaintext &msg) {
	ZZX ax, bx, vx, ex;

	ZZ qQ = ZZ(1) << (msg.logq + logQ);

	NumUtils::sampleZO(vx, N);
	Ring::mult(ax, vx, encryptionKey.ax, qQ, N);
	NumUtils::sampleGauss(ex, N, sigma);
	Ring::addAndEqual(ax, ex, qQ, N);

	Ring::mult(bx, vx, encryptionKey.bx, qQ, N);
	NumUtils::sampleGauss(ex, N, sigma);
	Ring::addAndEqual(bx, ex, qQ, N);

	Ring::addAndEqual(bx, msg.mx, qQ, N);

	Ring::rightShiftAndEqual(ax, logQ, N);
	Ring::rightShiftAndEqual(bx, logQ, N);

	return Ciphertext(ax, bx, msg.logp, msg.logq, msg.slots, msg.isComplex, N);
}

Ciphertext EncryptionManager::encrypt(double val, long logp, long logq) {

	Plaintext msg = encode(val, logp, logq);

	return encryptMsg(msg);
}

Ciphertext EncryptionManager::encrypt(double *vals, long slots, long logp,
		long logq) {

	Plaintext msg = encode(vals, slots, logp, logq);

	return encryptMsg(msg);
}


