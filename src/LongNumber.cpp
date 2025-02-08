#include "LongNumber.hpp"
#include <iostream>

LongNumber::LongNumber() : precision(0) {}

LongNumber::LongNumber(const LongNumber& other) 
    : binary_digits(other.binary_digits), precision(other.precision) {}
LongNumber::~LongNumber() {}

LongNumber LongNumber::operator+(const LongNumber& other) const {
    LongNumber result;
    int max_precision = std::max(precision, other.precision);
    // copies of the current object

    LongNumber new_this = *this;
    LongNumber new_other = other;

//precision resizing
    new_this.binary_digits.resize(new_this.binary_digits.size()+(max_precision- precision), 0);
    new_other.binary_digits.resize(new_other.binary_digits.size()+(max_precision- other.precision),0);

    // integer resizing
    while (new_this.binary_digits.size() < new_other.binary_digits.size()) {
    new_this.binary_digits.insert(new_this.binary_digits.begin(), 0); 
    }
    while (new_other.binary_digits.size() < new_this.binary_digits.size()) {
    new_other.binary_digits.insert(new_other.binary_digits.begin(), 0);
    }

    //handling the overflow
    int carry = 0;
    size_t max_size = std::max(new_this.binary_digits.size(), new_other.binary_digits.size());
    
    // the loop runs when max_size and carry arent 0 at the same time.
    //max_size is not zero (meaning we still have more bits to process).
    //carry is not zero (meaning there's a carry bit left to add).
    // hervee max size duustsan c gsn carry1 bh yum bol 1 extra step hiigdeh
    
    for (int i = max_size - 1; i >= 0 || carry; --i){

        int sum = new_this.binary_digits[i] + new_other.binary_digits[i] +carry;
        if (sum >= 2) {
            sum -= 2;
            carry = 1;
        } else {
            carry = 0; //uragsha uguh carry bhgu
        }
        
        result.binary_digits.insert(result.binary_digits.begin(), sum);
    }
    if (carry==1) {
        result.binary_digits.insert(result.binary_digits.begin(), 1);
    }

    size_t first_nonzero = 0;
    while (first_nonzero < result.binary_digits.size() && result.binary_digits[first_nonzero] == 0) {
        ++first_nonzero;
    }
    result.binary_digits.erase(result.binary_digits.begin(), result.binary_digits.begin() + first_nonzero);

    if (result.binary_digits.empty()) {
        result.binary_digits.push_back(0);
    }
    result.precision = max_precision;
    return result;
    }

LongNumber LongNumber::operator-(const LongNumber& other) const {
    LongNumber result;
    int max_precision = std::max(precision, other.precision);

    LongNumber new_this = *this
    LongNumber new_other = other;

    //precision resizing
    new_this.binary_digits.resize(new_this.binary_digits.size()+(max_precision- precision), 0);
    new_other.binary_digits.resize(new_other.binary_digits.size()+(max_precision- other.precisiion),0);

    // integer resizing
    while (new_this.binary_digits.size() < new_other.binary_digits.size()) {
    new_this.binary_digits.insert(new_this.binary_digits.begin(), 0); 
    }
    while (new_other.binary_digits.size() < new_this.binary_digits.size()) {
    new_other.binary_digits.insert(new_other.binary_digits.begin(), 0);
    }

    bool this_bigger = true;
    for(int i = new_this.binary_digits.size() -1; i >= 0; --i){
        if(new_this.binary_digits[i] != new_other.binary_digits[i]){
            this_bigger = new_this.binary_digits[i] > new_other.binary_digits[i] //only compare the most sig bit and enough to 
            break;
        }

    }
    if(!this_bigger){
        std::swap(new_this, new_other);
    }

    int borrow = 0;
    for(int i = new_this.binary_digits.size()-1; i >= 0; --i){
        int diff = new_this.binary_digits[i] - new_other.binary_digits[i] - borrow;

        if(diff < 0){
            diff+= 2; 
            borrow = 1;
        } else{
            borrow = 0;
        }

        result.binary_digits.insert(result.binary_digits.begin(), diff);
    }
    size_t first_nonzero = 0;
    while (first_nonzero < result.binary_digits.size() && result.binary_digits[first_nonzero] == 0) {
        ++first_nonzero;
    }
    result.binary_digits.erase(result.binary_digits.begin(), result.binary_digits.begin() + first_nonzero);
    
    if (result.binary_digits.empty()) {
        result.binary_digits.push_back(0);
        result.is_negative = false; // Zero is never negative
    }
    result.precision = max_precision;
    return result;
    }

LongNumber LongNumber::operator*(const LongNumber& other) const {
    LongNumber result;
    int max_precision = precision + other.precision;

    LongNumber new_this = *this;
    LongNumber new_other = other;

    while (new_this.binary_digits.size() < new_other.binary_digits.size()) {
        new_this.binary_digits.insert(new_this.binary_digits.begin(), 0);
    }
    while (new_other.binary_digits.size() < new_this.binary_digits.size()) {
        new_other.binary_digits.insert(new_other.binary_digits.begin(), 0);
    }
    result.binary_digits.resize(new_this.binary_digits.size() + new_other.binary_digits.size(), 0);
    for (int i = new_other.binary_digits.size() - 1; i >= 0; --i){
        if (new_other.binary_digits[i] == 1) {
            LongNumber temp = new_this;
            temp.binary_digits.insert(temp.binary_digits.end(), new_other.binary_digits.size() - 1 - i, 0); // Left shift
            result = result + temp; // Adding shifted value to result
        }
    }

    size_t first_nonzero = 0;
    while (first_nonzero < result.binary_digits.size() && result.binary_digits[first_nonzero] == 0) {
        ++first_nonzero;
    }
    result.binary_digits.erase(result.binary_digits.begin(), result.binary_digits.begin() + first_nonzero);
    if (result.binary_digits.empty()) {
        result.binary_digits.push_back(0);
    }

    result.precision = max_precision;
    return result;
}

LongNumber LongNumber::operator/(const LongNumber& other) const {
    LongNumber result;














    return result;
}

bool LongNumber::operator==(const LongNumber& other) const {
    return binary_digits == other.binary_digits && precision == other.precision;
}

bool LongNumber::operator!=(const LongNumber& other) const {
    return !(*this == other);
}

bool LongNumber::operator<(const LongNumber& other) const {
 
    return false; 
}

bool LongNumber::operator>(const LongNumber& other) const {
    return false;
}


void LongNumber::setPrecision(int bits) {
    precision = bits;
}

void LongNumber::print() const {
    // Implement printing logic
    for (int digit : binary_digits) {
        std::cout << digit;
    }
    std::cout << std::endl;
}


LongNumber operator""_longnum(long double number) {
    LongNumber result;
    return result;
}


