#include "Ciphertext.h"
#include "Ring.h"

Ciphertext Ciphertext::negate() {
	return Ciphertext(-this->ax, -this->bx, this->logp, this->logq, this->slots,
			this->isComplex, this->N);
}

void Ciphertext::negateAndEqual() {
	this->ax = -this->ax;
	this->bx = -this->bx;
}


//----------------------------------------------------------------------------------
//   RESCALING & MODULUS DOWN
//----------------------------------------------------------------------------------

Ciphertext Ciphertext::reScaleBy(long bitsDown) {
	ZZX ax, bx;

	Ring::rightShift(ax, this->ax, bitsDown, N);
	Ring::rightShift(bx, this->bx, bitsDown, N);


	return Ciphertext(ax, bx, this->logp - bitsDown, this->logq - bitsDown, this->slots, this->isComplex, this->N);

}

Ciphertext Ciphertext::reScaleTo(long newlogq) {
	ZZX ax, bx;
	long bitsDown = this->logq - newlogq;

	Ring::rightShift(ax, this->ax, bitsDown, N);
	Ring::rightShift(bx, this->bx, bitsDown, N);


	return Ciphertext(ax, bx, this->logp - bitsDown, newlogq, this->slots, this->isComplex, this->N);
}

void Ciphertext::reScaleByAndEqual(long bitsDown) {
	Ring::rightShiftAndEqual(this->ax, bitsDown, N);
	Ring::rightShiftAndEqual(this->bx, bitsDown, N);

	this->logq -= bitsDown;
	this->logp -= bitsDown;


}

void Ciphertext::reScaleToAndEqual(long logq) {
	long bitsDown = this->logq - logq;
	this->logq = logq;
	this->logp -= bitsDown;

	Ring::rightShiftAndEqual(this->ax, bitsDown, N);
	Ring::rightShiftAndEqual(this->bx, bitsDown, N);
}

Ciphertext Ciphertext::modDownBy(long bitsDown) {
	ZZX ax, bx;
	long newlogq = this->logq - bitsDown;
	ZZ q = ZZ(1) << newlogq;

	Ring::mod(ax, this->ax, q, N);
	Ring::mod(bx, this->bx, q, N);

	return Ciphertext(ax, bx, this->logp, newlogq, this->slots, this->isComplex, this->N);
}

void Ciphertext::modDownByAndEqual(long bitsDown) {
	this->logq -= bitsDown;
	ZZ q = ZZ(1) << this->logq;

	Ring::modAndEqual(this->ax, q, N);
	Ring::modAndEqual(this->bx, q, N);
}

Ciphertext Ciphertext::modDownTo(long logq) {
	ZZX bx, ax;
	ZZ q = ZZ(1) << logq;

	Ring::mod(ax, this->ax, q, N);
	Ring::mod(bx, this->bx, q, N);
	return Ciphertext(ax, bx, this->logp, logq, this->slots, this->isComplex, this->N);
}

void Ciphertext::modDownToAndEqual(long logq) {
	this->logq = logq;
	ZZ q = ZZ(1) << logq;

	Ring::modAndEqual(this->ax, q, N);
	Ring::modAndEqual(this->bx, q, N);
}




void Ciphertext::copy(Ciphertext& o) {
	logp = o.logp;
	logq = o.logq;
	this->slots = o.slots;
	
	for (long i = 0; i < N; ++i) {
		ax[i] = o.ax[i];
		bx[i] = o.bx[i];
	}
}

void Ciphertext::free() {
	for (long i = 0; i < N; ++i) {
		clear(ax[i]);
		clear(bx[i]);
	}
}

Ciphertext::~Ciphertext() {
//	delete &ax;
//	delete &bx;
}

void Ciphertext::kill() {
	if(ax != NULL) {
		for(long i = 0; i < N; i++) {
			ax[i].kill();
		}
	}
	if(bx != NULL) {
		for(long i = 0; i < N; i++) {
			bx[i].kill();
		}
	}
}
