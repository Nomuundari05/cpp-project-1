#include <cstddef>
#include <ios>
#include <cmath>
#include <ostream>
#include <istream>
#include <string>
#include <utility>
#include <boost/range/algorithm/reverse.hpp>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "long_number.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <sstream>
#include <iomanip>
#include<vector>

bool checkAllZero(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](char ch) { return ch == '0'; });
}

static LongNumber factorial(int n) {
    LongNumber result("1");
    for (int i = 2; i <= n; i++) {
        result = result * LongNumber(i);
    }
    return result;
}

LongNumber chudnovskyPi(int terms) { //fastest pi calculation method
    LongNumber L_13591409("13591409");
    LongNumber L_545140134("545140134");
    LongNumber L_640320("640320");
    LongNumber L_12("12");

    LongNumber invPi("0");
    LongNumber c = L_640320 * L_640320 * L_640320;
    c = sqrt(c);
    LongNumber factor = L_12 / c;

    for (int k = 0; k < terms; k++) {
        LongNumber sixkFact = factorial(6 * k);
        LongNumber top1 = L_545140134 * LongNumber(k) + L_13591409;
        LongNumber numerator = sixkFact * top1;

        LongNumber threekFact = factorial(3 * k);
        LongNumber kFact = factorial(k);
        LongNumber kFactCubed = kFact * kFact * kFact;
        LongNumber denomFactorial = threekFact * kFactCubed;

        LongNumber power640320 = pow(L_640320, LongNumber(3 * k));
        LongNumber denominator = denomFactorial * power640320;

        if (k % 2 != 0) {
            numerator = numerator * LongNumber("-1");
        }

        LongNumber term_k = (factor * (numerator / denominator));
        invPi = invPi + term_k;
    }

    return LongNumber("1") / invPi;
}
LongNumber operator""_longnum(const char* str, std::size_t) {

    std::cout << "Input string: " << str << std::endl;

    if (!str || str[0] == '\0') {
        throw LongNumberException("Invalid input string to LongNumber literal");
    }

    return LongNumber(std::string(str));
}

namespace {
constexpr long double EPSILON = 1e-30L; //toleration 
constexpr long long MAX_EXP_CHECK = 100000000LL;
constexpr int MAX_ITERATIONS = 100;
constexpr long long VERY_LARGE_VAL = 999999999999999999LL;
constexpr int DEC_BASE = 10;
constexpr long double FRACTION_BASE = 0.5L;

bool checkAllZero(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](char ch) { return ch == '0'; });
}

auto addBinaryStrings(const std::string& lhsBinary, const std::string& rhsBinary) -> std::string {
    const std::string& xVal = lhsBinary;
    const std::string& yVal = rhsBinary;
    int carry = 0;
    int i = static_cast<int>(xVal.size()) - 1;
    int j = static_cast<int>(yVal.size()) - 1;
    std::string result;
    while (i >= 0 || j >= 0 || carry != 0) {
        int sum = carry;
        if (i >= 0) {
            if (xVal[static_cast<std::size_t>(i)] == '1') { sum++;
}
            i--;
        }
        if (j >= 0) {
            if (yVal[static_cast<std::size_t>(j)] == '1') { sum++;
}
            j--;
        }
        carry = sum / 2;
        result.push_back(((sum & 1) != 0) ? '1' : '0');
    }
    boost::range::reverse(result);
    if (result.empty()) {
        result = "0";
    }
    return result;
}

auto subBinaryStrings(const std::string& lhsBinary, const std::string& rhsBinary, bool& negativeResult) -> std::string {
    if (lhsBinary == rhsBinary) {
        negativeResult = false;
        return "0";
    }
    bool less = false;
    if (lhsBinary.size() < rhsBinary.size()) {
        less = true;
    } else if (lhsBinary.size() == rhsBinary.size() && lhsBinary < rhsBinary) {
        less = true;
    }
    negativeResult = less;
    const std::string& bigVal = (less ? rhsBinary : lhsBinary);
    const std::string& smallVal = (less ? lhsBinary : rhsBinary);
    std::string xVal = bigVal;
    std::string yVal = smallVal;
    int i = static_cast<int>(xVal.size()) - 1;
    int j = static_cast<int>(yVal.size()) - 1;
    int borrow = 0;
    while (j >= 0) {
        int const xBit = (xVal[static_cast<std::size_t>(i)] == '1') ? 1 : 0;
        int const yBit = (yVal[static_cast<std::size_t>(j)] == '1') ? 1 : 0;
        int diff = xBit - yBit - borrow;
        if (diff < 0) {
            diff += 2;
            borrow = 1;
        } else {
            borrow = 0;
        }
        xVal[static_cast<std::size_t>(i)] = ((diff == 1) ? '1' : '0');
        i--;
        j--;
    }
    while (i >= 0 && borrow != 0) {
        if (xVal[static_cast<std::size_t>(i)] == '1') {
            xVal[static_cast<std::size_t>(i)] = '0';
            borrow = 0;
        } else {
            xVal[static_cast<std::size_t>(i)] = '1';
            borrow = 1;
        }
        i--;
    }
    while (!xVal.empty() && xVal.front() == '0') {
        xVal.erase(xVal.begin());
    }
    if (xVal.empty()) {
        xVal = "0";
    }
    return xVal;
}

