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
long logN = 13;
long logQ = 1200;
long logp = 30;
long logSlots = 6;
long slots = (1 << logSlots);

//void TestScheme::testBasic(long logN, long logQ, long logp, long logSlots) {
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

	auto ct = scheme.multByConst(cipher1, 1. / 256, logp);
	ct.reScaleByAndEqual(logp);
	scheme.imultAndEqual(ct);
	auto nect = ct.negate();

//y = 0.991576 + 0.99636 .*x + 0.550876 .*x.^2 + 0.178834 .*x.^3 ;
	auto ct1 = ct;
	auto ct2 = scheme.mult(ct1, ct1);
	ct2.reScaleByAndEqual(logp);
	auto ct3 = scheme.mult(ct1, ct2);
	ct3.reScaleByAndEqual(logp);
	ct2 = scheme.multByConst(ct2, 0.5, logp);
	ct2.reScaleByAndEqual(logp);
	ct3 = scheme.multByConst(ct3, 1./6, logp);
	ct3.reScaleByAndEqual(logp);
	ct1.modDownToAndEqual(ct3.logq);
// cout << "ct0.logq = " << ct0.logq  << "ct0.logp = " << ct0.logp << endl;
	cout << "ct1.logq = " << ct1.logq << "\t\tct1.logp = " << ct1.logp << endl;
	cout << "ct2.logq = " << ct2.logq << "\t\tct2.logp = " << ct2.logp << endl;
	cout << "ct3.logq = " << ct3.logq << "\t\tct3.logp = " << ct3.logp << endl;
	scheme.addAndEqual(ct1, ct2);
	scheme.addAndEqual(ct1, ct3);
	auto ctexp1 = scheme.addConst(ct1, 1);
	long logpp = ctexp1.logp;
	for (long i = 0; i < 8; ++i) {
		ctexp1 = scheme.mult(ctexp1, ctexp1);
		ctexp1.reScaleByAndEqual(logpp);
	}

	auto ct11 = nect;
	auto ct22 = scheme.mult(ct11, ct11);
	ct22.reScaleByAndEqual(logp);
	auto ct33 = scheme.mult(ct11, ct22);
	ct33.reScaleByAndEqual(logp);
	ct22 = scheme.multByConst(ct22, 0.5, logp);
	ct22.reScaleByAndEqual(logp);
	ct33 = scheme.multByConst(ct33, 1./6, logp);
	ct33.reScaleByAndEqual(logp);
	ct11.modDownToAndEqual(ct33.logq);
// cout << "ct0.logq = " << ct0.logq  << "ct0.logp = " << ct0.logp << endl;
	cout << "ct1.logq = " << ct1.logq << "\t\tct1.logp = " << ct1.logp << endl;
	cout << "ct2.logq = " << ct2.logq << "\t\tct2.logp = " << ct2.logp << endl;
	cout << "ct3.logq = " << ct3.logq << "\t\tct3.logp = " << ct3.logp << endl;
	scheme.addAndEqual(ct11, ct22);
	scheme.addAndEqual(ct11, ct33);
	auto ctexp11 = scheme.addConst(ct11, 1);
	logpp = ctexp11.logp;
	for (long i = 0; i < 8; ++i) {
		ctexp11 = scheme.mult(ctexp11, ctexp11);
		ctexp11.reScaleByAndEqual(logpp);
	}

	auto ctres = scheme.sub(ctexp1, ctexp11);
	ctres = scheme.multByConst(ctres, -0.5, logp);
	ctres.reScaleByAndEqual(logp);

	scheme.imultAndEqual(ctres);

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

