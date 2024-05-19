////////////////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
#include <fcntl.h>
#include <procfs.h>

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#include <stdio.h>

#endif

#else
#error "Cannot define getPeakRSS( ) or getCurrentRSS( ) for an unknown OS."
#endif
////////////////////////////////////////////////////////////////////////////////////

#ifndef IDASH2017_GD_H_
#define IDASH2017_GD_H_

#include <iostream>

#include "Scheme.h"

////////////////////////////////  For Full-Batch NAG  //////////////////////////////
//        Just Use The Polynomials Computed by Kim iDASH2017                      //        
//             Round 1  ~ 05 : |wTx| < 8                                          //
//             Round 05 ~ 10 : |wTx| < 8                                          //
//             Round 10 ~ 30 : |wTx| < 12                                         //
////////////////////////////////////////////////////////////////////////////////////
//static double degree1[2] = {0.5,0.25};
// y = 0.5  +  0.15012 x^1  -  0.0015930078125 x^3 (0.001593008)                polyfit(x,y,3)
//static double degree3[3] = {+0.5, -0.15012, +0.00159300781};  // 1 - poly(-yWTx)
const long double PI= 3.14159265358979323846264338327950288419716939937510L;
const long double E = 2.71828182845904523536028747135266249775724709369995L;


using namespace std;

class MyTools {


public:

	static long suggestLogN(long lambda, long logQ);
    static bool is_number(string& s);
	static double** dataFromFile(string& path, long& factorDim, long& sampleDim, double** &X, double* &Y);
	static double** zDataFromFile(string& path, long& factorDim, long& sampleDim, bool isfirst = true);

	static double** dataFromNNweightsFile(string& path, long& len, long* &dims);

	static double** zInvBFromFile(double **zDataTrain, long& factorDim, long& sampleDim, bool isfirst = true, double epsilon = 1e-8);
	static void printData(double** zData, long factorDim, long sampleDim);

	static void shuffleDataSync(double** X, long factorDim, long sampleDim, double* Y);
	static void shuffleZData(double** zData, long factorDim, long sampleDim);

	static void normalizeZData(double** zData, long factorDim, long sampleDim);
	static void normalizezData2(double** zDataLearn, double** zDataTest, long factorDim, long sampleDimLearn, long sampleDimTest);

	static double trueIP(double* a, double* b, long size); 

	static double calculateAUC(double** zData, double* wData, long factorDim, long sampleDim, double& correctness, double& AUC);
	static double calculateMLE(double** zData, double* wData, long factorDim, long sampleDim, double& correctness, double& auc);
	static double calculateACC(double** zData, double* wData, long factorDim, long sampleDim, double& correctness, double& auc);
	
	static size_t getPeakRSS( );
	static size_t getCurrentRSS( );

static Ciphertext Sine(Scheme& scheme, Ciphertext& ctx, double alpha, long t, long logp); 
static Ciphertext Cosine(Scheme& scheme, Ciphertext& ctx, double alpha, long t, long logp); 

};

#endif /* SGD_SGD_H_ */
