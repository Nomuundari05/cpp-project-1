#ifndef LONGNUMBER_HPP
#define LONGNUMBER_HPP

#include <vector>
#include <string>

class LongNumber {

public:
    std::vector<int> integer_digits; 
    std::vector<int> fractional_digits; 
    int precision; 
    bool is_negative;
    LongNumber();
    LongNumber(const LongNumber& other); // Copy constructor
    ~LongNumber();

    LongNumber& operator=(const LongNumber& other);

    // Arithmetic operators
    LongNumber operator+(const LongNumber& other) const;
    LongNumber operator-(const LongNumber& other) const;
    LongNumber operator*(const LongNumber& other) const;
    //LongNumber operator/(const LongNumber& other) const;

    bool operator==(const LongNumber& other) const;
    bool operator!=(const LongNumber& other) const;
    bool operator<(const LongNumber& other) const;
    bool operator>(const LongNumber& other) const;

    static LongNumber make_number(long double number, int desired_precision);

    std::string to_string() const; 
    void print() const;

    void align_parts(const LongNumber& other, LongNumber& result) const;
    void normalize();
    LongNumber absolute_value() const;
};

#endif