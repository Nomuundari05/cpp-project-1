#include "../long_number.hpp"
#include <iostream>
#include <cmath>
#include <chrono>

static int g_totalTests = 0;
static int g_failedTests = 0;

static void checkTest(bool condition, const std::string& name) {
    g_totalTests++;
    if (condition) {
        std::cout << name << ": OK\n";
    } else {
        g_failedTests++;
        std::cout << name << ": FAIL\n";
    }
}

static void testConstructors() {
    LongNumber a(0);
    checkTest(a.toString() == "0", "Constructor 0");

    LongNumber b(123);
    checkTest(b.toLongLong() == 123, "Constructor 123");

    LongNumber c(-999999);
    checkTest(c.toLongLong() == -999999, "Constructor -999999");

    LongNumber d("456.78");
    checkTest(std::fabs(d.toDouble() - 456.78) < 0.001, "Constructor from string 456.78");

    LongNumber e("0001.100");
    checkTest(e.toString() == "1.1", "Constructor from padded string 0001.100");

    LongNumber f("-0.5");
    checkTest(f.toString() == "-0.5", "Constructor from negative fraction -0.5");
}

static void testCopyAndAssign() {
    LongNumber original("123.456");

    LongNumber copy(original);
    checkTest(copy.toString() == "123.456", "Copy constructor");

    LongNumber assigned = original;
    checkTest(assigned.toString() == "123.456", "Assignment operator");
}

static void testLiterals() {
    LongNumber lit = "123.456"_longnum;

    std::cout << "[DEBUG] Expected: 123.456" << std::endl;
    std::cout << "[DEBUG] Actual  : " << lit.toString() << std::endl;

    checkTest(lit.toString() == "123.456", "operator\"\"_longnum test");
}

static void testAddSub() {
    LongNumber x("10");
    LongNumber y("3");
    checkTest((x + y).toString() == "13", "10 + 3 => 13");
    checkTest((x - y).toString() == "7", "10 - 3 => 7");
    checkTest((LongNumber("0") - LongNumber("10")).toString() == "-10", "0 - 10 => -10");
    checkTest((LongNumber("999999") + LongNumber("1")).toString() == "1000000", "999999 + 1 => 1000000");
}

static void testMulDiv() {
    checkTest((LongNumber("7") * LongNumber("8")).toString() == "56", "7 * 8 => 56");
    checkTest((LongNumber("100") / LongNumber("4")).toString() == "25", "100 / 4 => 25");
    checkTest((LongNumber("-2") * LongNumber("3")).toString() == "-6", "-2 * 3 => -6");
    checkTest((LongNumber("0.5") / LongNumber("0.25")).toString() == "2", "0.5 / 0.25 => 2");
    checkTest((LongNumber("1") / LongNumber("3")).toString().substr(0, 10) == "0.333333", "1 / 3 => 0.333333...");

    bool caught = false;
    try {
        LongNumber z = LongNumber("7") / LongNumber("0");
    } catch(...) {
        caught = true;
    }
    checkTest(caught, "divide by zero => exception");
}

static void testComparison() {
    checkTest(LongNumber("10") == LongNumber("10"), "10 == 10");
    checkTest(LongNumber("10") != LongNumber("12"), "10 != 12");
    checkTest(LongNumber("10") < LongNumber("12"), "10 < 12");
    checkTest(LongNumber("12") > LongNumber("10"), "12 > 10");
    checkTest(LongNumber("-1") < LongNumber("1"), "-1 < 1");
}

static void testPrecision() {
    LongNumber::setPrecision(10);
    checkTest(LongNumber::getPrecision() == 10, "setPrecision(10)");

    bool caught = false;
    try {
        LongNumber::setPrecision(9999999);
    } catch(...) {
        caught = true;
    }
    checkTest(caught, "exceed maxPrecision => exception");
}

static void testLargeNumbers() {
    checkTest((LongNumber("999999999999999999999999999") + LongNumber("1")).toString() == "1000000000000000000000000000", "Large number addition");
    checkTest((LongNumber("1000000000000") * LongNumber("1000000000000")).toString() == "1000000000000000000000000", "Large number multiplication");
    checkTest((LongNumber("1000000000000000000000") / LongNumber("10")).toString() == "100000000000000000000", "Large number division");
}

 LongNumber calculatePi(int precision) {
    LongNumber pi("0");
    LongNumber four("4");

    for (int k = 0; k < precision; ++k) {
        LongNumber term = four / LongNumber(2 * k + 1);
        pi = (k % 2 == 0) ? (pi + term) : (pi - term);
    }

    return pi;
}

static void testPiCalculation() {

    LongNumber::setPrecision(200);

    auto start = std::chrono::high_resolution_clock::now();

    LongNumber pi = chudnovskyPi(15);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken for Pi(100 digits): " << elapsed.count() << " seconds\n";

    checkTest(elapsed.count() <= 1.0, "Pi calculation under 1 sec");

    checkTest(pi.toString().substr(0, 5) == "3.141", "Pi calculation test");
}

int runAllTests() {
    g_totalTests = 0;
    g_failedTests = 0;

    std::cout << "==== Running  ====\n";
    testConstructors();
    testCopyAndAssign();
    testLiterals();
    testAddSub();
    testMulDiv();
    testComparison();
    testPrecision();
    testLargeNumbers();
    testPiCalculation();

    std::cout << "==== Tests Done ====\n";
    std::cout << "Total tests: " << g_totalTests << "\n";
    std::cout << "Failed tests: " << g_failedTests << "\n";

    return (g_failedTests == 0) ? 0 : 1;
}