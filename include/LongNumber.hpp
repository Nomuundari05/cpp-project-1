#ifndef LONGNUM_HPP
#define LONGNUM_HPP

#include<iostream>
#include<vector>

class Longnumber{
    std::vector<int>binary_digits;
    int precision;
public:
    LongNumber();
    LongNumber(const LongNumber& other); // copy constructor
    ~LongNumber();

    LongNumber& operator=(const LongNumber& other);
    LongNumber operator+(const LongNumber& other) const;
    LongNumber operator-(const LongNumber& other) const;
    LongNumber operator*(const LongNumber& other) const;
    LongNumber operator/(const LongNumber& other) const;

    bool operator==(const LongNumber& other) const;
    bool operator!=(const LongNumber& other) const;
    bool operator<(const LongNumber& other) const;
    bool operator>(const LongNumber& other) const;

    void SettingPrecision(int bits)
    friend LongNumber operator""_longnum(long double number);

    void print();

};
#endif

