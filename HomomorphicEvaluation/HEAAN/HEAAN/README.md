```cpp

void Ciphertext::copy(Ciphertext& o) {
	this->logp = o.logp;
	this->logq = o.logq;
	this->slots = o.slots;
	this->isComplex = o.isComplex;
	this->N = o.N;

	for (long i = 0; i < o.N; ++i) {
		SetCoeff(this->ax, i, coeff(o.ax, i) );
		SetCoeff(this->bx, i, coeff(o.bx, i) );
	}
}

void Ciphertext::free() {
		clear(ax);
		clear(bx);
}
```
