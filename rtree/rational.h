#ifndef RATIONAL_H
#define RATIONAL_H

#include <iostream>

/*
 * Ported from rational.java by Lewis and Loftus, Lewis E. Hitchner
 * and John E. Boon, Jr.
 * http://www.csc.calpoly.edu/~hitchner/CSC101.F2002/RationalNumbers.html#RationalNumbers
 *
 * Ported by Donovan Nye
 * 3/2022
 */

class rational {
 /*  Instance variables
  *
  *  Class invariants are:
  *    denom != 0
  *    denom is non-negative (but numerator may be negative)
  *    numer and denom are in reduced form
  *       i.e., there exists no integer n such that,
  *       numer = n * denom or denom = n * numer
  */
  int numer, denom;

  public:
    inline rational(): numer(0), denom(1) { };
    rational(int nu, int de);
    rational(const char* in);

    // C++ operator overloading to make rational
    // behave like a numeric value.
    // See .cc file for method documentation.
    rational operator+(const rational&) const;
    rational operator-(const rational& op2) const;
    rational operator*(const rational& op2) const;
    rational operator/(const rational& op2) const;
    rational operator^(const rational& op2) const;
    rational operator^(const int& op2) const;

    rational reciprocal() const;
    inline int numerator() const { return numer; }
    inline int denominator() const { return denom; }
    inline double to_real() const { return numer / (double)denom; }
    inline const std::string to_string(bool with_n = false, bool with_paren = false) const {
      std::string s;
      if (denom == 1) {
        s.append(std::to_string(numer));
        if (with_n) {
          s.append("n");
        }
        return s;
      }

      std::string pretty_numer = (with_n) ? 
        (numer == 1) ? "n" : std::to_string(numer) + "n" 
        : std::to_string(numer);
            
      if (with_paren) s.append("(");
      s.append(pretty_numer).append("/").append(std::to_string(denom));
      if (with_paren) s.append(")");
      return s;
    }

    inline std::string open_paren() const { return (denominator() == 1) ? "" : "("; }
    inline std::string close_paren() const { return (denominator() == 1) ? "" : ")"; }

    inline bool operator==(const rational& rhs) const {
      return (numer == rhs.numerator()) && (denom == rhs.denominator());
    }

    inline std::ostream& operator<< (std::ostream& ost) const {
      if (numer == 0) return ost << "0";
      else if (denom == 1) return ost << numer;
      else return ost << numer << "/" << denom;
    }

  private:
    void reduce();
    // Prefer C++ <numeric> gcd
    //int gcd(const int a, const int b) const;
};

#endif
