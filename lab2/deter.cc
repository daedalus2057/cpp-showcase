#include <functional>
#include <iostream>

#include "deter.h"

/**
 * Implementation for the isNumeric function.
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 7 - 602.202.82
 */
bool deter::isNumeric(const char &c) {
  return ((c != 0x2f) && (c > 0x2c && c < 0x3a));
}

