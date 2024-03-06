#include "Ring.h"

//----------------------------------------------------------------------------------
//   MODULUS
//----------------------------------------------------------------------------------

void Ring::mod(ZZX &res, ZZX &p, ZZ &mod, const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		rem(res.rep[i], p.rep[i], mod);
	}
}

void Ring::modAndEqual(ZZX &p, ZZ &mod, const long degree) {
	for (long i = 0; i < degree; ++i) {
		rem(p.rep[i], p.rep[i], mod);
	}
}

//----------------------------------------------------------------------------------
//   ADDITION & SUBSTRACTION
//----------------------------------------------------------------------------------

void Ring::add(ZZX &res, ZZX &p1, ZZX &p2, ZZ &mod, const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		AddMod(res.rep[i], p1.rep[i], p2.rep[i], mod);
	}
}

ZZX Ring::add(ZZX &p1, ZZX &p2, ZZ &mod, const long degree) {
	ZZX res;
	add(res, p1, p2, mod, degree);
	return res;
}

void Ring::addAndEqual(ZZX &p1, ZZX &p2, ZZ &mod, const long degree) {
	for (long i = 0; i < degree; ++i) {
		AddMod(p1.rep[i], p1.rep[i], p2.rep[i], mod);
	}
}

void Ring::sub(ZZX &res, ZZX &p1, ZZX &p2, ZZ &mod, const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		AddMod(res.rep[i], p1.rep[i], -p2.rep[i], mod);
	}
}

ZZX Ring::sub(ZZX &p1, ZZX &p2, ZZ &mod, const long degree) {
	ZZX res;
	sub(res, p1, p2, mod, degree);
	return res;
}

void Ring::subAndEqual(ZZX &p1, ZZX &p2, ZZ &mod, const long degree) {
	for (long i = 0; i < degree; ++i) {
		AddMod(p1.rep[i], p1.rep[i], -p2.rep[i], mod);
	}
}

void Ring::subAndEqual2(ZZX &p1, ZZX &p2, ZZ &mod, const long degree) {
	for (long i = 0; i < degree; ++i) {
		AddMod(p2.rep[i], p1.rep[i], -p2.rep[i], mod);
	}
}

//----------------------------------------------------------------------------------
//   MULTIPLICATION & SQUARING
//----------------------------------------------------------------------------------

void Ring::mult(ZZX &res, ZZX &p1, ZZX &p2, ZZ &mod, const long degree) {
	res.SetLength(degree);
	ZZX pp;
	mul(pp, p1, p2);
	pp.SetLength(2 * degree);
	for (long i = 0; i < degree; ++i) {
		rem(pp.rep[i], pp.rep[i], mod);
		rem(pp.rep[i + degree], pp.rep[i + degree], mod);
		SubMod(res.rep[i], pp.rep[i], pp.rep[i + degree], mod);
	}
}

ZZX Ring::mult(ZZX &p1, ZZX &p2, ZZ &mod, const long degree) {
	ZZX res;
	mult(res, p1, p2, mod, degree);
	return res;
}

void Ring::multAndEqual(ZZX &p1, ZZX &p2, ZZ &mod, const long degree) {
	ZZX pp;
	mul(pp, p1, p2);
	pp.SetLength(2 * degree);

	for (long i = 0; i < degree; ++i) {
		rem(pp.rep[i], pp.rep[i], mod);
		rem(pp.rep[i + degree], pp.rep[i + degree], mod);
		SubMod(p1.rep[i], pp.rep[i], pp.rep[i + degree], mod);
	}
}

void Ring::square(ZZX &res, ZZX &p, ZZ &mod, const long degree) {
	res.SetLength(degree);
	ZZX pp;
	sqr(pp, p);
	pp.SetLength(2 * degree);

	for (long i = 0; i < degree; ++i) {
		rem(pp.rep[i], pp.rep[i], mod);
		rem(pp.rep[i + degree], pp.rep[i + degree], mod);
		SubMod(res.rep[i], pp.rep[i], pp.rep[i + degree], mod);
	}
}

ZZX Ring::square(ZZX &p, ZZ &mod, const long degree) {
	ZZX res;
	square(res, p, mod, degree);
	return res;
}

void Ring::squareAndEqual(ZZX &p, ZZ &mod, const long degree) {
	ZZX pp;
	sqr(pp, p);
	pp.SetLength(2 * degree);

	for (long i = 0; i < degree; ++i) {
		rem(pp.rep[i], pp.rep[i], mod);
		rem(pp.rep[i + degree], pp.rep[i + degree], mod);
		SubMod(p.rep[i], pp.rep[i], pp.rep[i + degree], mod);
	}
}