auto multiplyBinaryStrings(const std::string& lhsBinary, const std::string& rhsBinary) -> std::string {
    if (checkAllZero(lhsBinary) || checkAllZero(rhsBinary)) {
        return "0";
    }
    std::string result(lhsBinary.size() + rhsBinary.size(), '0');
    for (int i = static_cast<int>(lhsBinary.size()) - 1; i >= 0; i--) {
        int carry = 0;
        int const aInt = (lhsBinary[static_cast<std::size_t>(i)] == '1') ? 1 : 0;
        for (int j = static_cast<int>(rhsBinary.size()) - 1; j >= 0; j--) {
            int const bInt = (rhsBinary[static_cast<std::size_t>(j)] == '1') ? 1 : 0;
            int const sum = (result[static_cast<std::size_t>(i + j + 1)] - '0') + (aInt * bInt) + carry;
            carry = sum / 2;
            result[static_cast<std::size_t>(i + j + 1)] = static_cast<char>('0' + (sum % 2));
        }
        int const oldVal = result[static_cast<std::size_t>(i)] - '0';
        int const newVal = (oldVal + carry) % 2;
        result[static_cast<std::size_t>(i)] = static_cast<char>('0' + newVal);
    }
    while (result.size() > 1 && result.front() == '0') {
        result.erase(result.begin());
    }
    return result;
}

