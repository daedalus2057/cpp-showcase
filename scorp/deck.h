/**
 * Deck is a simple container for an array of cards. 
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 9 - 602.202.82
 */
#ifndef DECK_H
#define DECK_H

#include <stdexcept>

class Deck {
  const static size_t SIZE = 52;

  private:
    int deck[SIZE];

  public:
    Deck() noexcept;
    /**
     * Initialize a deck from a array.
     */
    Deck(int cards[52]);

    /**
     * Shuffle the deck in place.
     */
    void shuffle(); 

    /**
     * Allow accessing the deck like an array.
     *
     * @throw std::out_of_range if idx is not within limits [0, 51].
     */
    int operator[](std::size_t idx) const {
      if (idx < 0 || idx > SIZE-1) throw std::out_of_range("Invalid index");

      return deck[idx];
    }
};

#endif
