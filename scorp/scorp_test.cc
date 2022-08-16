#include <chrono>
#include <gtest/gtest.h>
#include <initializer_list>
#include <sstream>
#include <iostream>
#include <chrono>

#include "scorp.h"
#include "board.h"

/**
 * The tests for the scorp application. A simple set of tests for the
 * functionality expected. Uses googletest.
 * 
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 9 - 602.202.82
 */
TEST(ScorpTest, ShuffleDeck) {
  Deck adeck; 

  for (int i=0; i < 52; i++) {
    EXPECT_EQ(i, adeck[i]);
  }

  adeck.shuffle();

  int matches = 0;
  for (int i=0; i < 52; i++)  if (adeck[i] == i) matches++; 

  EXPECT_LT(matches, 10);
}

TEST(ScorpTest, strCToInt) {
  EXPECT_EQ(strCtoInt("AC"), 0);
  EXPECT_EQ(strCtoInt("2C"), 1);
  
  std::stringstream required_input { "KD 3H 4C 8S 6S 6D JH 3C 2D KS AS " \
                         "9D 5S 8C QS 10D 4H KC 10C 9H 5D 7H " \
                         "7C QD 2H QC AH JS 10H 7S 9C 5H 2S " \
                         "QH JD AC 3S KH 4D 6C 6H 10S 3D 4S 7D " \
                         "2C 9S AD 5C 8D JC 8H" };

  EXPECT_NO_THROW({
    std::string card;
    while (required_input >> card) {
      strCtoInt(card.c_str());
    }
  });
}

TEST(ScorpTest, intToStrC) {
   EXPECT_EQ(intToStrC(0), "AC");
   EXPECT_EQ(intToStrC(1), "2C");

  std::stringstream required_input { "KD 3H 4C 8S 6S 6D JH 3C 2D KS AS " \
                         "9D 5S 8C QS 10D 4H KC 10C 9H 5D 7H " \
                         "7C QD 2H QC AH JS 10H 7S 9C 5H 2S " \
                         "QH JD AC 3S KH 4D 6C 6H 10S 3D 4S 7D " \
                         "2C 9S AD 5C 8D JC 8H" };

  try {
    auto deck = parseInput(required_input);

    std::string card;
    int i = 0;
    while (required_input >> card) {
      EXPECT_EQ(intToStrC(deck[i]), card);
    }
  } catch (std::exception& e) {
    FAIL() << e.what();
  }
}

TEST(ScorpTest, parseInput) {
  
  std::stringstream required_input { "KD 3H 4C 8S 6S 6D JH 3C 2D KS AS " \
                         "9D 5S 8C QS 10D 4H KC 10C 9H 5D 7H " \
                         "7C QD 2H QC AH JS 10H 7S 9C 5H 2S " \
                         "QH JD AC 3S KH 4D 6C 6H 10S 3D 4S 7D " \
                         "2C 9S AD 5C 8D JC 8H" };

  int vals[52];
  for (int i=0; i<52; i++) vals[i] = -1;

  try {
    auto deck = parseInput(required_input);
    for (int i=0; i < 52; i++) {
      EXPECT_GE(deck[i], 0);
      EXPECT_LT(deck[i], 52);
      vals[deck[i]] = 1;
    }
  } catch (const std::exception& e) {
    FAIL() << e.what();
  } 

  for (int i=0; i<52; i++) EXPECT_EQ(vals[i], 1);
}

TEST(ScorpTest, ConstructBoard) {
  std::stringstream required_input { "KD 3H 4C 8S 6S 6D JH 3C 2D KS AS " \
                         "9D 5S 8C QS 10D 4H KC 10C 9H 5D 7H " \
                         "7C QD 2H QC AH JS 10H 7S 9C 5H 2S " \
                         "QH JD AC 3S KH 4D 6C 6H 10S 3D 4S 7D " \
                         "2C 9S AD 5C 8D JC 8H" };

  try {
    auto deck = parseInput(required_input);

    Board b {deck};

    auto rc = b.reserve();
    int reserve_size = 1;
    while (rc->get_cnext() != -1) {
      reserve_size++;
      rc = b.cardFor(rc->get_cnext());
    }

    EXPECT_EQ(reserve_size, 3);

    for (int i=0; i < 7; i++) {
      auto c = b.colHead(i);
      EXPECT_NE(c->position(), -1);
      int col_size = 1;
      while (c->get_cnext() != -1) {
        col_size++;
        c = b.cardFor(c->get_cnext());
      }

      EXPECT_EQ(col_size, 7);
    }
  } catch (std::exception &e) {
    FAIL() << e.what();
  }
}

TEST(ScorpTest, DealReserve) {

  std::stringstream required_input { "KD 3H 4C 8S 6S 6D JH 3C 2D KS AS " \
                         "9D 5S 8C QS 10D 4H KC 10C 9H 5D 7H " \
                         "7C QD 2H QC AH JS 10H 7S 9C 5H 2S " \
                         "QH JD AC 3S KH 4D 6C 6H 10S 3D 4S 7D " \
                         "2C 9S AD 5C 8D JC 8H" };

  try {
    auto deck = parseInput(required_input);

    Board b {deck};

    b.dealReserve();

    auto rc = b.reserve();

    EXPECT_EQ(rc->position(), -1);
  } catch (std::exception &e) {
    FAIL() << e.what();
  }
}
