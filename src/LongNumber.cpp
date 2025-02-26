#include "../include/LongNumber.hpp"
#include <iostream>
#include <algorithm>


LongNumber::LongNumber() : precision(0), is_negative(false) {}

LongNumber::LongNumber(const LongNumber& other)
    : int_part(other.int_part), frac_part(other.frac_part),
      precision(other.precision), is_negative(other.is_negative) {}

LongNumber::~LongNumber() {}

LongNumber& LongNumber::operator=(const LongNumber& other) {
    if (this != &other) {
        int_part = other.int_part;
        frac_part = other.frac_part;
        precision = other.precision;
        is_negative = other.is_negative;
    }
    return *this;
}


void LongNumber::align_parts(const LongNumber& other, LongNumber& result) const { // Function for making 2 numbers have the same size before +,-
    
    result.precision = std::max(precision, other.precision);
    result.frac_part = frac_part;                                            // Align fractional parts
    result.frac_part.resize(result.precision, 0);                        // add 0 in the rightmost 

    size_t max_int_part_size = std::max(int_part.size(), other.int_part.size()); // Align integer parts
    result.int_part = int_part;
    result.int_part.insert(result.int_part.begin(), max_int_part_size - int_part.size(), 0); //pads with 0 in the front
}


void LongNumber::normalize() {                              // Function for removing unnecessary zeros and digits

    while (int_part.size() > 1 && int_part[0] == 0) {                  // Remove leading 0
        int_part.erase(int_part.begin());
    }

    while (!frac_part.empty() && frac_part.back() == 0) {     // Remove trailing 0
        frac_part.pop_back();
    }

    if (int_part.empty()) {                                      // If integer part is empty, set it to 0
        int_part.push_back(0);
    }
    precision = frac_part.size();
}

LongNumber LongNumber::absolute_value() const {
    LongNumber result = *this; 
    result.is_negative = false;
    return result;
}