void Ring::multByMonomial(ZZX &res, ZZX &p, const long monomialDeg,
		const long degree) {
	long shift = monomialDeg % (2 * degree);
	if (shift == 0) {
		res = p;
	} else {
		ZZX tmpx;
		tmpx.SetLength(degree);
		tmpx = (shift < degree) ? p : -p;
		shift %= degree;

		res.SetLength(degree);

		for (long i = 0; i < shift; ++i) {
			res.rep[i] = -tmpx.rep[degree - shift + i];
		}

		for (long i = shift; i < degree; ++i) {
			res.rep[i] = tmpx.rep[i - shift];
		}
	}
}

ZZX Ring::multByMonomial(ZZX &p, const long monomialDeg, const long degree) {
	ZZX res;
	multByMonomial(res, p, monomialDeg, degree);
	return res;
}

void Ring::multByMonomialAndEqual(ZZX &p, const long monomialDeg,
		const long degree) {
	long shift = monomialDeg % (2 * degree);
	if (shift == 0) {
		return;
	}
	ZZX tmpx;
	tmpx.SetLength(degree);
	tmpx = (shift < degree) ? p : -p;
	shift %= degree;
	for (long i = 0; i < shift; ++i) {
		p.rep[i] = -tmpx.rep[degree - shift + i];
	}

	for (long i = shift; i < degree; ++i) {
		p.rep[i] = tmpx.rep[i - shift];
	}
}

void Ring::multByConst(ZZX &res, ZZX &p, const ZZ &cnst, ZZ &mod,
		const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		MulMod(res.rep[i], p.rep[i], cnst, mod);
	}
}

ZZX Ring::multByConst(ZZX &p, const ZZ &cnst, ZZ &mod, const long degree) {
	ZZX res;
	multByConst(res, p, cnst, mod, degree);
	return res;
}

void Ring::multByConstAndEqual(ZZX &p, const ZZ &cnst, ZZ &mod,
		const long degree) {
	for (long i = 0; i < degree; ++i) {
		MulMod(p.rep[i], p.rep[i], cnst, mod);
	}
}

void Ring::leftShift(ZZX &res, ZZX &p, const long bits, ZZ &mod,
		const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		LeftShift(res.rep[i], p.rep[i], bits);
		rem(res.rep[i], res.rep[i], mod);
	}
}

void Ring::leftShiftAndEqual(ZZX &p, const long bits, ZZ &mod,
		const long degree) {
	for (long i = 0; i < degree; ++i) {
		LeftShift(p.rep[i], p.rep[i], bits);
		rem(p.rep[i], p.rep[i], mod);
	}
}

void Ring::doubleAndEqual(ZZX &p, ZZ &mod, const long degree) {
	for (long i = 0; i < degree; ++i) {
		LeftShift(p.rep[i], p.rep[i], 1);
		rem(p.rep[i], p.rep[i], mod);
	}
}

void Ring::rightShift(ZZX &res, ZZX &p, const long bits, const long degree) {
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		RightShift(res.rep[i], p.rep[i], bits);
	}
}

void Ring::rightShiftAndEqual(ZZX &p, const long bits, const long degree) {
	for (long i = 0; i < degree; ++i) {
		RightShift(p.rep[i], p.rep[i], bits);
	}
}

//----------------------------------------------------------------------------------
//   CONJUGATION ROTATION AND OTHER
//----------------------------------------------------------------------------------

void Ring::conjugate(ZZX &res, ZZX &p, const long degree) {
	res.SetLength(degree);
	res.rep[0] = p.rep[0];
	for (long i = 1; i < degree; ++i) {
		res.rep[i] = -p.rep[degree - i];
	}
}

void Ring::conjugateAndEqual(ZZX &p, const long degree) {
	for (long i = 0; i < degree / 2; ++i) {
		ZZ tmp = p.rep[i];
		p.rep[i] = p.rep[degree - i];
		p.rep[degree - i] = tmp;
	}
	p.rep[degree / 2] = -p.rep[degree / 2];
}

/**
 * changing p(X) to p(X^pow) in Z_q[X] / (X^N + 1)
 * @param[out] p(X^pow) in Z_q[X] / (X^N + 1)
 * @param[in] p(X) in Z_q[X] / (X^N + 1)
 * @param[in] power pow
 * @param[in] mod q
 * @param[in] degree N
 */
void Ring::inpower(ZZX &res, ZZX &p, const long pow, ZZ &mod,
		const long degree) {
	res.kill();
	res.SetLength(degree);
	for (long i = 0; i < degree; ++i) {
		long ipow = i * pow;
		long shift = ipow % (2 * degree);
		if (shift < degree) {
			AddMod(res.rep[shift % degree], res.rep[shift % degree], p.rep[i],
					mod);
		} else {
			AddMod(res.rep[shift % degree], res.rep[shift % degree], -p.rep[i],
					mod);
		}
	}
}

ZZX Ring::inpower(ZZX &p, const long pow, ZZ &mod, const long degree) {
	ZZX res;
	inpower(res, p, pow, mod, degree);
	return res;
}

