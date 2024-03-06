#pragma once

class TestScheme {
public:

	//----------------------------------------------------------------------------------
	//   STANDARD TESTS
	//----------------------------------------------------------------------------------

	/**
	 * Testing encoding and decoding timing of the ciphertext
	 * c(m_1, ..., m_slots)
	 * number of modulus bits down: 0
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logSlots: log of number of slots
	 */
	static void testEncodeBatch(long logN, long logQ, long logp, long logSlots);

	/**
	 * Testing encoding and decoding timing of the ciphertext of single value
	 * c(m_1)
	 * number of modulus bits down: 0
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 */
	static void testEncodeSingle(long logN, long logQ, long logp);

	/**
	 * Testing encoding, decoding, add, and mult timing of the ciphertext
	 * c(m_1, ..., m_slots)
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] logSlots: log of number of slots
	 */
	static void testBasic(long logN, long logQ, long logp, long logSlot);

	/**
	 * Testing left rotation timing in the ciphertext
	 * c(m_1, ..., m_slots) -> c(m_(rotslots+1), m_(rotslots+2), ... m_(rotslots-1))
	 * number of modulus bits down: 0
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logp: log of precision
	 * @param[in] rotSlots: number of rotation slots
	 * @param[in] logSlots: log of number of slots
	 * @param[in] isLeft: switching between left and right rotations
	 */
	static void testRotateBatch(long logN, long logQ, long logp, long rotSlots,
			long logSlots, bool isLeft);

	/**
	 * Testing bootstrapping procedure
	 * number of modulus bits up: depends on parameters
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logq: log of initial modulus
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] logSlots: log of number of slots
	 * @param[in] nu: auxiliary parameter, corresonds to message bits (message bits is logq - nu)
	 * @param[in] logT: auxiliary parameter, corresponds to number of iterations in removeIpart (num of iterations is logI + logT)
	 */
	static void testBootstrap(long logN, long logq, long logQ, long logSlots,
			long nu, long logT);

	/**
	 * Testing bootstrapping procedure for single real value
	 * number of modulus bits up: depends on parameters
	 * @param[in] logN: input parameter for Params class
	 * @param[in] logq: log of initial modulus
	 * @param[in] logQ: input parameter for Params class
	 * @param[in] nu: auxiliary parameter, corresonds to message bits (message bits is logq - nu)
	 * @param[in] logT: auxiliary parameter, corresponds to number of iterations in removeIpart (num of iterations is logI + logT)
	 */
	//static void testBootstrapSingleReal(long logN, long logq, long logQ, long nu, long logT);
};