LongNumber LongNumber::operator+(const LongNumber& other) const {
    LongNumber result;

    if (is_negative && other.is_negative) {                   // If both numbers are negative
        result = absolute_value() + other.absolute_value();
        result.is_negative = true;
        return result;
    } 
    else if (is_negative != other.is_negative) {                  // One negative, one positive
        if (is_negative) {
            return other - absolute_value();
        } else {
            return *this - other.absolute_value();
        }
    }

    LongNumber aligned_this = *this;                 
    LongNumber aligned_other = other;

    align_parts(aligned_other, aligned_this);                      // Align both numbers
    aligned_other.align_parts(*this, aligned_other);

    int carry = 0;
    int frac_part_size = aligned_this.frac_part.size(); 

    result.frac_part.resize(frac_part_size);                          // Add fractional digits
    for (int i = frac_part_size - 1; i >= 0; --i) {
        int sum = aligned_this.frac_part[i] + aligned_other.frac_part[i] + carry;
        result.frac_part[i] = sum % 2;
        carry = sum / 2;
    }

    int int_part_size = aligned_this.int_part.size(); 
    result.int_part.resize(int_part_size);                                 // Add integer digits
    for (int i = int_part_size - 1; i >= 0; --i) {
        int sum = aligned_this.int_part[i] + aligned_other.int_part[i] + carry;
        result.int_part[i] = sum % 2;
        carry = sum / 2;
    }

    if (carry) {                                    // Handle remaining carry
        result.int_part.insert(result.int_part.begin(), carry);
    }

    result.normalize();
    return result;
}
LongNumber LongNumber::operator-(const LongNumber& other) const {
    LongNumber result;

    if (is_negative && other.is_negative) {                     // If both numbers are negative
        if (absolute_value() > other.absolute_value()) { 
            result = absolute_value() - other.absolute_value();
            result.is_negative = true;
        } else {                                                
            result = other.absolute_value() - absolute_value();
            result.is_negative = false;
        }
        return result;
    } 
    
    if (is_negative != other.is_negative) {                     // One negative, one positive
        result = absolute_value() + other.absolute_value();
        result.is_negative = is_negative;
        return result;
    }

    LongNumber aligned_this = *this;                           // Align the parts
    LongNumber aligned_other = other;
    align_parts(aligned_other, aligned_this);
    aligned_other.align_parts(*this, aligned_other);

    int borrow = 0;
    int frac_part_size = aligned_this.frac_part.size();
    result.frac_part.resize(frac_part_size);                              // Subtract fractional digits
    for (int i = frac_part_size - 1; i >= 0; --i) {
        int diff = aligned_this.frac_part[i] - aligned_other.frac_part[i] - borrow;
        if (diff < 0) {
            diff += 2;                                         // Adjust for binary subtraction
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.frac_part[i] = diff;
    }

    int int_part_size = aligned_this.int_part.size(); 
    result.int_part.resize(int_part_size);                               // Subtract integer digits
    for (int i = int_part_size - 1; i >= 0; --i) {
        int diff = aligned_this.int_part[i] - aligned_other.int_part[i] - borrow;
        if (diff < 0) {
            diff += 2;                                         // Adjust for binary subtraction
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.int_part[i] = diff;
    }

    if (borrow) {                                              // Handle borrow by swapping and negating
        result = other - *this;
        result.is_negative = true;
    } else {
        result.normalize();                                    // Normalize the result
    }

    return result;
}
LongNumber LongNumber::operator*(const LongNumber& other) const {
    LongNumber result;
    result.is_negative = (is_negative != other.is_negative);        // Result is negative if signs differ


    LongNumber abs_this = absolute_value();
    LongNumber abs_other = other.absolute_value();

    size_t total_digits_this = abs_this.int_part.size() + abs_this.frac_part.size();
    size_t total_digits_other = abs_other.int_part.size() + abs_other.frac_part.size();

    std::vector<int> result_digits(total_digits_this + total_digits_other, 0); //initialize the product as a vector


    for (size_t i = 0; i < total_digits_this; ++i) {
        int current_digit_this;
        if (i < abs_this.int_part.size()) {
            current_digit_this = abs_this.int_part[i]; // Integer part digit
        } else {
            current_digit_this = abs_this.frac_part[i - abs_this.int_part.size()]; // Fractional part digit
        }
    
        for (size_t j = 0; j < total_digits_other; ++j) {
            int current_digit_other;
            if (j < abs_other.int_part.size()) {
                current_digit_other = abs_other.int_part[j]; // Integer part digit
            } else {
                current_digit_other = abs_other.frac_part[j - abs_other.int_part.size()]; // Fractional part digit
            }
    
            int product = current_digit_this * current_digit_other; // Multiply the digits and adto the result

            result_digits[i + j + 1] += product;                     // Add to the next position
            result_digits[i + j] += result_digits[i + j + 1] / 2;            // Handle carry
            result_digits[i + j + 1] %= 2;                                     // Keep only the remainder
        }
    }

    size_t binary_point_position = abs_this.frac_part.size() + abs_other.frac_part.size();

    result.int_part.assign(result_digits.begin(), result_digits.begin() + result_digits.size() - binary_point_position); //Split the result into int_part and frac_part
    result.frac_part.assign(result_digits.begin() + result_digits.size() - binary_point_position, result_digits.end()); 

    result.precision = std::max(abs_this.precision, abs_other.precision);

    result.normalize();
    return result;

}

bool LongNumber::operator==(const LongNumber& other) const {
    LongNumber diff = *this - other;
    diff.is_negative = false;
    LongNumber eps = LongNumber::Longnum(0.000001, 64);         // Adjustable tolerance
    return diff < eps;
}

bool LongNumber::operator!=(const LongNumber& other) const {
    return !(*this == other);
}

bool LongNumber::operator<(const LongNumber& other) const {
    if (is_negative != other.is_negative) {
        return is_negative;             // Negative numbers are always smaller
    }

    LongNumber aligned_this, aligned_other;
    align_parts(other, aligned_this);
    other.align_parts(*this, aligned_other);

    if (aligned_this.int_part.size() != aligned_other.int_part.size()) {           // Compare integer part sizes first
        return (aligned_this.int_part.size() < aligned_other.int_part.size()) ^ is_negative;
    }

    if (aligned_this.int_part != aligned_other.int_part) {                         // Compare integer parts
        return (aligned_this.int_part < aligned_other.int_part) ^ is_negative;
    }


    return (aligned_this.frac_part < aligned_other.frac_part) ^ is_negative;           // Compare fractional parts directly
}

bool LongNumber::operator>(const LongNumber& other) const {
    if (is_negative != other.is_negative) {
        return other.is_negative;                              // positive number is always greater than a negative number
    }

    LongNumber aligned_this, aligned_other;                  // Align both numbers before comparison
    align_parts(other, aligned_this);
    other.align_parts(*this, aligned_other);

    if (aligned_this.int_part.size() != aligned_other.int_part.size()) {
        return (aligned_this.int_part.size() > aligned_other.int_part.size()) ^ is_negative;
    }

    if (aligned_this.int_part != aligned_other.int_part) {              // Compare integer parts
        return (aligned_this.int_part > aligned_other.int_part) ^ is_negative;
    }

    return (aligned_this.frac_part > aligned_other.frac_part) ^ is_negative; // Compare fractional parts
}

std::string LongNumber::to_string() const {
    std::string result;

    if (int_part.empty() && frac_part.empty()) {                      // If both parts are empty, return "0"
        return "0";
    }
    if (is_negative) result += "-";                       // Add negative sign if needed

    size_t start = 0;                                 // Remove leading zeros from integer part
    while (start < int_part.size() - 1 && int_part[start] == 0) {
        start++;
    }

    for (size_t i = start; i < int_part.size(); i++) {      // Convert integer part to string
        result += std::to_string(int_part[i]);
    }

    if (!frac_part.empty()) {                           // Add fractional part if it exists
        result += ".";
        for (int digit : frac_part) {
            result += std::to_string(digit);
        }
    }

    return result;
}

void LongNumber::print() const {               ///print
    std::cout << to_string() << std::endl;      
}

LongNumber LongNumber::Longnum(long double number, int desired_precision) {
    LongNumber result;
    result.is_negative = (number < 0);                 
    number = std::abs(number);

    result.precision = desired_precision;

    long long int_part = static_cast<long long>(number);      // Extract integer part
    long double frac_part = number - int_part;                // Extract fractional part

    if (int_part == 0) {                                     // Hif 0 int
        result.int_part.push_back(0);
    } else {
        while (int_part > 0) {                               // int_part to binary
            result.int_part.insert(result.int_part.begin(), int_part % 2);
            int_part /= 2;
        }
    }

    while (result.frac_part.size() < result.precision) {          // Convert frac_part to binary
        frac_part *= 2;
        result.frac_part.push_back(static_cast<int>(frac_part));
        frac_part -= static_cast<int>(frac_part);
        if (frac_part < 0)
            frac_part = 0;
    }

    return result;
}