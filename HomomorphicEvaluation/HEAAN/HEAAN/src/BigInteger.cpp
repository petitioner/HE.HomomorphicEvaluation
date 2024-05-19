#include "BigInteger.h"

BigInteger::BigInteger() :
		isNegative(false) {
}

BigInteger::BigInteger(const std::string &numStr) :
		isNegative(false) {
	for (int i = numStr.length() - 1; i >= 0; --i) {
		if (isdigit(numStr[i])) {
			digits.push_back(numStr[i] - '0');
		}
	}
	removeLeadingZeros();
}

BigInteger::BigInteger(const BigInteger &other) {
	copy(other);
}

BigInteger& BigInteger::operator=(const BigInteger &other) {
	if (this != &other) {
		copy(other);
	}
	return *this;
}

BigInteger::~BigInteger() {
}

void BigInteger::copy(const BigInteger &other) {
	digits = other.digits;
	isNegative = other.isNegative;
}

void BigInteger::removeLeadingZeros() {
	while (!digits.empty() && digits.back() == 0) {
		digits.pop_back();
	}
}

BigInteger BigInteger::operator+(const BigInteger &other) const {
	BigInteger result;
	performAddition(other, result);
	return result;
}

bool BigInteger::operator==(const BigInteger &other) const {
	return digits == other.digits && isNegative == other.isNegative;
}

bool BigInteger::operator!=(const BigInteger &other) const {
	return !(*this == other);
}

bool BigInteger::operator<(const BigInteger &other) const {
	if (isNegative != other.isNegative) {
		return isNegative;
	}

	if (digits.size() != other.digits.size()) {
		return (digits.size() < other.digits.size()) ^ isNegative;
	}

	for (int i = digits.size() - 1; i >= 0; --i) {
		if (digits[i] != other.digits[i]) {
			return (digits[i] < other.digits[i]) ^ isNegative;
		}
	}

	return false;
}

bool BigInteger::operator>(const BigInteger &other) const {
	return !(*this <= other);
}

bool BigInteger::operator<=(const BigInteger &other) const {
	return *this < other || *this == other;
}

bool BigInteger::operator>=(const BigInteger &other) const {
	return !(*this < other);
}

std::ostream& operator<<(std::ostream &os, const BigInteger &bigInt) {
	if (bigInt.isNegative) {
		os << '-';
	}

	for (int i = bigInt.digits.size() - 1; i >= 0; --i) {
		os << bigInt.digits[i];
	}

	return os;
}

// 將大整數轉換為字符串的實現
std::string BigInteger::toString() const {
	std::ostringstream oss;
	oss << *this;
	return oss.str();
}

void BigInteger::performAddition(const BigInteger &other,
		BigInteger &result) const {
	const BigInteger *larger = this;
	const BigInteger *smaller = &other;

	if (digits.size() < other.digits.size()) {
		std::swap(larger, smaller);
	}

	result = *larger;
	int carry = 0;

	for (int i = 0; i < smaller->digits.size() || carry; ++i) {
		if (i == result.digits.size()) {
			result.digits.push_back(0);
		}

		result.digits[i] += carry
				+ (i < smaller->digits.size() ? smaller->digits[i] : 0);
		carry = result.digits[i] >= 10;

	}

}

