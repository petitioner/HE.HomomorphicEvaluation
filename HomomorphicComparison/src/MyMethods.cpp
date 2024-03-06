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
//>> x = [-28:0.1:28];
//>> y = 0.6316286288.* sin(0.09817477042.*x) + -0.009662432677.* sin(0.1963495408.*x) + 0.1984535063.* sin(0.2945243113.*x) + -0.01709950346.* sin(0.3926990817.*x) + 0.1076807109.* sin(0.4908738521.*x) + -0.02141386721.* sin(0.5890486225.*x) + 0.06844624569.* sin(0.687223393.*x) + -0.02301669581.* sin(0.7853981634.*x) + 0.04764760391.* sin(0.8835729338.*x) + -0.02282633391.* sin(0.9817477042.*x) ;
void MyMethods::Sigmoid28() {

	long logN = 13;
	long logQ = 1200;
	long logp = 30;
	long logSlots = 9;
	long slots = (1 << logSlots);

	TimeUtils timeutils;
	SecretKey secretKey(logN);
	Scheme scheme(secretKey, logN, logQ);

	SetNumThreads(1);

	srand(time(NULL));

	double *Coeff = new double[10]();
	Coeff[0] = 0.6316286288;
	Coeff[1] = -0.009662432677;
	Coeff[2] = 0.1984535063;
	Coeff[3] = -0.01709950346;
	Coeff[4] = 0.1076807109;
	Coeff[5] = -0.02141386721;
	Coeff[6] = 0.06844624569;
	Coeff[7] = -0.02301669581;
	Coeff[8] = 0.04764760391;
	Coeff[9] = -0.02282633391;

	double *Alpha = new double[10]();
	Alpha[0] = 0.09817477042;
	Alpha[1] = 0.1963495408;
	Alpha[2] = 0.2945243113;
	Alpha[3] = 0.3926990817;
	Alpha[4] = 0.4908738521;
	Alpha[5] = 0.5890486225;
	Alpha[6] = 0.687223393;
	Alpha[7] = 0.7853981634;
	Alpha[8] = 0.8835729338;
	Alpha[9] = 0.9817477042;
	
	double* zeros = new double[slots]();
	Ciphertext ctres;
	ctres = scheme.encrypt(zeros, slots, logp, logQ);

	Ciphertext *ctresult = new Ciphertext[10];
	NTL_EXEC_RANGE(10, first, last);
	for (long i = first; i < last; ++i) {
		ctresult[i] = scheme.encrypt(zeros, slots, logp, logQ);
	}
	NTL_EXEC_RANGE_END

	auto mvec1 = EvaluatorUtils::randomRealArray(slots);

	long t = 5;
	for (long i = 0; i < slots; ++i)
		mvec1[i] = -28 + 0.1 * i;

	timeutils.start("Encrypt two batch");
	Ciphertext cipher1 = scheme.encrypt(mvec1, slots, logp, logQ);
	timeutils.stop("Encrypt two batch");

	NTL_EXEC_RANGE(10, first, last);
	for (long i = first; i < last; ++i) {

		ctresult[i] = MyTools::Sine(scheme, cipher1, Alpha[i], t, logp);
		ctresult[i] = scheme.multByConst(ctresult[i], Coeff[i], logp);
		ctresult[i].reScaleByAndEqual(logp);
	}
	NTL_EXEC_RANGE_END

	ctres.reScaleToAndEqual(ctresult[0].logp);
	ctres.modDownToAndEqual(ctresult[0].logq);
	for (long i = 0; i < 10; ++i) {

		scheme.addAndEqual(ctres, ctresult[i]);

	}

	timeutils.start("Decrypt batch");
	auto dvec1 = scheme.decrypt(secretKey, ctres);
	timeutils.stop("Decrypt batch");

	auto mvec2 = EvaluatorUtils::randomRealArray(slots);
	cout << "mvec1[i]" << "\t\t" << "sin(mvec1[i])" << "\t\t" << "dvec1[i]"
			<< endl;
	for (long i = 0; i < slots; ++i) {
		cout << mvec1[i] << "\t\t" << sin(mvec1[i]) << "\t\t" << dvec1[i]
				<< endl;
		mvec2[i] = sin(mvec1[i]);
	}

	StringUtils::showcompare(mvec2, dvec1, slots, "sin");

}

//>> x = [-100: 0.1:100];
//y = 0.6363045789.* sin(0.02454369261.*x) + -0.0006290784636.* sin(0.04908738521.*x) + 0.2112662282.* sin(0.07363107782.*x) + -0.001247785887.* sin(0.09817477042.*x) + 0.1257738188.*sin(0.122718463.*x) + -0.001846260376.* sin(0.1472621556.*x) + 0.08881059942.* sin(0.1718058482.*x) + -0.002415608169.* sin(0.1963495408.*x) + 0.06804859261.* sin(0.2208932335.*x) + -0.002948270535.* sin(0.2454369261.*x) + 0.0546756136.* sin(0.2699806187.*x) + -0.003438271112.* sin(0.2945243113.*x) + 0.04530488188.* sin(0.3190680039.*x) + -0.003881333543.* sin(0.3436116965.*x) + 0.03835692586.* sin(0.3681553891.*x) + -0.004274875864.* sin(0.3926990817.*x) + 0.03299551371.* sin(0.4172427743.*x) + -0.004617901052.* sin(0.4417864669.*x) + 0.02873575699.* sin(0.4663301595.*x) + -0.004910810893.* sin(0.4908738521.*x) + 0.02527630354.* sin(0.5154175447.*x) + -0.005155172745.* sin(0.5399612373.*x) + 0.02241922603.* sin(0.5645049299.*x) + -0.005353466802.* sin(0.5890486225.*x) + 0.02002848958.* sin(0.6135923152.*x) + -0.005508836558.* sin(0.6381360078.*x) + 0.01800697869.* sin(0.6626797004.*x) + -0.00562485902.* sin(0.687223393.*x) + 0.01628310444.* sin(0.7117670856.*x) + -0.005705344942.* sin(0.7363107782.*x) + 0.01480264652.* sin(0.7608544708.*x) + -0.005754173953.* sin(0.7853981634.*x) + 0.01352359244.* sin(0.809941856.*x) + -0.005775165244.* sin(0.8344855486.*x) + 0.01241276188.* sin(0.8590292412.*x) + -0.005771981587.* sin(0.8835729338.*x) + 0.01144353071.* sin(0.9081166264.*x) + -0.005748062736.* sin(0.932660319.*x) + 0.0105942531.* sin(0.9572040116.*x) + -0.005706583479.* sin(0.9817477042.*x) + 0.009847138938.* sin(1.006291397.*x) ;

void MyMethods::Sigmoid100() {

}

