#pragma once

#include <NTL/ZZX.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>
#include <complex>

#include "Common.h"
using namespace NTL;
using namespace std;

class FFT {
public:
	static void fftSpecial(complex<double> *vals, const long size,
			const long M);
	static void fftSpecialInv(complex<double> *vals, const long size,
			const long M);
};