std::pair<std::string, std::string> divideBinaryStrings(const std::string& numerator,
                                                        const std::string& denominator,
                                                        std::size_t fracBits)
{
    if (checkAllZero(denominator)) {
        throw LongNumberException("Division by zero");
    }

    std::string intQuotient;
    intQuotient.reserve(numerator.size());

        std::string remainder;

        for (char bit : numerator)
        {

            remainder.push_back(bit);

            while (remainder.size() > 1 && remainder.front() == '0') {
                remainder.erase(remainder.begin());
            }

            bool negTrial = false;
            std::string trial = subBinaryStrings(remainder, denominator, negTrial);

            if (!negTrial) {

                intQuotient.push_back('1');
                remainder = trial;
            } else {

                intQuotient.push_back('0');
            }
        }

        std::string fracQuotient;
        fracQuotient.reserve(fracBits);

        if (!checkAllZero(remainder) && fracBits > 0)
        {
            for (std::size_t i = 0; i < fracBits; i++)
            {

                remainder.push_back('0');

                while (remainder.size() > 1 && remainder.front() == '0') {
                    remainder.erase(remainder.begin());
                }

                bool negTrial = false;
                std::string trial = subBinaryStrings(remainder, denominator, negTrial);

                if (!negTrial) {

                    fracQuotient.push_back('1');
                    remainder = trial;
                } else {

                    fracQuotient.push_back('0');
                }

                if (checkAllZero(remainder)) {
                    break;
                }
            }
        }

        while (intQuotient.size() > 1 && intQuotient.front() == '0') {
            intQuotient.erase(intQuotient.begin());
        }
        if (intQuotient.empty()) {
            intQuotient = "0";
        }

        if (checkAllZero(fracQuotient)) {
            fracQuotient.clear();
        }

        return {intQuotient, fracQuotient};
    }

    auto decimalToBinaryInteger(const std::string& decimalStr, bool& ) -> std::string {
        if (decimalStr == "0") {
            return "0";
        }
        std::string temp = decimalStr;
        while (temp.size() > 1 && temp.front() == '0') {
            temp.erase(temp.begin());
        }
        std::string bin;
        while (temp != "0") {
            int carry = 0;
            for (int idx = 0; idx < static_cast<int>(temp.size()); idx++) {
                int const val = (carry * DEC_BASE) + (temp[static_cast<std::size_t>(idx)] - '0');
                carry = val % 2;
                int const digit2 = val / 2;
                temp[static_cast<std::size_t>(idx)] = static_cast<char>('0' + digit2);
            }
            bin.push_back((carry == 1) ? '1' : '0');
            while (temp.size() > 1 && temp.front() == '0') {
                temp.erase(temp.begin());
            }
        }
        boost::range::reverse(bin);
        return bin;
    }

    auto binaryToDecimalInteger(const std::string& binStr, bool neg) -> std::string {
        if (checkAllZero(binStr)) {
            if (neg) {
                return "-0";
            }
            return "0";
        }
        long long val = 0;
        for (char const ch : binStr) {
            val = (val << 1) + ((ch == '1') ? 1 : 0);
            if (val > VERY_LARGE_VAL) {
                std::string dec = "0";
                for (char const cc : binStr) {
                    int carry = 0;
                    for (int i = static_cast<int>(dec.size()) - 1; i >= 0; i--) {
                        int const d = ((dec[static_cast<std::size_t>(i)] - '0') * 2) + carry;
                        carry = d / DEC_BASE;
                        dec[static_cast<std::size_t>(i)] = static_cast<char>('0' + (d % DEC_BASE));
                    }
                    if (carry != 0) {
                        dec.insert(dec.begin(), static_cast<char>('0' + carry));
                    }
                    if (cc == '1') {
                        carry = 1;
                        for (int i = static_cast<int>(dec.size()) - 1; i >= 0 && carry != 0; i--) {
                            int const d = (dec[static_cast<std::size_t>(i)] - '0') + 1;
                            carry = d / DEC_BASE;
                            dec[static_cast<std::size_t>(i)] = static_cast<char>('0' + (d % DEC_BASE));
                        }
                        if (carry != 0) {
                            dec.insert(dec.begin(), '1');
                        }
                    }
                }
                if (neg) {
                    dec.insert(dec.begin(), '-');
                }
                return dec;
            }
        }
        std::ostringstream oss;
        if (neg) {
            oss << '-';
        }
        oss << val;
        return oss.str();
    }
    auto binaryFractionToDecimalFraction(const std::string& binFrac) -> std::string
    {
        if (checkAllZero(binFrac)) {
            return "0";
        }

        long double fractionVal = 0.0L;
        long double factor = 0.5L;
        for (char const ch : binFrac) {
            if (ch == '1') {
                fractionVal += factor;
            }
            factor *= 0.5L;
        }

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(100);
        oss << fractionVal;
        std::string sVal = oss.str();

            auto dotPos = sVal.find('.');
            if (dotPos == std::string::npos) {
                return "0";
            }
            std::string fracPart = sVal.substr(dotPos + 1);

            while (fracPart.size() > 1 && fracPart.back() == '0') {
                fracPart.pop_back();
            }
            if (fracPart == "0") {
                return "0";
            }
            return fracPart;
        }

        auto decimalFractionToBinaryFraction(const std::string& decFrac, std::size_t bits) -> std::string {
            if (decFrac.empty() || decFrac == "0") {
                return "";
            }

            std::string tempFrac = decFrac;

            tempFrac.erase(tempFrac.find_last_not_of('0') + 1);

            if (tempFrac.empty()) {
                return "";
            }

            std::string binStr;
            binStr.reserve(bits);

            std::vector<int> digits;
            for (char c : tempFrac) {
                digits.push_back(c - '0');
            }

            for (std::size_t i = 0; i < bits; i++) {
                int carry = 0;
                for (int j = digits.size() - 1; j >= 0; j--) {
                    int temp = digits[j] * 2 + carry;
                    carry = temp / 10;
                    digits[j] = temp % 10;
                }
                binStr.push_back(carry ? '1' : '0');

                bool allZero = true;
                for (int digit : digits) {
                    if (digit != 0) {
                        allZero = false;
                        break;
                    }
                }
                if (allZero) {
                    break;
                }
            }

            while (!binStr.empty() && binStr.back() == '0') {
                binStr.pop_back();
            }

            return binStr;
        }

        auto binaryToLongLong(const std::string& binStr, bool neg) -> long long {
            long long val = 0;
            for (char const ch : binStr) {
                if (val > (std::numeric_limits<long long>::max() >> 1)) {
                    break;
                }
                val = (val << 1) + ((ch == '1') ? 1 : 0);
            }
            return neg ? -val : val;
        }
        }

        std::size_t LongNumber::s_precision = 64;
        std::size_t LongNumber::s_maxPrecision = 1024;
        RoundingMode LongNumber::s_roundMode = RoundingMode::Round;

        LongNumber::LongNumber()
            : negative(false), binInteger("0") {}

        LongNumber::LongNumber(const LongNumber& other)
            : negative(other.negative), binInteger(other.binInteger), binFraction(other.binFraction) {
            std::cout << "Copy Constructor called! Copying: " << other.toString() << std::endl;

            normalize();
            std::cout << "Copied binInteger: " << binInteger << ", binFraction: " << binFraction << std::endl;
        }

        LongNumber::LongNumber(LongNumber&& other) noexcept
            : negative(other.negative),
              binInteger(std::move(other.binInteger)),
              binFraction(std::move(other.binFraction))
        {
            other.negative = false;
            other.binInteger = "0";
            other.binFraction.clear();
        }

        auto LongNumber::operator=(const LongNumber& other) -> LongNumber& {
            if (this != &other) {
                std::cout << "Assignment Operator called! Assigning: " << other.toString() << std::endl;
                negative = other.negative;
                binInteger = other.binInteger;
                binFraction = other.binFraction;

                normalize();
            }
            return *this;
        }

        auto LongNumber::operator=(LongNumber&& other) noexcept -> LongNumber&
        {
            if (this != &other) {
                negative = other.negative;
                binInteger = std::move(other.binInteger);
                binFraction = std::move(other.binFraction);
                other.negative = false;
                other.binInteger = "0";
                other.binFraction.clear();
            }
            return *this;
        }

        LongNumber::~LongNumber() = default;

        LongNumber::LongNumber(int value)
            : negative(value < 0), binInteger("0")
        {
            if (value == 0) {
                binInteger = "0";
                return;
            }
            long long tempVal = (value < 0) ? -(static_cast<long long>(value)) : static_cast<long long>(value);
            std::string buffer;
            while (tempVal > 0) {
                buffer.push_back(((tempVal & 1) != 0) ? '1' : '0');
                tempVal >>= 1;
            }
            if (buffer.empty()) {
                buffer = "0";
            }
            boost::range::reverse(buffer);
            binInteger = buffer;
            normalize();
        }

        LongNumber::LongNumber(long long value)
            : negative(value < 0), binInteger("0")
        {
            if (value == 0) {
                binInteger = "0";
                return;
            }
            long long tempVal = (value < 0) ? -value : value;
            std::string buffer;
            while (tempVal > 0) {
                buffer.push_back(((tempVal & 1) != 0) ? '1' : '0');
                tempVal >>= 1;
            }
            if (buffer.empty()) {
                buffer = "0";
            }
            boost::range::reverse(buffer);
            binInteger = buffer;
            normalize();
        }

        LongNumber::LongNumber(long double value)
            : negative(value < 0.0L), binInteger("0")
        {
            long double const absVal = (value < 0.0L) ? -value : value;
            if (absVal < EPSILON) {
                binInteger = "0";
                binFraction.clear();
                negative = false;
                return;
            }
            auto whole = static_cast<long long>(absVal);
            long double const frac = absVal - static_cast<long double>(whole);
            {
                std::string buffer;
                if (whole == 0) {
                    buffer = "0";
                }
                while (whole > 0) {
                    buffer.push_back(((whole & 1) != 0) ? '1' : '0');
                    whole >>= 1;
                }
                if (buffer.empty()) {
                    buffer = "0";
                }
                boost::range::reverse(buffer);
                binInteger = buffer;
            }
            {
                std::size_t const bits = std::min(s_precision, s_maxPrecision);
                long double fracAcc = frac;
                std::string buffer;
                buffer.reserve(bits);
                for (std::size_t i = 0; i < bits; i++) {
                    fracAcc *= 2.0L;
                    if (fracAcc >= 1.0L) {
                        buffer.push_back('1');
                        fracAcc -= 1.0L;
                    } else {
                        buffer.push_back('0');
                    }
                }
                binFraction = buffer;
            }
            clamp();
            normalize();
        }

        LongNumber::LongNumber(const std::string& str)
        : negative(false), binInteger("0"), binFraction("")
    {

        size_t dotPos = str.find('.');

        std::string intPart = str.substr(0, dotPos);
        std::string fracPart = (dotPos != std::string::npos) ? str.substr(dotPos + 1) : "";

        if (!intPart.empty() && intPart[0] == '-') {
            negative = true;
            intPart.erase(0, 1);
        }

        intPart.erase(0, intPart.find_first_not_of('0'));
        if (intPart.empty()) {
            intPart = "0";
        }

        fracPart.erase(fracPart.find_last_not_of('0') + 1);
        if (fracPart.empty()) {
            fracPart = "";
        }

        bool dummy = false;

        binInteger = ::decimalToBinaryInteger(intPart, dummy);

        if (!fracPart.empty()) {

            binFraction = ::decimalFractionToBinaryFraction(fracPart, 30);
        }

        normalize();
    }

        void LongNumber::setPrecision(std::size_t bits)
        {
            validatePrecision(bits);
            s_precision = bits;
        }

        auto LongNumber::getPrecision() -> std::size_t
        {
            return s_precision;
        }

        void LongNumber::setMaxPrecision(std::size_t maxBits)
        {
            s_maxPrecision = maxBits;
        }

        auto LongNumber::getMaxPrecision() -> std::size_t
        {
            return s_maxPrecision;
        }

        void LongNumber::setRoundingMode(RoundingMode mode)
        {
            s_roundMode = mode;
        }

        auto LongNumber::getRoundingMode() -> RoundingMode
        {
            return s_roundMode;
        }
        auto operator+(const LongNumber& lhs, const LongNumber& rhs) -> LongNumber
{

    if (::checkAllZero(lhs.binInteger) && lhs.binFraction.empty()) {
        return rhs;
    }
    if (::checkAllZero(rhs.binInteger) && rhs.binFraction.empty()) {
        return lhs;
    }

    if (lhs.negative == rhs.negative) {
        LongNumber result;
        result.negative = lhs.negative;

        std::string intPart = ::addBinaryStrings(lhs.binInteger, rhs.binInteger);

        std::size_t maxFracLen = std::max(lhs.binFraction.size(), rhs.binFraction.size());
        std::string lf = lhs.binFraction;
        std::string rf = rhs.binFraction;

        lf.resize(maxFracLen, '0');
        rf.resize(maxFracLen, '0');

        int carry = 0;
        std::string fracRes(maxFracLen, '0');

        for (int i = static_cast<int>(maxFracLen) - 1; i >= 0; i--) {
            int sum = carry + ((lf[static_cast<std::size_t>(i)] == '1') ? 1 : 0)
                              + ((rf[static_cast<std::size_t>(i)] == '1') ? 1 : 0);
            fracRes[static_cast<std::size_t>(i)] = (sum % 2) ? '1' : '0';
            carry = sum / 2;
        }

        if (carry != 0) {
            intPart = ::addBinaryStrings(intPart, "1");
        }

        result.binInteger = intPart;
        result.binFraction = fracRes;

        result.clamp();
        result.normalize();
        return result;
    }

    if (lhs.negative) {
        LongNumber lhsPos = lhs;
        lhsPos.negative = false;
        return rhs - lhsPos;
    } else {
        LongNumber rhsPos = rhs;
        rhsPos.negative = false;
        return lhs - rhsPos;
    }
}

        auto operator-(const LongNumber& lhs, const LongNumber& rhs) -> LongNumber
        {
            if (::checkAllZero(rhs.binInteger) && rhs.binFraction.empty()) {
                return lhs;
            }
            if (::checkAllZero(lhs.binInteger) && lhs.binFraction.empty()) {
                LongNumber temp = rhs;
                temp.negative = !temp.negative;
                return temp;
            }
            if (lhs.negative != rhs.negative) {
                LongNumber rCopy = rhs;
                rCopy.negative = !rCopy.negative;
                return lhs + rCopy;
            }         bool negRes = false;
                std::string intPart = ::subBinaryStrings(lhs.binInteger, rhs.binInteger, negRes);
                bool bothNeg = lhs.negative && rhs.negative;
                if (negRes) {
                    bothNeg = !bothNeg;
                }
                std::size_t maxFracLen = std::max(lhs.binFraction.size(), rhs.binFraction.size());
                std::string lf = lhs.binFraction;
                lf.resize(maxFracLen, '0');
                std::string rf = rhs.binFraction;
                rf.resize(maxFracLen, '0');
                int borrow = 0;
                std::string fracRes;
                fracRes.resize(maxFracLen, '0');
                for (int i = static_cast<int>(maxFracLen) - 1; i >= 0; i--) {
                    int leftBit = (lf[static_cast<std::size_t>(i)] == '1') ? 1 : 0;
                    int rightBit = (rf[static_cast<std::size_t>(i)] == '1') ? 1 : 0;
                    int diff = leftBit - rightBit - borrow;
                    if (diff < 0) {
                        diff += 2;
                        borrow = 1;
                    } else {
                        borrow = 0;
                    }
                    fracRes[static_cast<std::size_t>(i)] = ((diff == 1) ? '1' : '0');
                }
                if (borrow != 0) {
                    bool dummy = false;
                    intPart = ::subBinaryStrings(intPart, "1", dummy);
                    if (dummy) {
                        bothNeg = !bothNeg;
                    }
                }
                LongNumber result;
                result.negative = bothNeg;
                result.binInteger = intPart;
                result.binFraction = fracRes;
                result.clamp();
                result.normalize();
                return result;

        }

        auto operator*(const LongNumber& lhs, const LongNumber& rhs) -> LongNumber
        {
            LongNumber result;
            bool const lhsZero = ::checkAllZero(lhs.binInteger) && lhs.binFraction.empty();
            bool const rhsZero = ::checkAllZero(rhs.binInteger) && rhs.binFraction.empty();
            bool const isZero = lhsZero || rhsZero;
            result.negative = (lhs.negative != rhs.negative) && !isZero;
            std::size_t const fracLen = lhs.binFraction.size() + rhs.binFraction.size();
            std::string const leftAll = lhs.binInteger + lhs.binFraction;
            std::string const rightAll = rhs.binInteger + rhs.binFraction;
            std::string product = ::multiplyBinaryStrings(leftAll, rightAll);
            if (product.size() <= fracLen) {
                product.insert(product.begin(), (fracLen - product.size()) + 1, '0');
            }
            std::size_t const intLen = product.size() - fracLen;
            result.binInteger = product.substr(0, intLen);
            result.binFraction = product.substr(intLen);
            if (result.binInteger.empty()) {
                result.binInteger = "0";
            }
            result.clamp();
            result.normalize();
            return result;
        }
        std::string integerDivideBinary(const std::string& lhsBin, const std::string& rhsBin);
        LongNumber operator/(const LongNumber& lhs, const LongNumber& rhs)
        {

            if (rhs.binInteger == "0" && rhs.binFraction.empty()) {
                throw LongNumberException("Division by zero");
            }

            if (lhs.binInteger == "0" && lhs.binFraction.empty()) {
                return LongNumber("0");
            }

            LongNumber result;
            result.negative = (lhs.negative != rhs.negative);

            const std::size_t lhsFracSize = lhs.binFraction.size();
            const std::size_t rhsFracSize = rhs.binFraction.size();

            std::string numerator   = lhs.binInteger + lhs.binFraction;
            std::string denominator = rhs.binInteger + rhs.binFraction;

            while (numerator.size()   > 1 && numerator.front()   == '0') numerator.erase(numerator.begin());
            while (denominator.size() > 1 && denominator.front() == '0') denominator.erase(denominator.begin());
            if (numerator.empty())   numerator   = "0";
            if (denominator.empty()) denominator = "0";

            long long difference = static_cast<long long>(rhsFracSize) - static_cast<long long>(lhsFracSize);
            if (difference > 0) {
                numerator.append(static_cast<std::size_t>(difference), '0');
            }
            else if (difference < 0) {
                denominator.append(static_cast<std::size_t>(-difference), '0');
            }

            const std::size_t fracBits = LongNumber::getPrecision();
            auto [qStr, fStr] = divideBinaryStrings(numerator, denominator, fracBits);
            result.binInteger  = qStr;
            result.binFraction = fStr;

            while (result.binInteger.size() > 1 && result.binInteger.front() == '0') {
                result.binInteger.erase(result.binInteger.begin());
            }
            if (result.binInteger.empty()) {
                result.binInteger = "0";
            }

            result.clamp();
            result.normalize();

            return result;
        }

        std::string integerDivideBinary(const std::string& lhsBin, const std::string& rhsBin)
        {

            std::string quotient;
            quotient.reserve(lhsBin.size());
            std::string current;

            for (char bit : lhsBin)
            {
                current.push_back(bit);
                while (current.size() > 1 && current.front() == '0')
                {
                    current.erase(current.begin());
                }
                bool negTrial = false;
                std::string trial = subBinaryStrings(current, rhsBin, negTrial);
                if (!negTrial)
                {
                    quotient.push_back('1');
                    current = trial;
                }
                else
                {
                    quotient.push_back('0');
                }
            }
            if (quotient.empty())
            {
                quotient = "0";
            }
            while (quotient.size() > 1 && quotient.front() == '0')
            {
                quotient.erase(quotient.begin());
            }
            return quotient;
        }

        auto operator%(const LongNumber& lhs, const LongNumber& rhs) -> LongNumber
        {
            if (rhs.binInteger == "0" && rhs.binFraction.empty()) {
                throw LongNumberException("Division by zero in mod");
            }
            bool const lhsZero = ::checkAllZero(lhs.binInteger) && lhs.binFraction.empty();
            if (lhsZero) {
                return LongNumber("0");
            }
            LongNumber const quotient = lhs / rhs;
            LongNumber floorQ = quotient;
            floorQ.binFraction.clear();
            if (quotient.negative) {
                bool const isQZero = ::checkAllZero(quotient.binInteger) && quotient.binFraction.empty();
                if (!isQZero) {
                    bool const fracNonZero = !::checkAllZero(quotient.binFraction);
                    if (fracNonZero) {
                        floorQ = floorQ - LongNumber("1");
                    }
                }
            }
            LongNumber remainder = lhs - (floorQ * rhs);
            return remainder;
        }

        auto operator==(const LongNumber& lhs, const LongNumber& rhs) -> bool
        {
            if (lhs.negative != rhs.negative) {
                bool const lhsZ = ::checkAllZero(lhs.binInteger) && lhs.binFraction.empty();
                bool const rhsZ = ::checkAllZero(rhs.binInteger) && rhs.binFraction.empty();
                return lhsZ && rhsZ;
            }
            return (lhs.binInteger == rhs.binInteger) && (lhs.binFraction == rhs.binFraction);
        }

        auto operator!=(const LongNumber& lhs, const LongNumber& rhs) -> bool
        {
            return !(lhs == rhs);
        }

        auto operator<(const LongNumber& lhs, const LongNumber& rhs) -> bool
        {
            if (lhs == rhs) {
                return false;
            }
            if (lhs.negative && !rhs.negative) {
                return true;
            }
            if (!lhs.negative && rhs.negative) {
                return false;
            }
            bool const bothNeg = (lhs.negative && rhs.negative);
            if (lhs.binInteger.size() != rhs.binInteger.size()) {
                if (lhs.binInteger.size() < rhs.binInteger.size()) {
                    return !bothNeg;
                }
                return bothNeg;
            }
            if (lhs.binInteger != rhs.binInteger) {
                bool const less = (lhs.binInteger < rhs.binInteger);
                return (less) ^ bothNeg;
            }
            bool const lessFrac = (lhs.binFraction < rhs.binFraction);
            return lessFrac ^ bothNeg;
        }

        auto operator>(const LongNumber& lhs, const LongNumber& rhs) -> bool
        {
            return (rhs < lhs);
        }

        auto operator<=(const LongNumber& lhs, const LongNumber& rhs) -> bool
        {
            return !(lhs > rhs);
        }

        auto operator>=(const LongNumber& lhs, const LongNumber& rhs) -> bool
        {
            return !(lhs < rhs);
        }

        auto pow(const LongNumber& base, const LongNumber& exponent) -> LongNumber
        {
            long long const expVal = exponent.toLongLong();
            if (expVal < 0) {
                LongNumber const one("1");
                return one / pow(base, LongNumber(-expVal));
            }
            LongNumber result("1");
            LongNumber curr = base;
            long long eCopy = expVal;
            while (eCopy > 0) {
                if ((eCopy & 1) != 0) {
                    result = result * curr;
                }
                curr = curr * curr;
                eCopy >>= 1;
            }
            return result;
        }

        auto sqrt(const LongNumber& x) -> LongNumber
        {
            if (x.negative) {
                throw LongNumberException("sqrt of negative number");
            }
            bool const xZero = (::checkAllZero(x.binInteger) && x.binFraction.empty());
            if (xZero) {
                return x;
            }
            LongNumber guess = x;
            LongNumber const two("2");
            for (int i = 0; i < MAX_ITERATIONS; i++) {
                LongNumber const update = (guess + (x / guess)) / two;
                if (update == guess) {
                    break;
                }
                guess = update;
            }
            return guess;
        }

        auto ln(const LongNumber& x) -> LongNumber
        {
            if (x.negative) {
                throw LongNumberException("ln domain error");
            }
            bool const xZero = (::checkAllZero(x.binInteger) && x.binFraction.empty());
            if (xZero) {
                throw LongNumberException("ln domain error");
            }
            LongNumber const one("1");
            if (x == one) {
                return LongNumber("0");
            }
            return LongNumber("0");
        }

        auto exp(const LongNumber&  ) -> LongNumber
        {
            return LongNumber("1");
        }

        auto sin(const LongNumber&  ) -> LongNumber
        {
            return LongNumber("0");
        }

        auto cos(const LongNumber&  ) -> LongNumber
        {
            return LongNumber("1");
        }

        auto tan(const LongNumber& x) -> LongNumber
        {
            LongNumber const sVal = sin(x);
            LongNumber const cVal = cos(x);
            bool const zeroCos = (cVal.binInteger == "0" && cVal.binFraction.empty());
            if (zeroCos) {
                throw LongNumberException("tan undefined (cos(x) = 0)");
            }
            return sVal / cVal;
        }

        auto operator<<(std::ostream& os, const LongNumber& num) -> std::ostream&
        {
            os << num.toString();
            return os;
        }

        auto operator>>(std::istream& is, LongNumber& num) -> std::istream&
        {
            std::string param;
            is >> param;
            num = LongNumber(param);
            return is;
        }

        auto LongNumber::toDecimalString() const -> std::string
        {
            bool const zAll = (::checkAllZero(binInteger) && binFraction.empty());
            if (zAll) {
                return "0";
            }

            std::string const intDec = ::binaryToDecimalInteger(binInteger, negative);
            std::string output = intDec;

            if (!binFraction.empty()) {
                std::string fracDec = ::binaryFractionToDecimalFraction(binFraction);

                if (fracDec != "0") {
                    output.push_back('.');
                    output.append(fracDec);

                    while (output.back() == '0')
                        output.pop_back();

                    if (output.back() == '.')
                        output.pop_back();
                }
            }

            return output;
        }

        auto LongNumber::toBinaryString() const -> std::string
        {
            bool const zeroAll = (::checkAllZero(binInteger) && binFraction.empty());
            std::string ret;
            if (negative && !zeroAll) {
                ret.push_back('-');
            }
            ret.append(binInteger);
            if (!binFraction.empty()) {
                ret.push_back('.');
                ret.append(binFraction);
            }
            return ret;
        }

        auto LongNumber::toString() const -> std::string
        {
            return toDecimalString();
        }

        auto LongNumber::fromBinaryString(const std::string& bin) -> LongNumber
        {
            LongNumber tmp;
            bool neg = false;
            std::string local = bin;
            if (!local.empty() && local[0] == '-') {
                neg = true;
                local.erase(local.begin());
            }
            auto pos = local.find('.');
            if (pos == std::string::npos) {
                tmp.binInteger = local;
                tmp.binFraction.clear();
            } else {
                tmp.binInteger = local.substr(0, pos);
                tmp.binFraction = local.substr(pos + 1);
            }
            if (tmp.binInteger.empty()) {
                tmp.binInteger = "0";
            }
            bool const zeroAll = (::checkAllZero(tmp.binInteger) && tmp.binFraction.empty());
            tmp.negative = neg && !zeroAll;
            tmp.clamp();
            tmp.normalize();
            return tmp;
        }

        auto LongNumber::fromDecimalString(const std::string& dec) -> LongNumber {
            if (dec.empty()) {
                throw LongNumberException("Invalid decimal string");
            }

            std::string localDec = dec;
            bool negFlag = false;

            if (localDec[0] == '-') {
                negFlag = true;
                localDec.erase(localDec.begin());
            } else if (localDec[0] == '+') {
                localDec.erase(localDec.begin());
            }

            auto dotPos = localDec.find('.');
            std::string intPart, fracPart;

            if (dotPos == std::string::npos) {
                intPart = localDec;
                fracPart = "";
            } else {
                intPart = localDec.substr(0, dotPos);
                fracPart = localDec.substr(dotPos + 1);
            }

            while (intPart.size() > 1 && intPart.front() == '0') {
                intPart.erase(intPart.begin());
            }

            while (!fracPart.empty() && fracPart.back() == '0') {
                fracPart.pop_back();
            }

            if (intPart.empty()) {
                intPart = "0";
            }

            std::cout << "Integer Part: " << intPart << std::endl;
            std::cout << "Fraction Part: " << fracPart << std::endl;

            LongNumber tmp;
            tmp.negative = negFlag && (intPart != "0" || !fracPart.empty());
            tmp.binInteger = ::decimalToBinaryInteger(intPart, tmp.negative);

            if (!fracPart.empty()) {
                std::size_t bits = std::min(s_precision, s_maxPrecision);
                tmp.binFraction = ::decimalFractionToBinaryFraction(fracPart, bits);
            }

            tmp.clamp();
            tmp.normalize();
            return tmp;
        }

        auto LongNumber::fromScientificString(const std::string& sci) -> LongNumber
        {
            std::size_t const epos = sci.find_first_of("eE");
            if (epos == std::string::npos) {
                return fromDecimalString(sci);
            }
            std::string const base = sci.substr(0, epos);
            std::string expStr = sci.substr(epos + 1);
            bool negExp = false;
            if (!expStr.empty() && (expStr[0] == '+' || expStr[0] == '-')) {
                negExp = (expStr[0] == '-');
                expStr.erase(expStr.begin());
            }
            if (expStr.empty()) {
                throw LongNumberException("Invalid scientific notation");
            }
            long long exponentVal = 0;
            for (char const ch : expStr) {
                if (std::isdigit(ch) == 0) {
                    throw LongNumberException("Invalid exponent in scientific notation");
                }
                exponentVal = exponentVal * DEC_BASE + (ch - '0');
                if (exponentVal > MAX_EXP_CHECK) {
                    break;
                }
            }
            if (negExp) {
                exponentVal = -exponentVal;
            }
            LongNumber bVal = fromDecimalString(base);
            if (exponentVal == 0) {
                return bVal;
            }
            LongNumber const ten("10");
            LongNumber const p = pow(LongNumber("10"), LongNumber(exponentVal));
            if (exponentVal > 0) {
                return bVal * p;
            }
            return bVal / p;
        }

        auto LongNumber::toDouble() const -> double
        {
            long long const iv = ::binaryToLongLong(binInteger, false);
            auto const val = static_cast<double>(iv);
            double fracVal = 0.0;
            double factor = 0.5;
            for (char const ch : binFraction) {
                if (ch == '1') {
                    fracVal += factor;
                }
                factor *= 0.5;
            }
            double const total = val + fracVal;
            return negative ? -total : total;
        }

        auto LongNumber::toInt() const -> int
        {
            return static_cast<int>(toLongLong());
        }

        auto LongNumber::toLongLong() const -> long long
        {
            return ::binaryToLongLong(binInteger, negative);
        }

        void LongNumber::normalize() {

            if (binInteger.empty()) {
                binInteger = "0";
            }
            while (binInteger.size() > 1 && binInteger.front() == '0') {
                binInteger.erase(binInteger.begin());
            }

            if (binInteger == "0") {

                bool const allFracZero = ::checkAllZero(binFraction);
                if (allFracZero) {
                    negative = false;
                }
            }

            if (!binFraction.empty()) {
                while (binFraction.size() > 1 && binFraction.back() == '0') {
                    binFraction.pop_back();
                }
            }
        }

        void LongNumber::clamp() {
            if (binFraction.size() > s_precision) {

                std::string tail = binFraction.substr(s_precision);
                binFraction.erase(s_precision);

                if (s_roundMode == RoundingMode::Round && !::checkAllZero(tail)) {
                    bool carry = true;
                    int idx = static_cast<int>(binFraction.size()) - 1;
                    while (carry && idx >= 0) {
                        if (binFraction[static_cast<std::size_t>(idx)] == '0') {
                            binFraction[static_cast<std::size_t>(idx)] = '1';
                            carry = false;
                        } else {
                            binFraction[static_cast<std::size_t>(idx)] = '0';
                            idx--;
                        }
                    }

                    if (carry) {
                        binInteger = ::addBinaryStrings(binInteger, "1");
                    }
                }
            }
        }

        void LongNumber::validatePrecision(std::size_t bits)
        {
            if (bits > s_maxPrecision) {
                throw LongNumberException("Precision exceeds maxPrecision");
            }
        }