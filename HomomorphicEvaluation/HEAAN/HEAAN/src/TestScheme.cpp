#include "TestScheme.h"

#include <NTL/BasicThreadPool.h>
#include <NTL/RR.h>
#include <NTL/ZZ.h>

#include "Common.h"
#include "Ciphertext.h"
#include "EvaluatorUtils.h"
#include "NumUtils.h"
#include "Scheme.h"
#include "SecretKey.h"
#include "StringUtils.h"
#include "TimeUtils.h"
#include "SerializationUtils.h"

using namespace std;
using namespace NTL;

//----------------------------------------------------------------------------------
//   STANDARD TESTS
//----------------------------------------------------------------------------------

void TestScheme::testEncodeBatch(long logN, long logQ, long logp,
		long logSlots) {
	cout << "!!! START TEST ENCODE BATCH !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, logN, logQ);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	long slots = (1 << logSlots);
	auto mvec = EvaluatorUtils::randomRealArray(slots);

	timeutils.start("Encrypt batch");
	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);
	timeutils.stop("Encrypt batch");

	timeutils.start("Decrypt batch");
	auto dvec = scheme.decrypt(secretKey, cipher);
	timeutils.stop("Decrypt batch");

	StringUtils::showcompare(mvec, dvec, slots, "val");

	cout << "!!! END TEST ENCODE BATCH !!!" << endl;
}

void TestScheme::testEncodeSingle(long logN, long logQ, long logp) {
	cout << "!!! START TEST ENCODE SINGLE !!!" << endl;
	//-----------------------------------------
	TimeUtils timeutils;
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, logN, logQ);
	//-----------------------------------------
	srand(time(NULL));
	//-----------------------------------------
	double mval = EvaluatorUtils::randomReal();

	timeutils.start("Encrypt Single");
	Ciphertext cipher = scheme.encrypt(mval, logp, logQ);
	timeutils.stop("Encrypt Single");
	cipher.modDownByAndEqual(5);
	timeutils.start("Decrypt Single");
	double dval = scheme.decryptSingle(secretKey, cipher);
	timeutils.stop("Decrypt Single");

	StringUtils::showcompare(mval, dval, "val");

	cout << "!!! END TEST ENCODE SINGLE !!!" << endl;

	long slots = 8;
	auto mvec1 = EvaluatorUtils::randomRealArray(slots);

	timeutils.start("Encrypt two batch");
	Ciphertext cipher1 = scheme.encrypt(mvec1, slots, logp, logQ);
//cipher1 = scheme.add(cipher1, cipher1);
//scheme.addAndEqual(cipher1, cipher1);
//cipher1 = scheme.addConst(cipher1, 1, logp);
//scheme.addConstAndEqual(cipher1, 1, logp);
//auto cipher2 = scheme.multByConstVec(cipher1, mvec1, slots, logp);
//scheme.multAndEqual(cipher1, cipher1);
//cipher1 = scheme.subConst(cipher1, 1, logp);
//scheme.subConstAndEqual(cipher1, 1, logp);
	timeutils.stop("Encrypt two batch");
	scheme.addLeftRotKey(secretKey, 1);
	auto cipher2 = scheme.leftRotateFast(cipher1, 1);
	scheme.leftRotateAndEqualFast(cipher2, 1);
	timeutils.start("Decrypt batch");
	cipher2.modDownByAndEqual(5);
	auto dvec1 = scheme.decrypt(secretKey, cipher2);
	timeutils.stop("Decrypt batch");

	auto mvec2 = EvaluatorUtils::randomComplexArray(slots);
	for (long i = 0; i < slots; ++i)
		mvec2[i].real(mvec1[i]);

	StringUtils::showcompare(mvec1, dvec1, slots, "MULT");

	for (long i = 0; i < slots; ++i)
		cout << mvec1[i] << "\t";
	cout << endl;
	for (long i = 0; i < slots; ++i)
		cout << dvec1[i] << "\t";
	cout << endl;

}

