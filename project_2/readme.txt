For differentiator:

./differentiator --eval "x * y" x=10 y=12

For symbolic derivatives:

./differentiator --diff "x * sin(x)" --by x

Testing the Library:

make clean
make
make run_tests

TESTING: 

BASICS ARITHMETICS: 

# Addition
./differentiator --eval "5 + 3"           # Expected: 8
./differentiator --eval "x + 3" x=5       # Expected: 8

# Subtraction
./differentiator --eval "10 - 4"          # Expected: 6
./differentiator --eval "y - 2" y=5       # Expected: 3

# Multiplication
./differentiator --eval "6 * 7"           # Expected: 42
./differentiator --eval "x * y" x=6 y=7   # Expected: 42

# Division
./differentiator --eval "15 / 3"          # Expected: 5
./differentiator --eval "x / 2" x=10      # Expected: 5

# Exponentiation
./differentiator --eval "2 ^ 8"           # Expected: 256
./differentiator --eval "x ^ 3" x=5       # Expected: 125

FUNCTION TEST:

# Sine function

./differentiator --eval "sin(0)"                                     # Expected: 0
./differentiator --eval "sin(pi/2)" pi=3.14159265358979323846        # Expected: ~1.0
./differentiator --eval "sin(x)" x=1.5708                            # Expected: ~1.0 (pi/2 ≈ 1.5708)

# Cosine function
./differentiator --eval "cos(0)"                                     # Expected: 1
./differentiator --eval "cos(pi)" pi=3.14159265358979323846          # Expected: ~-1.0
./differentiator --eval "cos(x)" x=3.1416                            # Expected: ~-1.0

# Natural logarithm
./differentiator --eval "ln(1)"                                      # Expected: 0
./differentiator --eval "ln(e)" e=2.71828182845904523536             # Expected: ~1.0
./differentiator --eval "ln(x)" x=20.0855                            # Expected: ~3.0 (e^3 ≈ 20.0855)

# Exponential function
./differentiator --eval "exp(0)"                                     # Expected: 1
./differentiator --eval "exp(1)"                                     # Expected: ~2.71828
./differentiator --eval "exp(x)" x=2                                 # Expected: ~7.38906 (e^2)

COMBINED OPERATION TEST:
./differentiator --eval "(2 + 3) * 4"                                # Expected: 20
./differentiator --eval "sin(x^2)" x=1.253                           # Expected: 1
./differentiator --eval "exp(x + y)" x=1 y=1                         # Expected: ~7.38906 
./differentiator --eval "sin(cos(x))" x=0                            # Expected: ~0.8415 (sin(1))
./differentiator --eval "ln(exp(x))" x=5                             # Expected: 5

EDGE CASES: 
./differentiator --eval "1/0" 
# Undefined logarithms
./differentiator --eval "ln(0)"                                     # Expected: Error
./differentiator --eval "ln(-1)"                                    # Expected: Error
./differentiator --eval "x*y + y*z + z*x" x=1 y=2 z=3               # Expected: 11
./differentiator --eval "sin(x) + cos(y)" x=1.5708 y=0              # Expected: ~1.0 + 1.0 = 2.0
./differentiator --eval "()"                                        # Expected: Error

CCOMPLEX EXPRESSIONS;
./differentiator --eval "exp(sin(x) + ln(y)) - z^2" x=1.5708 y=2.71828 z=2  # Expected: 3.389
./differentiator --eval "sin(x^2 + y^2)/(x*y)" x=1 y=1              #0.9093


BRACKET NESTING: 
./differentiator --eval "(1 + (2 * 3))"                         # Expected: 7
./differentiator --eval "((1 + 2) * 3)"                         # Expected: 9
./differentiator --eval "(((5)))"                               # Expected: 5
./differentiator --eval "(x + (y * z))" x=1 y=2 z=3             # Expected: 7
./differentiator --eval "((a + b) * (c - d))" a=5 b=3 c=4 d=1   # Expected: 24
./differentiator --eval "-(-5)"                                 # Expected: 5
./differentiator --eval "-(-(-5))"                              # Expected: -5


./differentiator --diff "x^2" --by x
./differentiator --diff "x^3 + 2*x" --by x
./differentiator --diff "sin(x) + cos(x)" --by x
./differentiator --diff "ln(x)" --by x
./differentiator --diff "5" --by x             # Expected: 0
./differentiator --diff "x^3" --by x           # Expected: 3*x^2
./differentiator --diff "x^0" --by x           # Expected: 0

TRIGONOMETRIC FUNCTIONS:

./differentiator --diff "sin(x)" --by x        # Expected: cos(x)
./differentiator --diff "cos(x)" --by x        # Expected: -sin(x)
./differentiator --diff "-sin(x)" --by x       # Expected: -cos(x)
./differentiator --diff "-cos(x)" --by x       # Expected: sin(x)
./differentiator --diff "sin(x^2)" --by x      # Expected: cos(x^2)*2*x
./differentiator --diff "cos(sin(x))" --by x   # Expected: -sin(sin(x))*cos(x)
./differentiator --diff "ln(x)" --by x         # Expected: 1/x
./differentiator --diff "ln(x^3)" --by x       # Expected: (3x^2)/ (x^3)

./differentiator --diff "exp(x)" --by x        # Expected: exp(x)
./differentiator --diff "exp(2*x)" --by x      # Expected: 2*exp(2*x)
./differentiator --diff "exp(-x)" --by x       # Expected: -exp(-x)
./differentiator --diff "sin(2*x + 1)" --by x  # Expected: 2*cos(2*x + 1)
./differentiator --diff "x*y" --by x           # Expected: y
./differentiator --diff "(x + 1)^(x + 2)" --by x #Expected: (x + 1)^(x + 2)*(1/(x+1) + ln(x+1))

