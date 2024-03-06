
#include "DecryptionManager.h"
#include "EvaluatorUtils.h"
#include "Ring.h"
#include "FFT.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>
#include <complex>



void DecryptionManager::init(long logN, long logQ, double sigma, long h) {
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

double* DecryptionManager::decode(Plaintext &msg) {
	ZZ q = ZZ(1) << msg.logq;  
	long slots = msg.slots;
	long gap = Nh / slots;
	complex<double> *res = new complex<double> [slots];
	ZZ tmp;

	for (long i = 0, idx = 0; i < slots; ++i, idx += gap) {
		rem(tmp, msg.mx[idx], q);
		if (NumBits(tmp) == msg.logq)
			tmp -= q;
		res[i].real(EvaluatorUtils::scaleDownToReal(tmp, msg.logp));

		rem(tmp, msg.mx[idx + Nh], q);
		if (NumBits(tmp) == msg.logq)
			tmp -= q;
		res[i].imag(EvaluatorUtils::scaleDownToReal(tmp, msg.logp));
	}
	FFT::fftSpecial(res, slots, M);
	double *rres = new double[slots];
	for (long i = 0; i < slots; ++i)
		rres[i] = res[i].real();
	return rres;
}

double DecryptionManager::decodeSingle(Plaintext &msg) {
	ZZ q = ZZ(1) << msg.logq;

	complex<double> res;
	ZZ tmp = msg.mx.rep[0] % q;
	if (NumBits(tmp) == msg.logq)
		tmp -= q;
	res.real(EvaluatorUtils::scaleDownToReal(tmp, msg.logp));

	if (msg.isComplex) {
		tmp = msg.mx.rep[Nh] % q;
		if (NumBits(tmp) == msg.logq)
			tmp -= q;
		res.imag(EvaluatorUtils::scaleDownToReal(tmp, msg.logp));
	}

	return res.real();
}

Plaintext DecryptionManager::decryptMsg(SecretKey &secretKey,
		Ciphertext &cipher) {
	ZZ q = ZZ(1) << cipher.logq;

	ZZX mx;
	Ring::mult(mx, cipher.ax, secretKey.sx, q, N);
	Ring::addAndEqual(mx, cipher.bx, q, N);

	return Plaintext(mx, cipher.logp, cipher.logq, cipher.slots,
			cipher.isComplex);
}

double* DecryptionManager::decrypt(SecretKey &secretKey, Ciphertext &cipher) {
	Plaintext msg = decryptMsg(secretKey, cipher);
	return decode(msg);
}

double DecryptionManager::decryptSingle(SecretKey &secretKey,
		Ciphertext &cipher) {
	Plaintext msg = decryptMsg(secretKey, cipher);
	return decodeSingle(msg);
}

