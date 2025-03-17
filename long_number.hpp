#ifndef LONG_NUMBER_HPP
#define LONG_NUMBER_HPP

#include <string>
#include <stdexcept>
#include <cstddef>
#include <iosfwd>

bool checkAllZero(const std::string& str);
class LongNumberException : public std::runtime_error {
public:
    explicit LongNumberException(const std::string& msg)
        : std::runtime_error(msg) {}
};

enum class RoundingMode { Truncate, Round };


class LongNumber {
public:
  LongNumber abs() const {
        LongNumber result = *this;
        result.negative = false;
        return result;
    }
    std::string toString() const;

    LongNumber();
    LongNumber(const LongNumber& other);
    LongNumber(LongNumber&& other) noexcept;
    LongNumber& operator=(const LongNumber& other);
    LongNumber& operator=(LongNumber&& other) noexcept;
    ~LongNumber();

    LongNumber(int value);
    LongNumber(long long value);
    LongNumber(long double value);
    LongNumber(const std::string& str);

    static void setPrecision(std::size_t bits);
    static std::size_t getPrecision();
    static void setMaxPrecision(std::size_t maxBits);
    static std::size_t getMaxPrecision();
    static void setRoundingMode(RoundingMode mode);
    static RoundingMode getRoundingMode();

    friend LongNumber operator+(const LongNumber& lhs, const LongNumber& rhs);
    friend LongNumber operator-(const LongNumber& lhs, const LongNumber& rhs);
    friend LongNumber operator*(const LongNumber& lhs, const LongNumber& rhs);
    friend LongNumber operator/(const LongNumber& lhs, const LongNumber& rhs);
    friend LongNumber operator%(const LongNumber& lhs, const LongNumber& rhs);

    friend bool operator==(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator!=(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator<(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator>(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator<=(const LongNumber& lhs, const LongNumber& rhs);
    friend bool operator>=(const LongNumber& lhs, const LongNumber& rhs);

    friend LongNumber pow(const LongNumber& base, const LongNumber& exponent);
    friend LongNumber sqrt(const LongNumber& x);
    friend LongNumber ln(const LongNumber& x);
    friend LongNumber exp(const LongNumber& x);
    friend LongNumber sin(const LongNumber& x);
    friend LongNumber cos(const LongNumber& x);
    friend LongNumber tan(const LongNumber& x);

    friend std::ostream& operator<<(std::ostream& os, const LongNumber& num);
    friend std::istream& operator>>(std::istream& is, LongNumber& num);

    std::string toDecimalString() const;
    std::string toBinaryString() const;
    std::string integerDivideBinary(const std::string& numerator, const std::string& denominator);

    static LongNumber fromBinaryString(const std::string& bin);
    static LongNumber fromDecimalString(const std::string& dec);
    static LongNumber fromScientificString(const std::string& sci);

    double toDouble() const;
    int toInt() const;
    long long toLongLong() const;
    std::string getBinInteger() const { return binInteger; }
    std::string getBinFraction() const { return binFraction; }

private:
    static std::size_t s_precision;
    static std::size_t s_maxPrecision;
    static RoundingMode s_roundMode;

    bool negative;
    std::string binInteger;
    std::string binFraction;

    void normalize();
    void clamp();
    static void validatePrecision(std::size_t bits);
    std::string decimalString;
};
LongNumber chudnovskyPi(int terms);

LongNumber operator""_longnum(const char* str, std::size_t);


#endif