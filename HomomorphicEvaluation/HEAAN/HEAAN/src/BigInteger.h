#pragma once

#include <iostream>
#include <sstream>
#include <vector>

class BigInteger {
public:
	// Constructors
	BigInteger();
	explicit BigInteger(const std::string &numStr);

	// Copy constructor
	BigInteger(const BigInteger &other);

	// Assignment operator
	BigInteger& operator=(const BigInteger &other);

	// Destructor
	~BigInteger();

	// Copy function
	void copy(const BigInteger &other);

	// Arithmetic operations
	BigInteger operator+(const BigInteger &other) const;
	BigInteger operator-(const BigInteger &other) const;
	BigInteger operator*(const BigInteger &other) const;

	// Comparison operators
	bool operator==(const BigInteger &other) const;
	bool operator!=(const BigInteger &other) const;
	bool operator<(const BigInteger &other) const;
	bool operator>(const BigInteger &other) const;
	bool operator<=(const BigInteger &other) const;
	bool operator>=(const BigInteger &other) const;

	// Output stream operator
	friend std::ostream& operator<<(std::ostream &os, const BigInteger &bigInt);
	// 將大整數轉換為字符串
	std::string toString() const;

private:
	std::vector<int> digits;
	bool isNegative;

	// Helper functions
	void removeLeadingZeros();
	void performAddition(const BigInteger &other, BigInteger &result) const;
	void performSubtraction(const BigInteger &other, BigInteger &result) const;
	void performMultiplication(const BigInteger &other,
			BigInteger &result) const;

};


