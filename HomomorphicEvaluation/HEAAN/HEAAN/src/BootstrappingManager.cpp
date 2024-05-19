
#include "BootstrappingManager.h"
#include "EvaluatorUtils.h"
#include "Ring.h"
#include "FFT.h"
#include "TimeUtils.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <NTL/ZZX.h>




void BootstrappingManager::init(long logN, long logQ, double sigma, long h) {
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

void BootstrappingManager::addBootKey(SecretKey &secretKey, long logSlots,
		long logp) {

	if (bootContextMap.find(logSlots) == bootContextMap.end()) {
		long slots = 1 << logSlots;
		long dslots = slots << 1;
		long logk = logSlots >> 1;

		long k = 1 << logk;
		long i, pos, ki, jdx, idx, deg;
		long gap = Nh >> logSlots;

		ZZX *pvecInv = new ZZX[slots];
		ZZX *pvec = new ZZX[slots];

		complex<double> *pvals = new complex<double> [dslots];

		ZZX p1, p2;
		double c = 0.25 / M_PI;

		if (logSlots < logNh) {
			long dgap = gap >> 1;
			for (ki = 0; ki < slots; ki += k) {
				for (pos = ki; pos < ki + k; ++pos) {
					for (i = 0; i < slots - pos; ++i) {

						long rotGroup = 1;
						long fivePows = 1;
						for (long index = 0; index <= i + pos; ++index) {
							rotGroup = fivePows;
							fivePows *= 5;
							fivePows %= M;
						}

						deg = ((M - rotGroup) * i * gap) % M;

						//ksiPows
						complex<double> ksiPows;
						double angle = 2.0 * M_PI * deg / M;
						ksiPows.real(cos(angle));
						ksiPows.imag(sin(angle));

						pvals[i] = ksiPows;
						pvals[i + slots].real(-pvals[i].imag());
						pvals[i + slots].imag(pvals[i].real());
					}
					for (i = slots - pos; i < slots; ++i) {

						long rotGroup = 1;
						long fivePows = 1;
						for (long index = 0; index <= i + pos - slots;
								++index) {
							rotGroup = fivePows;
							fivePows *= 5;
							fivePows %= M;
						}

						deg = ((M - rotGroup) * i * gap) % M;

						//ksiPows
						complex<double> ksiPows;
						double angle = 2.0 * M_PI * deg / M;
						ksiPows.real(cos(angle));
						ksiPows.imag(sin(angle));

						pvals[i] = ksiPows;
						pvals[i + slots].real(-pvals[i].imag());
						pvals[i + slots].imag(pvals[i].real());
					}
					EvaluatorUtils::rightRotateAndEqual(pvals, dslots, ki);
					FFT::fftSpecialInv(pvals, dslots, M);
					pvec[pos].SetLength(N);
					for (i = 0, jdx = Nh, idx = 0; i < dslots;
							++i, jdx += dgap, idx += dgap) {
						pvec[pos].rep[idx] = EvaluatorUtils::scaleUpToZZ(
								pvals[i].real(), logp);
						pvec[pos].rep[jdx] = EvaluatorUtils::scaleUpToZZ(
								pvals[i].imag(), logp);
					}
				}
			}
			for (i = 0; i < slots; ++i) {
				pvals[i] = 0.0;
				pvals[i + slots].real(0);
				pvals[i + slots].imag(-c);
			}
			p1.SetLength(N);
			FFT::fftSpecialInv(pvals, dslots, M);
			for (i = 0, jdx = Nh, idx = 0; i < dslots; ++i, jdx += dgap, idx +=
					dgap) {
				p1.rep[idx] = EvaluatorUtils::scaleUpToZZ(pvals[i].real(),
						logp);
				p1.rep[jdx] = EvaluatorUtils::scaleUpToZZ(pvals[i].imag(),
						logp);
			}

			for (i = 0; i < slots; ++i) {
				pvals[i] = c;
				pvals[i + slots] = 0;
			}

			p2.SetLength(N);
			FFT::fftSpecialInv(pvals, dslots, M);
			for (i = 0, jdx = Nh, idx = 0; i < dslots; ++i, jdx += dgap, idx +=
					dgap) {
				p2.rep[idx] = EvaluatorUtils::scaleUpToZZ(pvals[i].real(),
						logp);
				p2.rep[jdx] = EvaluatorUtils::scaleUpToZZ(pvals[i].imag(),
						logp);
			}
		} else {
			for (ki = 0; ki < slots; ki += k) {
				for (pos = ki; pos < ki + k; ++pos) {
					for (i = 0; i < slots - pos; ++i) {

						long rotGroup = 1;
						long fivePows = 1;
						for (long index = 0; index <= i + pos; ++index) {
							rotGroup = fivePows;
							fivePows *= 5;
							fivePows %= M;
						}

						deg = ((M - rotGroup) * i * gap) % M;

						//ksiPows
						complex<double> ksiPows;
						double angle = 2.0 * M_PI * deg / M;
						ksiPows.real(cos(angle));
						ksiPows.imag(sin(angle));

						pvals[i] = ksiPows;
					}
					for (i = slots - pos; i < slots; ++i) {

						long rotGroup = 1;
						long fivePows = 1;
						for (long index = 0; index <= i + pos - slots;
								++index) {
							rotGroup = fivePows;
							fivePows *= 5;
							fivePows %= M;
						}

						deg = ((M - rotGroup) * i * gap) % M;

						//ksiPows
						complex<double> ksiPows;
						double angle = 2.0 * M_PI * deg / M;
						ksiPows.real(cos(angle));
						ksiPows.imag(sin(angle));

						pvals[i] = ksiPows;
					}
					EvaluatorUtils::rightRotateAndEqual(pvals, slots, ki);
					FFT::fftSpecialInv(pvals, slots, M);
					pvec[pos].SetLength(N);
					for (i = 0, jdx = Nh, idx = 0; i < slots;
							++i, jdx += gap, idx += gap) {
						pvec[pos].rep[idx] = EvaluatorUtils::scaleUpToZZ(
								pvals[i].real(), logp);
						pvec[pos].rep[jdx] = EvaluatorUtils::scaleUpToZZ(
								pvals[i].imag(), logp);
					}
				}
			}
		}

		for (ki = 0; ki < slots; ki += k) {
			for (pos = ki; pos < ki + k; ++pos) {

				for (i = 0; i < slots - pos; ++i) {

					long rotGroup = 1;
					long fivePows = 1;
					for (long index = 0; index <= i; ++index) {
						rotGroup = fivePows;
						fivePows *= 5;
						fivePows %= M;
					}

					deg = (rotGroup * (i + pos) * gap) % M;

					//ksiPows
					complex<double> ksiPows;
					double angle = 2.0 * M_PI * deg / M;
					ksiPows.real(cos(angle));
					ksiPows.imag(sin(angle));

					pvals[i] = ksiPows;
				}
				for (i = slots - pos; i < slots; ++i) {

					long rotGroup = 1;
					long fivePows = 1;
					for (long index = 0; index <= i; ++index) {
						rotGroup = fivePows;
						fivePows *= 5;
						fivePows %= M;
					}
					deg = (rotGroup * (i + pos - slots) * gap) % M;

					//ksiPows
					complex<double> ksiPows;
					double angle = 2.0 * M_PI * deg / M;
					ksiPows.real(cos(angle));
					ksiPows.imag(sin(angle));

					pvals[i] = ksiPows;
				}
				EvaluatorUtils::rightRotateAndEqual(pvals, slots, ki);
				FFT::fftSpecialInv(pvals, slots, M);
				pvecInv[pos].SetLength(N);
				for (i = 0, jdx = Nh, idx = 0; i < slots;
						++i, jdx += gap, idx += gap) {
					pvecInv[pos].rep[idx] = EvaluatorUtils::scaleUpToZZ(
							pvals[i].real(), logp);
					pvecInv[pos].rep[jdx] = EvaluatorUtils::scaleUpToZZ(
							pvals[i].imag(), logp);
				}
			}
		}

		delete[] pvals;
		bootContextMap.insert(
				pair<long, BootContext>(logSlots,
						BootContext(pvec, pvecInv, p1, p2, logp)));
	}
	//Context::addBootContext(long logSlots, long logp)
	//addConjKey(secretKey);
	//addLeftRotKeys(secretKey);
	for (long index = 0; index < logNh; ++index) {
		long idxx = 1 << index;
		if (rotationManager.leftRotKeyMap.find(idxx)
				== rotationManager.leftRotKeyMap.end()) {
			rotationManager.addLeftRotKey(secretKey, idxx);
		}
	}
	//addLeftRotKeys(secretKey);

	long logk = logSlots / 2;
	long k = 1 << logk;
	long m = 1 << (logSlots - logk);

	for (long i = 1; i < k; ++i) {
		if (rotationManager.leftRotKeyMap.find(i)
				== rotationManager.leftRotKeyMap.end()) {
			rotationManager.addLeftRotKey(secretKey, i);
		}
	}

	for (long i = 1; i < m; ++i) {
		long idx = i * k;
		if (rotationManager.leftRotKeyMap.find(idx)
				== rotationManager.leftRotKeyMap.end()) {
			rotationManager.addLeftRotKey(secretKey, idx);
		}
	}
}

void BootstrappingManager::divByPo2AndEqual(Ciphertext &cipher, long degree) {
	Ring::rightShiftAndEqual(cipher.ax, degree, N);
	Ring::rightShiftAndEqual(cipher.bx, degree, N);

	cipher.logq -= degree;
}

void BootstrappingManager::idivAndEqual(Ciphertext &cipher) {
	Ring::multByMonomialAndEqual(cipher.ax, 3 * Nh, N);
	Ring::multByMonomialAndEqual(cipher.bx, 3 * Nh, N);
}

//----------------------------------------------------------------------------------
//   ADDITIONAL METHODS FOR BOOTSTRAPPING
//----------------------------------------------------------------------------------

void BootstrappingManager::normalizeAndEqual(Ciphertext &cipher) {
	ZZ q = ZZ(1) << cipher.logq;

	for (int i = 0; i < N; ++i) {
		if (NumBits(cipher.ax.rep[i]) == cipher.logq)
			cipher.ax.rep[i] -= q;
		if (NumBits(cipher.bx.rep[i]) == cipher.logq)
			cipher.bx.rep[i] -= q;
	}
}

void BootstrappingManager::coeffToSlotAndEqual(Ciphertext &cipher) {
	long slots = cipher.slots;
	long logSlots = log2(slots);
	long logk = logSlots / 2;
	long k = 1 << logk;

	long ki, j;
	Ciphertext *rotvec = new Ciphertext[k];
	rotvec[0] = cipher;

	NTL_EXEC_RANGE(k - 1, first, last);
	for (j = first; j < last; ++j) {
		rotvec[j + 1] = rotationManager.leftRotateFast(rotvec[0], j + 1);
	}
	NTL_EXEC_RANGE_END
	;

	BootContext bootContext = bootContextMap.at(logSlots);

	Ciphertext *tmpvec = new Ciphertext[k];

	NTL_EXEC_RANGE(k, first, last);
	for (j = first; j < last; ++j) {
		tmpvec[j] = multiplicationManager.multByPoly(rotvec[j], bootContext.pvec[j], bootContext.logp);
	}
	NTL_EXEC_RANGE_END
	;

	for (j = 1; j < k; ++j) {
		additionManager.addAndEqual(tmpvec[0], tmpvec[j]);
	}
	cipher = tmpvec[0];

	for (ki = k; ki < slots; ki += k) {
		NTL_EXEC_RANGE(k, first, last);
		for (j = first; j < last; ++j) {
			tmpvec[j] = multiplicationManager.multByPoly(rotvec[j], bootContext.pvec[j + ki], bootContext.logp);
		}
		NTL_EXEC_RANGE_END
		;
		for (j = 1; j < k; ++j) {
			additionManager.addAndEqual(tmpvec[0], tmpvec[j]);
		}
		rotationManager.leftRotateAndEqualFast(tmpvec[0], ki);
		additionManager.addAndEqual(cipher, tmpvec[0]);
	}
	cipher.reScaleByAndEqual(bootContext.logp);
	delete[] rotvec;
	delete[] tmpvec;
}

void BootstrappingManager::slotToCoeffAndEqual(Ciphertext &cipher) {
	long slots = cipher.slots;
	long logSlots = log2(slots);
	long logk = logSlots / 2;
	long k = 1 << logk;

	long ki, j;
	Ciphertext *rotvec = new Ciphertext[k];
	rotvec[0] = cipher;

	NTL_EXEC_RANGE(k-1, first, last);
	for (j = first; j < last; ++j) {
		rotvec[j + 1] = rotationManager.leftRotateFast(rotvec[0], j + 1);
	}
	NTL_EXEC_RANGE_END
	;

	BootContext bootContext = bootContextMap.at(logSlots);

	Ciphertext *tmpvec = new Ciphertext[k];

	NTL_EXEC_RANGE(k, first, last);
	for (j = first; j < last; ++j) {
		tmpvec[j] = multiplicationManager.multByPoly(rotvec[j], bootContext.pvecInv[j], bootContext.logp);
	}
	NTL_EXEC_RANGE_END
	;

	for (j = 1; j < k; ++j) {
		additionManager.addAndEqual(tmpvec[0], tmpvec[j]);
	}
	cipher = tmpvec[0];

	for (ki = k; ki < slots; ki += k) {
		NTL_EXEC_RANGE(k, first, last);
		for (j = first; j < last; ++j) {
			tmpvec[j] = multiplicationManager.multByPoly(rotvec[j], bootContext.pvecInv[j + ki], bootContext.logp);
		}
		NTL_EXEC_RANGE_END
		;

		for (j = 1; j < k; ++j) {
			additionManager.addAndEqual(tmpvec[0], tmpvec[j]);
		}

		rotationManager.leftRotateAndEqualFast(tmpvec[0], ki);
		additionManager.addAndEqual(cipher, tmpvec[0]);
	}
	cipher.reScaleByAndEqual(bootContext.logp);
	delete[] rotvec;
	delete[] tmpvec;
}

void BootstrappingManager::exp2piAndEqual(Ciphertext &cipher, long logp) {
	Ciphertext cipher2 = multiplicationManager.mult(cipher, cipher);
	cipher2.reScaleByAndEqual(logp); // cipher2.logq : logq - logp

	Ciphertext cipher4 = multiplicationManager.mult(cipher2, cipher2);
	cipher4.reScaleByAndEqual(logp); // cipher4.logq : logq -2logp

	RR c = 1 / (2 * Pi);
	Ciphertext cipher01 = additionManager.addConst(cipher, c, logp); // cipher01.logq : logq

	c = 2 * Pi;
	multiplicationManager.multByConstAndEqual(cipher01, c, logp);
	cipher01.reScaleByAndEqual(logp); // cipher01.logq : logq - logp

	c = 3 / (2 * Pi);
	Ciphertext cipher23 = additionManager.addConst(cipher, c, logp); // cipher23.logq : logq

	c = 4 * Pi * Pi * Pi / 3;
	multiplicationManager.multByConstAndEqual(cipher23, c, logp);
	cipher23.reScaleByAndEqual(logp); // cipher23.logq : logq - logp

	multiplicationManager.multAndEqual(cipher23, cipher2);
	cipher23.reScaleByAndEqual(logp); // cipher23.logq : logq - 2logp

	additionManager.addAndEqual(cipher23, cipher01); // cipher23.logq : logq - 2logp

	c = 5 / (2 * Pi);
	Ciphertext cipher45 = additionManager.addConst(cipher, c, logp); // cipher45.logq : logq

	c = 4 * Pi * Pi * Pi * Pi * Pi / 15;
	multiplicationManager.multByConstAndEqual(cipher45, c, logp);
	cipher45.reScaleByAndEqual(logp); // cipher45.logq : logq - logp

	c = 7 / (2 * Pi);
	additionManager.addConstAndEqual(cipher, c, logp); // cipher.logq : logq

	c = 8 * Pi * Pi * Pi * Pi * Pi * Pi * Pi / 315;
	multiplicationManager.multByConstAndEqual(cipher, c, logp);
	cipher.reScaleByAndEqual(logp); // cipher.logq : logq - logp

	multiplicationManager.multAndEqual(cipher, cipher2);
	cipher.reScaleByAndEqual(logp); // cipher.logq : logq - 2logp

	cipher45.modDownByAndEqual(logp); // cipher45.logq : logq - 2logp
	additionManager.addAndEqual(cipher, cipher45); // cipher.logq : logq - 2logp

	multiplicationManager.multAndEqual(cipher, cipher4);
	cipher.reScaleByAndEqual(logp); // cipher.logq : logq - 3logp

	cipher23.modDownByAndEqual(logp);
	additionManager.addAndEqual(cipher, cipher23); // cipher.logq : logq - 3logp
}

void BootstrappingManager::evalExpAndEqual(Ciphertext &cipher, long logT,
		long logI) {
	long slots = cipher.slots;
	long logSlots = log2(slots);
	BootContext bootContext = bootContextMap.at(logSlots);
	if (logSlots == 0 && !cipher.isComplex) {
		multiplicationManager.imultAndEqual(cipher);
		divByPo2AndEqual(cipher, logT); // bitDown: logT
		exp2piAndEqual(cipher, bootContext.logp); // bitDown: logT + 3(logq + logI)
		for (long i = 0; i < logI + logT; ++i) {
			multiplicationManager.multAndEqual(cipher, cipher);
			cipher.reScaleByAndEqual(bootContext.logp);
		}
		Ciphertext tmp = rotationManager.conjugate(cipher);
		substractionManager.subAndEqual(cipher, tmp);
		idivAndEqual(cipher);
		RR c = 0.25 / Pi;
		multiplicationManager.multByConstAndEqual(cipher, c, bootContext.logp);
		// bitDown: logT + 3(logq + logI) + (logI + logT)(logq + logI)
	} else if (logSlots < logNh) {
		Ciphertext tmp = rotationManager.conjugate(cipher);
		substractionManager.subAndEqual(cipher, tmp);
		divByPo2AndEqual(cipher, logT + 1); // bitDown: logT + 1
		exp2piAndEqual(cipher, bootContext.logp); // bitDown: logT + 1 + 3(logq + logI)
		for (long i = 0; i < logI + logT; ++i) {
			multiplicationManager.multAndEqual(cipher, cipher);
			cipher.reScaleByAndEqual(bootContext.logp);
		}
		tmp = rotationManager.conjugate(cipher);
		substractionManager.subAndEqual(cipher, tmp);

		tmp = multiplicationManager.multByPoly(cipher, bootContext.p1,
				bootContext.logp);
		Ciphertext tmprot = rotationManager.leftRotateFast(tmp, slots);
		additionManager.addAndEqual(tmp, tmprot);
		multiplicationManager.multByPolyAndEqual(cipher, bootContext.p2,
				bootContext.logp);
		tmprot = rotationManager.leftRotateFast(cipher, slots);
		additionManager.addAndEqual(cipher, tmprot);
		additionManager.addAndEqual(cipher, tmp);
		// bitDown: logT + 1 + 3(logq + logI) + (logI + logT)(logq + logI)
	} else {
		Ciphertext tmp = rotationManager.conjugate(cipher);
		Ciphertext c2 = substractionManager.sub(cipher, tmp);
		additionManager.addAndEqual(cipher, tmp);
		multiplicationManager.imultAndEqual(cipher);
		divByPo2AndEqual(cipher, logT + 1); // cipher bitDown: logT + 1
		c2.reScaleByAndEqual(logT + 1); // c2 bitDown: logT + 1
		exp2piAndEqual(cipher, bootContext.logp); // cipher bitDown: logT + 1 + 3(logq + logI)
		exp2piAndEqual(c2, bootContext.logp); // c2 bitDown: logT + 1 + 3(logq + logI)
		for (long i = 0; i < logI + logT; ++i) {
			multiplicationManager.multAndEqual(c2, c2);
			multiplicationManager.multAndEqual(cipher, cipher);
			c2.reScaleByAndEqual(bootContext.logp);
			cipher.reScaleByAndEqual(bootContext.logp);
		}
		tmp = rotationManager.conjugate(c2);
		substractionManager.subAndEqual(c2, tmp);
		tmp = rotationManager.conjugate(cipher);
		substractionManager.subAndEqual(cipher, tmp);
		multiplicationManager.imultAndEqual(cipher);
		substractionManager.subAndEqual2(c2, cipher);
		RR c = 0.25 / Pi;
		multiplicationManager.multByConstAndEqual(cipher, c, bootContext.logp);
		// bitDown: logT + 1 + 3(logq + logI) + (logI + logT)(logq + logI)
	}
	cipher.reScaleByAndEqual(bootContext.logp + logI);
	// if (logSlots == 0 && !cipher.isComplex) bitDown: logT + 3(logq + logI) + (logI + logT)(logq + logI) + logq + 2logI
	// else bitDown: logT + 1 + 3(logq + logI) + (logI + logT)(logq + logI) + logq + 2logI
}

void BootstrappingManager::bootstrapAndEqual(Ciphertext &cipher, long logq,
		long logQ, long logT, long logI) {
TimeUtils timeutils;
	long logSlots = log2(cipher.slots);
	long logp = cipher.logp;
	cipher.modDownToAndEqual(logq);
	normalizeAndEqual(cipher);
	cipher.logq = logQ;
	cipher.logp = logq + 4;
	for (long i = logSlots; i < logNh; ++i) {
		Ciphertext rot = rotationManager.leftRotateFast(cipher, (1 << i));
		additionManager.addAndEqual(cipher, rot);
	}

	if (logSlots == 0 && !cipher.isComplex) {
		Ciphertext cconj = rotationManager.conjugate(cipher);
		additionManager.addAndEqual(cipher, cconj);
		timeutils.start("divByPo2AndEqual");divByPo2AndEqual(cipher, logN);timeutils.stop("divByPo2AndEqual"); // bitDown: context.logN - logSlots
		timeutils.start("evalExpAndEqual");evalExpAndEqual(cipher, logT, logI);timeutils.stop("evalExpAndEqual"); // bitDown: context.logN - logSlots + (logq + logI + 4) * logq + (logq + logI + 5) * logI + logT
	} else {
		timeutils.start("divByPo2AndEqual");divByPo2AndEqual(cipher, logNh); timeutils.stop("divByPo2AndEqual");// bitDown: context.logNh - logSlots
		timeutils.start("coeffToSlotAndEqual");coeffToSlotAndEqual(cipher);timeutils.stop("coeffToSlotAndEqual");
		timeutils.start("evalExpAndEqual");evalExpAndEqual(cipher, logT, logI);timeutils.stop("evalExpAndEqual"); // bitDown: context.logNh + (logI + logT + 5) * logq + (logI + logT + 6) * logI + logT + 1
		timeutils.start("slotToCoeffAndEqual");slotToCoeffAndEqual(cipher);timeutils.stop("slotToCoeffAndEqual");
	}
	cipher.logp = logp;
}

