/**
 * Implementation of scorp.h.
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 9 - 602.202.82
 */
#include <iostream>
#include <stdlib.h>
#include <stdexcept>
#include <cstring>
#include "scorp.h"

std::string intToStrC(int c) {
  if ((c < 0) || (c > 51)) {
    throw std::invalid_argument("Invalid card value");
  }

  std::string card;

  switch (c % 13) {
    case 0:
      card.append("A");
      break;
    case 10:
      card.append("J");
      break;
    case 11:
      card.append("Q");
      break;
    case 12:
      card.append("K");
      break;
    default:
      card.append(std::to_string((c % 13) + 1));
  }

  switch (c / 13) {
    case 0:
      card.append("C");
      break;
    case 1:
      card.append("D");
      break;
    case 2:
      card.append("H");
      break;
    case 3:
      card.append("S");
      break;
    default:
      throw std::invalid_argument("Invalid card value");
  }

  return card;
}

int strCtoInt(const char* str) {
  int len = strlen(str);
  if ((len > 3) || (len < 2)) {
    throw std::invalid_argument("Expected a string of len 2 or 3" + std::to_string((int)*str));
  }

  int base = -1;
  int suit = -1;

  switch (str[len-1]) {
    case 'C':
      suit = 0;
      break;
    case 'D':
      suit = 1;
      break;
    case 'H':
      suit = 2;
      break;
    case 'S':
      suit = 3;
      break;
    default:
      throw std::invalid_argument(std::to_string(str[len-1]) + " is an invalid string (suit).");
       
  }

  if (len == 3) {
    // must be a 10
    if ((str[0] != '1') || (str[1] != '0')) {
      throw std::invalid_argument(std::string(str) + " is an invalid string (length).");
    }

    base = 9;
  }

  if (len == 2) {
    switch (str[0]) {
      case 'A':
        base = 0;
        break;
      case 'J':
        base = 10;
        break;
      case 'Q':
        base = 11;
        break;
      case 'K':
        base = 12;
        break;
      default:
        if ((str[0] < 0x32) || (str[0] > 0x39))
          throw std::invalid_argument(std::string(str) + " is an invalid string.");
        base = str[0] - 0x31;
    }
  }


  return (13 * suit) + base;
}

const Deck parseInput(std::istream &ins) {

  int cards[52] , vals[52];
  for (int i=0; i < 52; i++) vals[i] = cards[i] = -1;

  char tmp;
  char buf[4];
  buf[3] = buf[2] = buf[1] = buf[0] = '\0'; 

  int j = 0;

  for(int i=0; ins; i++) {
    tmp = ins.get();

    if (tmp == -1) {
      break;
    }

    switch (tmp) {
      case ' ':
      case 'A':
      case 'K':

      case 'Q':
      case 'J':
      case 'H':
      case 'C':
      case 'D':
      case 'S':
        break;
      default:
        if ((tmp < 0x30) || (tmp > 0x39))
          throw std::invalid_argument("Invalid character in input stream: " + std::to_string((int)tmp));
    }

    if (tmp == ' ') {
      cards[j] = strCtoInt(buf);
      vals[cards[j]] = 1;
      j++;
      i = -1;
      buf[3] = buf[2] = buf[1] = buf[0] = '\0'; 
      continue;
    }


    buf[i] = tmp;
  }

  cards[j] = strCtoInt(buf);
  vals[cards[j]] = 1;

  // make sure we have 52 cards and that there are no duplicates
  for (int i=0; i < 52; i++) {
    if ((vals[i] == -1) || (cards[i] == -1)) 
      throw std::invalid_argument("pos: " + std::string(std::to_string(i)) + ": There were either missing or duplicate cards in the input" + std::string(std::to_string(vals[i])) + " " + std::string(std::to_string(cards[i])));
  }

  Deck d(cards);

  return d;
}
