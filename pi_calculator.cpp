#include "long_number.hpp"
#include <iostream>
#include <cstdlib>
#include <string>

static LongNumber factorial(int n) {
    LongNumber result("1");
    for (int i = 2; i <= n; i++) {
        result = result * LongNumber(i);
    }
    return result;
}

LongNumber floorLongNumber(const LongNumber &num) {
    std::string s = num.toDecimalString();
    auto dotPos = s.find('.');
    if (dotPos == std::string::npos) {
        return num; 
    }
    std::string intPart = s.substr(0, dotPos);
    return LongNumber(intPart);
}

std::string toAccurateDecimalString(const LongNumber &num, int precision) {

    LongNumber integerPart = floorLongNumber(num);
    LongNumber fractionPart = num - integerPart;

    std::string result = integerPart.toDecimalString();
    result.push_back('.');

    for (int i = 0; i < precision; i++) {

        fractionPart = fractionPart * LongNumber("10");

        LongNumber digit = floorLongNumber(fractionPart);
        std::string digitStr = digit.toDecimalString();

        if (digitStr.size() > 1) {
            digitStr = digitStr.substr(0, 1);
        }
        result.append(digitStr);

        fractionPart = fractionPart - digit;
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <decimal_digits>\n";
        return 1;
    }

    int digits = std::atoi(argv[1]);
    if (digits <= 0) {
        std::cerr << "Invalid precision\n";
        return 1;
    }

    std::size_t bits = static_cast<std::size_t>(digits) * 4;
    if (bits < 64) bits = 64;
    LongNumber::setPrecision(bits);
    std::cout << "Using precision: " << LongNumber::getPrecision() << std::endl;

    LongNumber piVal = chudnovskyPi(8);

    std::string piDecimal = toAccurateDecimalString(piVal, digits);
    std::cout << "π ≈ " << piDecimal << std::endl;
    return 0;
}