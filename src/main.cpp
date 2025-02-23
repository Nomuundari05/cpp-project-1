#include "../include/LongNumber.hpp"
#include <iostream>
#include <cassert>

void custom_assert(bool condition) {
    if (!condition) {
        throw std::runtime_error("FAIL");
    }
}

bool compare_two_LongNumbers(LongNumber a, LongNumber b) {
    LongNumber c = a - b;
    c.is_negative = false;
    LongNumber eps = LongNumber::make_number(0.00000001, 64); //custom toleration
    if (c < eps)
        return true;
    return false;
}

void testAddition() {
    try{
        LongNumber a = LongNumber::make_number(999.99 ,64);
        LongNumber b = LongNumber::make_number(10.1 ,64);
        LongNumber result = a + b;
        LongNumber expected = LongNumber::make_number(1010.09,64);

        std::cout << "Test Case: Under addition" << std::endl;
        custom_assert(compare_two_LongNumbers(result, expected));
        std::cout << "OK" << std::endl;
        result.print();


        a = LongNumber::make_number(0.1000,64);
        b = LongNumber::make_number(0.55, 50);
        result = a + b;
        expected = LongNumber::make_number(0.65,64);

        std::cout << "Test Case: Addition with different precisions" << std::endl;
        custom_assert(compare_two_LongNumbers(result, expected));
        std::cout << "OK" << std::endl;
        result.print();

        a = LongNumber::make_number(0,15);
        b = LongNumber::make_number(0.0 ,10);
        result = a + b;
        expected = LongNumber::make_number(0.0 ,15);

        std::cout << "Test Case: Zero addition" << std::endl;
        custom_assert(result == expected);
        std::cout << "OK" << std::endl;
        result.print();
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

void testSubtraction() {
    try{
        LongNumber a = LongNumber::make_number(500000.0,64);
        LongNumber b = LongNumber::make_number(100.5 , 60);
        LongNumber result = a - b;
        LongNumber expected = LongNumber::make_number(499899.5 ,64);

        std::cout << "Test Case: Substraction of Large numbers" << std::endl;
        custom_assert(compare_two_LongNumbers(result, expected));
        std::cout << "OK" << std::endl;
        result.print();

        a = LongNumber::make_number(0.2545,32);
        b = LongNumber::make_number(0.1,30);
        result = a - b;
        expected = LongNumber::make_number(0.1545,32);

        std::cout << "Test Case: Substraction of different precisions" << std::endl;
        custom_assert(compare_two_LongNumbers(result, expected));
        std::cout << "OK" << std::endl;
        result.print();

        // Zero subtraction
        a = LongNumber::make_number(0.0,1);
        b = LongNumber::make_number(0.0,10);
        result = a - b;
        expected = LongNumber::make_number(0.0,10);

        std::cout << "Test Case:Subtraction of zeros" << std::endl;
        custom_assert(compare_two_LongNumbers(result, expected));
        std::cout << "OK" << std::endl;
        result.print();
    }catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

void testMultiplication() {
    try{
        LongNumber a = LongNumber::make_number(1234.0,15);
        LongNumber b = LongNumber::make_number(10000.5,15);
        LongNumber result = a * b;
        LongNumber expected = LongNumber::make_number(12340617.0,15);

        std::cout << "Test Case: Multiplication of big numbers " << std::endl;
        custom_assert(compare_two_LongNumbers(result, expected));
        std::cout << "OK" << std::endl;
        result.print();

        a = LongNumber::make_number(-123.23,50);
        b = LongNumber::make_number(4.0,32);
        result = a * b;
        expected = LongNumber::make_number(-492.92, 50);

        std::cout << "Test Case: Multiplication of negative number" << std::endl;
        custom_assert(compare_two_LongNumbers(result, expected));
        std::cout << "OK" << std::endl;
        result.print();

        a = LongNumber::make_number(0.0000001,15);
        b = LongNumber::make_number(0.00000001,15);
        result = a * b;
        expected = LongNumber::make_number(0.0000000000000001,15);
        std::cout << "Multiplication with small numbers" << std:: endl;
        custom_assert(compare_two_LongNumbers(result, expected));
        std::cout << "OK" << std::endl;
        result.print();
    } catch (const std::runtime_error& e){
        std::cout << e. what() << std::endl;
    }
}

void testEquality() {
    try {

        LongNumber a = LongNumber::make_number(100100.0000000,100);
        LongNumber b = LongNumber::make_number(100100.00, 100);

        std::cout << "Test Case: Equality of big numbers" << std::endl;
        custom_assert(a == b);
        std::cout << "OK" << std::endl;

        // Test Case: Small numbers
        a = LongNumber::make_number(0.0000010, 64);
        b = LongNumber::make_number(0.000001,64);

        std::cout << "Test Case:  Equality of Small numbers" << std::endl;
        custom_assert(a == b);
        std::cout << "OK" << std::endl;

        // Test Case: Equality (Zero)
        a = LongNumber::make_number(0.0000000,60);
        b = LongNumber::make_number(-0.0,64);

        std::cout << "Test Case: Equality of Zeros" << std::endl;
        custom_assert(a == b);
        std::cout << "OK" << std::endl;
        
        a = LongNumber::make_number(-1234,15);
        b = LongNumber::make_number(-1234.0000, 30);
        std::cout << "Test Case: Equality of negative numbers" << std::endl;
        custom_assert(a == b);
        std::cout << "OK" << std::endl;

    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

void testInequality() {
    try {

        LongNumber a = LongNumber::make_number(1000000000001.0,15);
        LongNumber b = LongNumber::make_number(1000000000001.01,15);

        std::cout << "Test Case: Inequality of numbers with different (within toleration) precision" << std::endl;
        custom_assert(a != b);
        std::cout << "OK" << std::endl;

        a = LongNumber::make_number(0.000001,30);
        b = LongNumber::make_number(0.000002,32);

        std::cout << "Test Case: Inequality of Small numbers" << std::endl;
        custom_assert(a != b);
        std::cout << "OK" << std::endl;

        a = LongNumber::make_number(-123.01234,50);
        b = LongNumber::make_number(123.01234, 100);

        std::cout << "Test Case: Inequality of negative and positive numbers" << std::endl;
        custom_assert(a != b);
        std::cout << "OK" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    
}


void testLessThan() {
    try {
        // Test 1
        LongNumber a = LongNumber::make_number(50.25,1);
        LongNumber b = LongNumber::make_number(100.5,15);

        std::cout << "Test Case:Less than" << std::endl;
        custom_assert(a < b);
        std::cout << "OK" << std::endl;\

        // Test 2
        a = LongNumber::make_number(0.00001,15);
        b = LongNumber::make_number(0.0001,15);

        std::cout << "Test Case:Less Than- small numbers " << std::endl;
        custom_assert(a < b);
        std::cout << "OK" << std::endl;

        // Test 3
        a = LongNumber::make_number(-0.1,15);
        b = LongNumber::make_number(0.0,15);

        std::cout << "Test Case: Less Than - negative and zero" << std::endl;
        custom_assert(a < b);
        std::cout << "OK" << std::endl;

        // Test 4
        a = LongNumber::make_number(-9999,90);
        b = LongNumber::make_number(-9998,100);

        std::cout << "Test Case: Less Than - negative numbers" << std::endl;
        custom_assert(a < b);
        std::cout << "OK" << std::endl;

    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

void testGreaterThan() {
    try {
        // Test 1
        LongNumber a = LongNumber::make_number(100.5,15);
        LongNumber b = LongNumber::make_number(50.25,15);

        std::cout << "Test Case: Greater Than" << std::endl;
        custom_assert(a > b);
        std::cout << "OK" << std::endl;


        a = LongNumber::make_number(0.0001,15);
        b = LongNumber::make_number(0.0000001,15);

        std::cout << "Test Case: Greater Than - Small numbers " << std::endl;
        custom_assert(a > b);
        std::cout << "OK" << std::endl;

      
        a = LongNumber::make_number(0.0,15);
        b = LongNumber::make_number(-10.1 ,15);

        std::cout << "Test Case: Greater Than - negative vs zero" << std::endl;
        custom_assert(a > b);
        std::cout << "OK" << std::endl;
        
        a = LongNumber::make_number(-9999,15);
        b = LongNumber::make_number(-10000.1,15);

        std::cout << "Test Case: Greater Than - negative numbers" << std::endl;
        custom_assert(a > b);
        std::cout << "OK" << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

int main() {
    testAddition(); //ok
    testSubtraction(); //ok
    testMultiplication(); //ok
    //testDivision();

    testEquality(); 
    testInequality();
    testLessThan();
    testGreaterThan();


    return 0;
}
