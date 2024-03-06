#include "FFT.h"

//----------------------------------------------------------------------------------
//   FFT & FFT INVERSE
//----------------------------------------------------------------------------------

void FFT::fftSpecialInv(complex<double> *vals, const long size, const long M) {
	for (long len = size; len >= 1; len >>= 1) {
		for (long i = 0; i < size; i += len) {
			long lenh = len >> 1;
			long lenq = len << 2;

			long rotGroup = 1;
			long fivePows = 1; //idx
			for (long j = 0; j < lenh; ++j) {
				rotGroup = fivePows;
				fivePows *= 5;
				fivePows %= M;
				long idx = (lenq - (rotGroup % lenq)) * M / lenq;
				complex<double> u = vals[i + j] + vals[i + j + lenh];
				complex<double> v = vals[i + j] - vals[i + j + lenh];

				//ksiPows
				complex<double> ksiPows;
				double angle = 2.0 * M_PI * idx / M;
				ksiPows.real(cos(angle));
				ksiPows.imag(sin(angle));

				v *= ksiPows;
				vals[i + j] = u;
				vals[i + j + lenh] = v;
			}
		}
	}
	//bitReverse(vals, size);
	for (long i = 1, j = 0; i < size; ++i) {
		long bit = size >> 1;
		for (; j >= bit; bit >>= 1) {
			j -= bit;
		}
		j += bit;
		if (i < j) {
			swap(vals[i], vals[j]);
		}
	}

	//fftSpecialInvLazy(vals, size);
	for (long i = 0; i < size; ++i) {
		vals[i] /= size;
	}
}

void FFT::fftSpecial(complex<double> *vals, const long size, const long M) {
	for (long i = 1, j = 0; i < size; ++i) {
		long bit = size >> 1;
		for (; j >= bit; bit >>= 1) {
			j -= bit;
		}
		j += bit;
		if (i < j) {
			swap(vals[i], vals[j]);
		}
	}
	//bitReverse(vals, size);
	for (long len = 2; len <= size; len <<= 1) {
		for (long i = 0; i < size; i += len) {
			long lenh = len >> 1;
			long lenq = len << 2;

			long rotGroup = 1;
			long fivePows = 1; //idx
			for (long j = 0; j < lenh; ++j) {
				rotGroup = fivePows;
				fivePows *= 5;
				fivePows %= M;
				long idx = ((rotGroup % lenq)) * M / lenq;
				complex<double> u = vals[i + j];
				complex<double> v = vals[i + j + lenh];

				//ksiPows
				complex<double> ksiPows;
				double angle = 2.0 * M_PI * idx / M;
				ksiPows.real(cos(angle));
				ksiPows.imag(sin(angle));

				v *= ksiPows;
				vals[i + j] = u + v;
				vals[i + j + lenh] = u - v;
			}
		}
	}
}

