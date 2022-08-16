/**
 * In data structure terms the Card is the Node in our data structure.
 * Maintaining two simultaneous data structures at once, Card 
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 9 - 602.202.82
 */
#ifndef CARD_H
#define CARD_H

#include <exception>
#include <string>
#include "scorp.h"

class Card {
  private:
    /**
     * maintaining the current column we are in enables performance optimization
     * during move consideration. This means it must be maintained during moves though.
     */
    int current_col = -1;
    /**
     * pos is this cards position in the board mem array
     */
    int pos = -1;
    bool open = true;

    /**
     * if size is > 1 then we are in an ordered list. This stores the
     * number of children we had before we are attached to a new order
     * head.
     */
    int size = 1;

    /**
     * The ordered list has a head and this property records it. 
     * If there is no order head then we might be the order head
     * or we might just be an unordered card.
     */
    int orderh = -1;

    // doublly linked list
    // for the column neighboors
    int cprev = -1;
    int cnext = -1;

  public:
    /**
     * Create a card in a position with a default open status
     * @throw std::invalid_argument if position is invalid.
     */
    Card(int pos, bool open) :pos{pos}, open{open} {
      if ((pos < 0) || (pos > 51)) throw std::invalid_argument(std::to_string(pos) + " is not a valid card position");
    };

    /**
     * A default constructor for rolling your own card.
     */
    Card() {}

    /**
     * For our inorder list we maintain the size of our ordered
     * children if we are the head of the order list. Note there
     * is no dec function, once a card is in order, it will never 
     * be able to move.
     */
    int inc() { return size++; }

    /**
     * Gives the status if this orderhead is a complete sequence.
     */
    bool iscomplete() { return size == 13; }

    /**
     * Utility for stringifying a card with respect to it's open status.
     */
    std::string as_string() const {
      if (!open) {
        return "__";
      }
      return intToStrC(pos);
    }

    /**
     * Get the position of this card in the board array
     */
    int position() const { return pos; }
    /** 
     * Determine if this card is open/flipped up
     */
    bool isopen() const { return open; }

    /**
     * flip this card over.
     */
    void openup() { open = true; }

    /**
     * append the given card to our column list.
     */
    void append(Card *c) {
      cnext = c->position();
      c->set_cprev(pos);
    }

    /**
     * remove our current next
     */
    void unpend() {
      cnext = -1;
    }

    /**
     * setter for cprev 
     */
    void set_cprev(int i) { cprev = i; }

    /**
     * accessors for our linked list pointers
     */
    int get_cprev() const { return cprev; }
    int get_cnext() const { return cnext; }

    /**
     * getter/setters for our private properties
     */
    int orderhead() const { return orderh; }
    void orderhead(int orderh) { this->orderh = orderh; }

    int col() const { return current_col; }
    void col(int cidx) { current_col = cidx; }
};

#endif