void TestScheme::testBasic(long logN, long logQ, long logp, long logSlots) {
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

//-----------------------------------------

void TestScheme::testRotateBatch(long logN, long logQ, long logp, long rotSlots,
		long logSlots, bool isLeft) {
	cout << "!!! START TEST ROTATE BATCH !!!" << endl;
//-----------------------------------------
	TimeUtils timeutils;
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, logN, logQ);
//scheme.addLeftRotKeys(secretKey);
//scheme.addRightRotKeys(secretKey);

	scheme.addLeftRotKey(secretKey, 13);

//-----------------------------------------
	srand(time(NULL));
//-----------------------------------------
	long slots = (1 << logSlots);
	auto mvec = EvaluatorUtils::randomRealArray(slots);
	Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);

	if (1) {
		timeutils.start("Left rotate batch");
		cipher = scheme.leftRotateFast(cipher, 13);
		//cipher = scheme.leftRotate(cipher, 13);  //多次一舉 沒有必要
		//scheme.leftRotateAndEqual(cipher, 13);   //多次一舉 沒有必要
		timeutils.stop("Left rotate batch");
	} else {
		timeutils.start("Right rotate batch");
		//scheme.rightRotateAndEqual(cipher, rotSlots);
		timeutils.stop("Right rotate batch");
	}

	auto dvec = scheme.decrypt(secretKey, cipher);

	if (isLeft) {
		//EvaluatorUtils::leftRotateAndEqual(mvec, slots, 13);
	} else {
		//EvaluatorUtils::rightRotateAndEqual(mvec, slots, rotSlots);
	}

//StringUtils::showcompare(mvec, dvec, slots, "rot");

	cout << "!!! END TEST ROTATE BATCH !!!" << endl;

	scheme.addConjKey(secretKey);

	for (long i = 0; i < 16; ++i) {
		auto mvec = EvaluatorUtils::randomRealArray(slots);
		Ciphertext cipher = scheme.encrypt(mvec, slots, logp, logQ);
		cipher = scheme.conjugate(cipher);
		auto dvec = scheme.decrypt(secretKey, cipher);
		////StringUtils::showcompare(mvec, dvec, slots, "conjugate");
	}

}

void TestScheme::testBootstrap(long logN, long logp, long logq, long logQ,
		long logSlots, long logT) {
	cout << "!!! START TEST BOOTSTRAP !!!" << endl;
//-----------------------------------------
	TimeUtils timeutils;
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, logN, logQ);
//-----------------------------------------
	timeutils.start("Key generating");
	scheme.addBootKey(secretKey, 0, logq + 4);
	scheme.addBootKey(secretKey, logSlots, logq + 4);
	timeutils.stop("Key generated");
//-----------------------------------------
	SetNumThreads(1);
//-----------------------------------------
	srand(time(NULL));
//-----------------------------------------

	double mval0 = EvaluatorUtils::randomReal();

	timeutils.start("Encrypt Single");
	Ciphertext cipher = scheme.encrypt(mval0, logp, logq);
	timeutils.stop("Encrypt Single");

	cout << "cipher logq before: " << cipher.logq << endl;
	scheme.bootstrapAndEqual(cipher, logq, logQ, logT);
	cout << "cipher logq after: " << cipher.logq << endl;

	double dval0 = scheme.decryptSingle(secretKey, cipher);

	StringUtils::showcompare(mval0, dval0, "bootstrap single real");

	long slots = (1 << logSlots-1);
cout << endl << "logSlots = " << logSlots << endl;
cout << "slots = " << slots << endl;
	auto mvec0 = EvaluatorUtils::randomRealArray(slots);

auto mvec = EvaluatorUtils::randomRealArray(slots*2);
for (long i = 0; i < slots*2; ++i)
mvec[i] = 100*mvec0[i%slots];

slots = (1 << logSlots);
	cipher = scheme.encrypt(mvec, slots, logp, logq);

	cout << "cipher logq before: " << cipher.logq << endl;

	scheme.bootstrapAndEqual(cipher, logq, logQ, logT);

	cout << "cipher logq after: " << cipher.logq << endl;

	auto dvec = scheme.decrypt(secretKey, cipher);

	StringUtils::showcompare(mvec, dvec, slots, "bootstrap real array");
	cout << "!!! END TEST BOOTSRTAP !!!" << endl;
}

