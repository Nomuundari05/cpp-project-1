#include "../include/LongNumber.hpp"
#include <iostream>
#include <algorithm>


LongNumber::LongNumber() : precision(0), is_negative(false) {}

LongNumber::LongNumber(const LongNumber& other)
    : integer_digits(other.integer_digits), fractional_digits(other.fractional_digits),
      precision(other.precision), is_negative(other.is_negative) {}

LongNumber::~LongNumber() {}

LongNumber& LongNumber::operator=(const LongNumber& other) {
    if (this != &other) {
        integer_digits = other.integer_digits;
        fractional_digits = other.fractional_digits;
        precision = other.precision;
        is_negative = other.is_negative;
    }
    return *this;
}

//Function for making numbers have the same structure
void LongNumber::align_parts(const LongNumber& other, LongNumber& result) const {
    result.precision = std::max(precision, other.precision);

    // Align fractional parts
    result.fractional_digits = fractional_digits;
    result.fractional_digits.resize(result.precision, 0); // Pad with zeros if necessary


    // Align integer parts
    size_t max_integer_size = std::max(integer_digits.size(), other.integer_digits.size());
    result.integer_digits = integer_digits;
    result.integer_digits.insert(result.integer_digits.begin(), max_integer_size - integer_digits.size(), 0); //pads with 0 in the front

}

// Function for removing unnecessary zeros and digts
void LongNumber::normalize() {
    // Remove leading 0
    while (integer_digits.size() > 1 && integer_digits[0] == 0) {
        integer_digits.erase(integer_digits.begin());
    }

    // Remove trailing 0
    while (!fractional_digits.empty() && fractional_digits.back() == 0) {
        fractional_digits.pop_back();
    }

    // If integer part is empty, set it to 0
    if (integer_digits.empty()) {
        integer_digits.push_back(0);
    }

    precision = fractional_digits.size();
}

//Function for taking the absolute value
LongNumber LongNumber::absolute_value() const {
    LongNumber result = *this; 
    result.is_negative = false;
    return result;
}

LongNumber LongNumber::operator+(const LongNumber& other) const {
    LongNumber result;

    // If both numbers are negative
    if (is_negative && other.is_negative) {
        result = absolute_value() + other.absolute_value();
        result.is_negative = true;
        return result;
    } 
    // One negative, one positive
    else if (is_negative != other.is_negative) {
        return is_negative ? other - absolute_value() : *this - other.absolute_value();
    }

    // Align both numbers
    LongNumber aligned_this = *this;
    LongNumber aligned_other = other;
    align_parts(aligned_other, aligned_this);
    aligned_other.align_parts(*this, aligned_other);

    int carry = 0;
    int frac_size = aligned_this.fractional_digits.size(); 

    // Add fractional digits
    result.fractional_digits.resize(frac_size);
    for (int i = frac_size - 1; i >= 0; --i) {
        int sum = aligned_this.fractional_digits[i] + aligned_other.fractional_digits[i] + carry;
        result.fractional_digits[i] = sum % 2;
        carry = sum / 2;
    }

    // Add integer digits
    int int_size = aligned_this.integer_digits.size(); 
    result.integer_digits.resize(int_size);
    for (int i = int_size - 1; i >= 0; --i) {
        int sum = aligned_this.integer_digits[i] + aligned_other.integer_digits[i] + carry;
        result.integer_digits[i] = sum % 2;
        carry = sum / 2;
    }

    if (carry) {
        result.integer_digits.insert(result.integer_digits.begin(), carry);
    }

    result.normalize();
    return result;
}

