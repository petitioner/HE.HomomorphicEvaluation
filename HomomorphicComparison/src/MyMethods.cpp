#include "MyMethods.h"
#include "SerializationUtils.h"  // error: ‘SerializationUtils’ has not been declared
#include "StringUtils.h"
#include "Ciphertext.h"
#include "NTL/ZZX.h"
#include "Scheme.h"
#include "TestScheme.h"
#include "SecretKey.h"
#include "TimeUtils.h"
#include <cmath>

#include "MyTools.h"
#include <EvaluatorUtils.h>
#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>

#include <iomanip>

#include <algorithm>    // std::shuffle
#include <vector>        // std::vector
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

#include <unistd.h>






using namespace std;
using namespace NTL;

void MyMethods::testSine(){
	cout << "!!! START TEST BASIC !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, logN, logQ);
	//-----------------------------------------
	SetNumThreads(1);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------

	logSlots = 6;

	long slots = (1 << logSlots);
	auto mvec1 = EvaluatorUtils::randomRealArray(slots);

	cout << endl << endl << "Test cosx form exp(ix)" << endl;
	for (long i = 0; i < slots; ++i) {
		std::complex<double> z1(mvec1[i], 0.0);
		std::complex<double> z2(0, 1);
		auto ix = z1 * z2;
		std::complex<double> e1(1, 0);
		std::complex<double> e2(.5, 0);
		std::complex<double> e3(1.0 / 6, 0);
		auto expix = e1 + ix + e2 * ix * ix + e3 * ix * ix * ix;

		std::complex<double> z11(-mvec1[i], 0.0);
		std::complex<double> z22(0, 1);
		auto nix = z11 * z22;
		auto expnix = e1 + nix + e2 * nix * nix + e3 * nix * nix * nix;

		cout << e2 * (expix + expnix) << " = = " << cos(mvec1[i]) << endl;

	}

	cout << endl << endl << "Test sinx form exp(ix)" << endl;
	for (long i = 0; i < slots; ++i) {
		std::complex<double> z1(mvec1[i], 0.0);
		std::complex<double> z2(0, 1);
		auto ix = z1 * z2;
		std::complex<double> e1(1, 0);
		std::complex<double> e2(.5, 0);
		std::complex<double> e3(1.0 / 6, 0);
		std::complex<double> e4(1.0 / 24, 0);
		auto expix = e1 + ix + e2 * ix * ix + e3 * ix * ix * ix
				+ e4 * ix * ix * ix * ix;

		std::complex<double> z11(mvec1[i], 0.0);
		std::complex<double> z22(0, 1);
		auto nix = -z11 * z22;
		auto expnix = e1 + nix + e2 * nix * nix + e3 * nix * nix * nix
				+ e4 * nix * nix * nix * nix;

		cout << -e2 * z2 * (expix - expnix) << " = = " << sin(mvec1[i]) << endl;

	}

	cout << "SDF" << endl;

	/*
	 exp(ix) = cosx + isinx
	 exp(-ix) = cosx - isinx

	 exp(ix) - exp(-ix) = 2isinx

	 i[exp(ix) - exp(-ix)] = -2sinx

	 -0.5i[exp(ix) - exp(-ix)] = sinx


	 exp(ix) + exp(-ix) = 2cosx
	 .5[exp(ix) + exp(-ix)] = cosx
	 */

	for (long i = 0; i < slots; ++i)
		mvec1[i] = 22 * mvec1[i];
//for (long i=0; i<slots; ++i) mvec1[i] = 52*mvec1[i];
//for (long i=0; i<slots; ++i) mvec1[i] = 12*mvec1[i];
	timeutils.start("Encrypt two batch");
	Ciphertext cipher1 = scheme.encrypt(mvec1, slots, logp, logQ);
	timeutils.stop("Encrypt two batch");


auto ctres = MyTools::Sine(scheme, cipher1, 6); 


	timeutils.start("Decrypt batch");
	auto dvec1 = scheme.decrypt(secretKey, ctres);
	timeutils.stop("Decrypt batch");

	auto mvec2 = EvaluatorUtils::randomRealArray(slots);
	cout << "mvec1[i]" << "\t\t" << "sin(mvec1[i])" << "\t\t" << "dvec1[i]" << endl;
	for (long i = 0; i < slots; ++i) {
		cout << mvec1[i] << "\t\t" << sin(mvec1[i]) << "\t\t" << dvec1[i] << endl;
		mvec2[i] = sin(mvec1[i]);
	}

	StringUtils::showcompare(mvec2, dvec1, slots, "sin");


	cout  << endl << endl  << endl << endl << "Sine = "  << endl << endl << endl << endl;

	cout << "logQ = " << logQ << endl << endl;
	cout << "ctres.logq = " << ctres.logq << endl << endl;

}

