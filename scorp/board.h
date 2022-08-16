/**
 * The game board holds the status of the game. Changes to the board are all
 * made by this object but the board does not initiate those changes. Furthermore
 * the board makes no attempt to have any strategy or prioritization of moves,
 * this is left to an outside player.
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 9 - 602.202.82
 */
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include "card.h"
#include "deck.h"

typedef int move_result;
const move_result SUCCESS = 1;
const move_result FLIP = 1 << 2;
const move_result COMPLETE = 1 << 3;

class Board {
  private:
    /** 
     * single card for reporting missing cards
     */
    Card nilcard;

    /**
     * Cards are managed in an array for random access.
     */
    Card* mem[52];

    /** 
     * col_heads hold the head of each column on the board
     * (including the reserve: col 8). -1 indicates an 
     * empty column.
     */
    int col_heads[8] { -1, -1, -1, -1, -1, -1, -1, -1 };

    /**
     * a simple mutator for the col_heads array. Note this is
     * private and is not really meant to be used for anything 
     * but the ability to monitor the mutations.
     *
     * @param col the column to assign.
     * @param card the card to use.
     */
    void setColHead(int col, int card);

    /**
     * like col_heads, col_tails maintains the end of each
     * column. 
     */
    int col_tails[8] { -1, -1, -1, -1, -1, -1, -1, -1 };
    /**
     * similar to setColHead in function.
     */
    void setColTail(int col, int card);


    /**
     * a simple abstraction used in the construction of the board.
     */
    bool isopen_by_default(int i) { return !(((i % 7) < 3) && (i / 7) < 3); }
  public:
    /** 
     * single constructor for the board, requires a deck. The deck is already
     * shuffled and will be used as is to lay out the board.
     */
    Board(const Deck& deal); 

    /**
     * There is only meant to be one board at any time. Copy/Move constructors
     * are removed.
     */
    Board(const Board&) = delete;
    Board(const Board&&) = delete;
    /**
     * Cards are created in dynamic memory, we need to clean them up when
     * we are cleaned up.
     */
    ~Board() {
      for (int i=0; i<52; i++) {
        if (mem[i] != nullptr) delete mem[i];
      }
    }
    
    /**
     * The main workhorse of the board. move hides the complexity of moving a card
     * from the user, taking care of all the board changes that might result from
     * a card move. Note that there is no validation done as to wether a move is valid
     * from the gameplay perspective. 
     *
     * @param card - the card to move.
     * @param to_col - the destination for the card, will be placed at the tail of the column.
     * @return a move_result indicating the effects of the move. See the move_result typedef
     * for more information.
     */
    move_result move(const int card, const int to_col);

    /**
     * The state of the board can be printed at any time by calling
     * this method with an ostream.
     */
    void display(std::ostream &);

    /**
     * A simple abstraction for checking of the board contains a completed game. 
     * @return true if the game is complete (each king has 12 cards under it in order).
     */
    bool isCompleteGame();

    /**
     * The following four methods access the current state of the board. 
     */
    const Card* cardFor(int);
    const Card* reserve();
    const Card* colHead(int);
    const Card* colTail(int);

    /**
     * A convienence method for dealing the reserve as specified by the rules of the game.
     * After returning the reserve will be empty. If called with an empty reserve this
     * is a noop.
     */
    void dealReserve();
};

#endif