LongNumber LongNumber::operator-(const LongNumber& other) const {
    LongNumber result;

    if (is_negative && other.is_negative) {
        if (absolute_value() > other.absolute_value()) { 
            result = absolute_value() - other.absolute_value();
            result.is_negative = true;
        } else {
            result = other.absolute_value() - absolute_value();
            result.is_negative = false;
        }
        return result;
    } 
    
    if (is_negative != other.is_negative) {
        result = absolute_value() + other.absolute_value();
        result.is_negative = is_negative;
        return result;
    }

    // Align the parts
    LongNumber aligned_this = *this;
    LongNumber aligned_other = other;
    align_parts(aligned_other, aligned_this);
    aligned_other.align_parts(*this, aligned_other);

    // Subtract fractional digits
    int borrow = 0;
    int frac_size = aligned_this.fractional_digits.size();
    result.fractional_digits.resize(frac_size);
    for (int i = frac_size - 1; i >= 0; --i) {
        int diff = aligned_this.fractional_digits[i] - aligned_other.fractional_digits[i] - borrow;
        if (diff < 0) {
            diff += 2; // Adjust for binary subtraction
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.fractional_digits[i] = diff;
    }

    // Subtract integer digits
    int int_size = aligned_this.integer_digits.size(); 
    result.integer_digits.resize(int_size);
    for (int i = int_size - 1; i >= 0; --i) {
        int diff = aligned_this.integer_digits[i] - aligned_other.integer_digits[i] - borrow;
        if (diff < 0) {
            diff += 2; 
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.integer_digits[i] = diff;
    }


    if (borrow) {
        // swap and then negate 
        result = other - *this;
        result.is_negative = true;
    } else {
        result.normalize();
    }

    return result;
}
LongNumber LongNumber::operator*(const LongNumber& other) const {
    LongNumber result;
    result.is_negative = (is_negative != other.is_negative);

    LongNumber abs_this = absolute_value();
    LongNumber abs_other = other.absolute_value();

    //size of the merged number
    size_t total_size_this = abs_this.integer_digits.size() + abs_this.fractional_digits.size();
    size_t total_size_other = abs_other.integer_digits.size() + abs_other.fractional_digits.size();

    // Initialize product vector
    std::vector<int> product(total_size_this + total_size_other, 0);

    //multiplication
    for (size_t i = 0; i < total_size_this; ++i) {
        int digit1;
        if (i < abs_this.integer_digits.size()) {
            digit1 = abs_this.integer_digits[i];
        } else {
            digit1 = abs_this.fractional_digits[i - abs_this.integer_digits.size()];
        }
    
        for (size_t j = 0; j < total_size_other; ++j) {
            int digit2;
            if (j < abs_other.integer_digits.size()) {
                digit2 = abs_other.integer_digits[j];
            } else {
                digit2 = abs_other.fractional_digits[j - abs_other.integer_digits.size()];
            }
    
            int mul = digit1 * digit2;
            product[i + j + 1] += mul;
            product[i + j] += product[i + j + 1] / 2;
            product[i + j + 1] %= 2;
        }
    }
    // position of the binary point
    size_t binary_point_position = abs_this.fractional_digits.size() + abs_other.fractional_digits.size();

    // Split the product into integer and fractional parts
    result.integer_digits.assign(product.begin(), product.begin() + product.size() - binary_point_position);
    result.fractional_digits.assign(product.begin() + product.size() - binary_point_position, product.end());

    result.precision = std::max(abs_this.precision, abs_other.precision);

    result.normalize();

    return result;
}

bool LongNumber::operator==(const LongNumber& other) const {
    LongNumber diff = *this - other;
    diff.is_negative = false;
    LongNumber eps = LongNumber::make_number(0.000001, 64); // Adjustable tolerance
    return diff < eps;
}
bool LongNumber::operator!=(const LongNumber& other) const {

    return !(*this == other);
}
bool LongNumber::operator<(const LongNumber& other) const {
    if (is_negative != other.is_negative) {
        return is_negative; // Negative numbers are always smaller
    }

    // Align both numbers before comparison
    LongNumber aligned_this, aligned_other;
    align_parts(other, aligned_this);
    other.align_parts(*this, aligned_other);

    // Compare integer part sizes first
    if (aligned_this.integer_digits.size() != aligned_other.integer_digits.size()) {
        return (aligned_this.integer_digits.size() < aligned_other.integer_digits.size()) ^ is_negative;
    }

    // Compare integer parts
    if (aligned_this.integer_digits != aligned_other.integer_digits) {
        return (aligned_this.integer_digits < aligned_other.integer_digits) ^ is_negative;
    }

    // Compare fractional parts directly (
    return (aligned_this.fractional_digits < aligned_other.fractional_digits) ^ is_negative;
}



bool LongNumber::operator>(const LongNumber& other) const {
    if (is_negative != other.is_negative) {
        return other.is_negative; // A positive number is always greater than a negative number
    }

    // Align both numbers before comparison
    LongNumber aligned_this, aligned_other;
    align_parts(other, aligned_this);
    other.align_parts(*this, aligned_other);

    if (aligned_this.integer_digits.size() != aligned_other.integer_digits.size()) {
        return (aligned_this.integer_digits.size() > aligned_other.integer_digits.size()) ^ is_negative;
    }

    // Compare integer parts
    if (aligned_this.integer_digits != aligned_other.integer_digits) {
        return (aligned_this.integer_digits > aligned_other.integer_digits) ^ is_negative;
    }

    // Compare fractional parts
    return (aligned_this.fractional_digits > aligned_other.fractional_digits) ^ is_negative;
}


std::string LongNumber::to_string() const {
    std::string result;

    if (integer_digits.empty() && fractional_digits.empty()) {
        return "0";
    }
    if (is_negative) result += "-";

    // Remove leading zeros from integer part
    size_t start = 0;
    while (start < integer_digits.size() - 1 && integer_digits[start] == 0) {
        start++;
    }

    for (size_t i = start; i < integer_digits.size(); i++) {
        result += std::to_string(integer_digits[i]);
    }

    if (!fractional_digits.empty()) {
        result += ".";
        for (int digit : fractional_digits) {
            result += std::to_string(digit);
        }
    }

    return result;
}

void LongNumber::print() const {
    std::cout << to_string() << std::endl;
}



LongNumber LongNumber::make_number(long double number, int desired_precision) {
    LongNumber result;
    result.is_negative = (number < 0);
    number = std::abs(number);
    
    result.precision = desired_precision;

    long long int_part = static_cast<long long>(number);
    long double frac_part = number - int_part;

    if (int_part == 0) {
        result.integer_digits.push_back(0);
    } else {
        while (int_part > 0) {
            result.integer_digits.insert(result.integer_digits.begin(), int_part % 2);
            int_part /= 2;
        }
    }


    while (result.fractional_digits.size() < result.precision) {
        frac_part *= 2;
        result.fractional_digits.push_back(static_cast<int>(frac_part));
        frac_part -= static_cast<int>(frac_part);
        if (frac_part < 0)
            frac_part = 0;
    }

    return result;
}
