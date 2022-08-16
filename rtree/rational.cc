/*
 * rational implementation. See header file for more information.
 */
#include "rational.h"
#include <numeric>
#include <cmath>

/*
 *  Constructor
 *
 *  Sets up the rational number by ensuring a nonzero denominator,
 *  making only the numerator signed., and storing reduced values.
 *
 *  PARAMETERS: 
 *    numer (type int) - value of the numerator of a rational number
 *    denom (type int) - value of the denominator of a rational number
 *  POSTCONDITIONS:
 *    a new Rational object has been constructed and initialized with
 *    instance variable values that meet class invariant conditions.
 */
rational::rational (int nu, int de) {
  // Ensure no div by 0 and avoid exceptions
  if (de == 0) de = 1;

  // Make numerator "store" sign
  // If numer was negative then overall
  // sign is positive, hence the below 
  // makes sense.
  if (de < 0) {
    nu = -nu;
    de = -de;
  }

  // Save our normalized data
  numer = nu; denom = de;

  // Reduce the values
  reduce();
}

rational::rational(const char* in) {
  // we expect two integers separated by a '/'
  // we will collect our char in a char buf
  char buf[11];
  int i = 0; 
  int j = 0;

  int nu = INT_MIN;
  int de = 1;

  while (in[i] != '\0') {
    if (in[i] == 0x2f) {
      buf[j] = '\0';
      nu = atoi(buf);
      j = 0;
      i++;
      continue;
    }

    buf[j++] = in[i++];
  }

  buf[j] = '\0';
  if (strlen(buf) > 0) {
    if (nu == INT_MIN) {
      nu = atoi(buf);
    } else {
      de = atoi(buf);
    }
  }

  numer = nu; denom = de;
}

/*
 *  add
 *
 *  Adds this rational number to the one passed as a parameter.
 *  A common denominator is found by multiplying the individual
 *  denominators.
 *
 *  PARAMETERS:
 *    op2 (type ref. to Rational object) - number to be added to this one
 *  RETURNS: ref. to a new Rational object that has a numerator and
 *    denominator (in reduced form) representing the sum of this object
 *    plus op2.
 *  POSTCONDITIONS:
 *    A new Rational object has been constructed and a reference to
 *    it has been returned.
 *    (No change to either object's instance variables.)
 */
rational rational::operator+(const rational& op) const {
  int c_denom = denom * op.denominator();
  int num_a = numer * op.denominator();
  int num_b = op.numerator() * denom;
  
  // move it: https://stackoverflow.com/a/4986802 
  return rational { (num_a + num_b), c_denom };
}

/*
 *  subtract
 *
 *  Subtracts the rational number passed as a parameter from this
 *  rational number.
 *
 *  PARAMETERS:
 *    op2 (type ref. to Rational object) - number to be subtracted from
 *        this one
 *  RETURNS: ref. to a new Rational object that has a numerator and
 *    denominator (in reduced form) representing the difference of
 *    this object minus op2.
 *  POSTCONDITIONS:
 *    A new Rational object has been constructed and a reference to
 *    it has been returned.
 *    (No change to either object's instance variables.)
 */
rational rational::operator-(const rational& op) const {

  int c_denom = denom * op.denominator();
  int num_a = numer * op.denominator();
  int num_b = op.numerator() * denom;

  return rational { (num_a - num_b), c_denom };
}

/*
 *  multiply
 *
 *  Multiplies this rational number by the one passed as a
 *  parameter.
 *
 *  PARAMETERS:
 *    op2 (type ref. to Rational object) - number to be multiplied times
 *        this one
 *  RETURNS: ref. to a new Rational object that has a numerator and
 *    denominator (in reduced form) representing the product of
 *    this object and op2.
 *  POSTCONDITIONS:
 *    A new Rational object has been constructed and a reference to
 *    it has been returned.
 *
 *    (No change to either object's instance variables.)
 */
rational rational::operator*(const rational &op) const {
  return rational { numer * op.numerator(), denom * op.denominator() };
}

/*
 *  reciprocal
 *
 *  Returns the reciprocal of this rational number.
 *
 *  PARAMETERS: none
 *  RETURNS: ref. to a new Rational object that has a numerator and
 *    denominator (in reduced form) representing the reciprocal of
 *    this object.
 *  POSTCONDITIONS:
 *    A new Rational object has been constructed and a reference to
 *    it has been returned.
 *    (No change to the object's instance variables.)
 */
rational rational::reciprocal() const {
  return rational { denom, numer };
}

/*
 *  divide
 *
 *  Divides this rational number by the one passed as a parameter
 *  by multiplying by the reciprocal of the second rational.
 *
 *  PARAMETERS:
 *    op2 (type ref. to Rational object) - number to be divided into
 *        this one
 *  RETURNS: ref. to a new Rational object that has a numerator and
 *    denominator (in reduced form) representing the quotient of
 *    this object and op2.
 *  POSTCONDITIONS:
 *    A new Rational object has been constructed and a reference to
 *    it has been returned.
 *    (No change to either object's instance variables.)
 */
rational rational::operator/(const rational &op) const {
  return (*this) * op.reciprocal();
}

/*
 *  expon
 *
 *  Raises this rational number by the one passed as a
 *  parameter. Number passed must be an "integer"
 *
 *  PARAMETERS:
 *    op2 (type ref. to Rational object) - number to be multiplied times
 *        this one
 *  RETURNS: ref. to a new Rational object that has a numerator and
 *    denominator (in reduced form) representing the product of
 *    this object and op2.
 *  POSTCONDITIONS:
 *    A new Rational object has been constructed and a reference to
 *    it has been returned.
 *    (No change to either object's instance variables.)
 */
rational rational::operator^(const rational& op2) const {
  double realExp = op2.numerator() / (double)op2.denominator();
  return rational { (int)std::pow(numer, realExp), (int)std::pow(denom, realExp) };
}

/*
 * See operator^(const rational&) doc
 *
 * Convienence operator for raising to int powers.
 */
rational rational::operator^(const int& op2) const {
  return rational { (int)std::pow(numer, op2), (int)std::pow(denom, op2) };
}

/*
 *  reduce
 *
 *  Reduces this rational number by dividing both the numerator
 *  and the denominator by their greatest common divisor.
 *
 *  PARAMETERS: none
 *  PRECONDITIONS: denominator > 0
 *  POSTCONDITIONS:
 *    If numerator is not zero, numerator and denominator values have
 *    been replaced by their reduced form values.
 */
void rational::reduce () {
  int common;

  if (numer != 0) {
    common = std::gcd(numer, denom);

    numer /= common;
    denom /= common;
  }
}

