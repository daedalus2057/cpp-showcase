/**
 * Implementation for deck.h
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 9 - 602.202.82
 */
#include <random>
#include "deck.h"

Deck::Deck() noexcept {
  for (int i=0; i < 52; i++) {
    deck[i] = i;
  }
}

Deck::Deck(int cards[52]) {
  for (int i=0; i < 52; i++) {
    deck[i] = cards[i];
  }
}

void Deck::shuffle() {
  // https://stackoverflow.com/a/12658061
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<int> gen(0, 51);

  int a, b, c;
  for (int i=0; i<100; i++) {
    a = gen(eng);
    b = gen(eng);
    c = deck[a];
    deck[a] = deck[b];
    deck[b] = c;
  }
}
