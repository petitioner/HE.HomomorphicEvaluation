#pragma once

#include <NTL/ZZ.h>
#include <NTL/ZZX.h>

using namespace std;
using namespace NTL;

/**
 * Ciphertext is an RLWE instance (ax, bx = mx + ex - ax * sx) in ring Z_q[X] / (X^N + 1);
 */
class Ciphertext {
public:

	ZZX ax; ///< Ciphertext is an RLWE instance (ax, bx = mx + ex - ax * sx) in ring Z_q[X] / (X^N + 1);
	ZZX bx; ///< Ciphertext is an RLWE instance (ax, bx = mx + ex - ax * sx) in ring Z_q[X] / (X^N + 1);

	long logp; ///< number of quantized bits
	long logq; ///< number of bits in modulus
	long slots; ///< number of slots in Ciphertext

	long N;  //  N = 1 << logN

	bool isComplex; ///< option of Ciphertext with single real slot

	//-----------------------------------------

	/**
	 * Ciphertext = (ax, bx = mx + ex - ax * sx) for secret key sx and error ex
	 * @param[in] ax: ZZX polynomial
	 * @param[in] bx: ZZX polynomial
	 * @param[in] logp: number of quantized bits
	 * @param[in] logq: number of bits in modulus
	 * @param[in] slots: number of slots in a ciphertext
	 * @param[in] isComplex: option of Ciphertext with single real slot
	 */
	//Ciphertext(ZZX ax = ZZX::zero(), ZZX bx = ZZX::zero(), long logp = 0, long logq = 0, long slots = 1, bool isComplex = true) : ax(ax), bx(bx), logp(logp), logq(logq), slots(slots), isComplex(isComplex) {}
	Ciphertext(ZZX ax = ZZX::zero(), ZZX bx = ZZX::zero(), long logp = 0,
			long logq = 0, long slots = 1, bool isComplex = true, long N = 1) :
			ax(ax), bx(bx), logp(logp), logq(logq), slots(slots), isComplex(
					isComplex), N(N) {
	}
	/**
	 * Copy Constructor
	 */
	Ciphertext(const Ciphertext &o) :
			ax(o.ax), bx(o.bx), logp(o.logp), logq(o.logq), slots(o.slots), isComplex(
					o.isComplex), N(o.N) {
	}

	void copy(Ciphertext& o);

	void free();




	Ciphertext negate();

	void negateAndEqual();
	//----------------------------------------------------------------------------------
	//   RESCALING & MODULUS DOWN
	//----------------------------------------------------------------------------------

	Ciphertext reScaleBy(long bitsDown);

	Ciphertext reScaleTo(long newlogq);

	void reScaleByAndEqual(long bitsDown);

	void reScaleToAndEqual(long newlogq);

	Ciphertext modDownBy(long bitsDown);

	void modDownByAndEqual(long bitsDown);

	Ciphertext modDownTo(long newlogq);

	void modDownToAndEqual(long newlogq);



};


