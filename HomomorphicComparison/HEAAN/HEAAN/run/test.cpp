/*
* Copyright (c) by CryptoLab inc.
* This program is licensed under a
* Creative Commons Attribution-NonCommercial 3.0 Unported License.
* You should have received a copy of the license along with this
* work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
*/

#include "../src/Scheme.h"
#include "../src/TestScheme.h"
/**
  * This file is for test HEAAN library
  * You can find more in src/TestScheme.h
  * "./TestHEAAN Encrypt" will run Encrypt Test
  * There are Encrypt, EncryptSingle, Add, Mult, iMult, RotateFast, Conjugate Tests
  */

/* To Consist With the IDASH2017, There Should Be Only One main() Function. */
/*              The error is : multiple definition of `main';               */
int main(int argc, char **argv) {




	long logq = 1200; ///< Ciphertext Modulus
	long logp = 30; ///< Real message will be quantized by multiplying 2^40
	long logn = 5; ///< log2(The number of slots)

long logN = 13;
long logQ = 1200;
logp = 30;
long logSlots = 6;
TestScheme::testBasic(logN, logQ, logp, logSlots);

	return 0;
}
