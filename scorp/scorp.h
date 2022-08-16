/**
 * Scorp is a simple wrapper for global utilities in the application.
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 9 - 602.202.82
 */
#ifndef SCORP_H
#define SCORP_H

#include <random>
#include <memory>
#include <stdexcept>
#include "deck.h"
#include <string>

/**
 * strCtoInt takes a c-string in the form [2-9,10,AJQK][CDHS] and 
 * returns the int for the card where each index is the value of 
 * the card (0-52) and for each index i is evaluated as follows. 
 *
 * (i % 13) gives the value of the card: 0 for ace and 12 for king. 
 * floor(i / 13) gives the suit where:
 * 0 = clubs, 1 = Diamonds, 2 = Hearts, 3 = Spades
 *
 * @param str - a 2 or 3 character string matching to form above.
 * @return the int conversion as specified above.
 */
int strCtoInt(const char* str); 

/**
 * intToStrC is the reverse operation of strCtoInt.
 *
 * @param c - the card value to stringify
 * @return string representation of the card.
 */
std::string intToStrC(int c);

/**
 * parseInput is the middleground between main and the class heirarchy.
 * The input stream is parsed according to the contract specified in the
 * application requirements. 
 *
 * @param istream - the data to try and parse
 * @throw std::invalid_argument - thrown if the input stream does not match
 * the expected input.
 * @return a Deck that can be used to create the game board.
 */
const Deck parseInput(std::istream &);

#endif
