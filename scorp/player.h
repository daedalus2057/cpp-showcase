/**
 * Player is responsible for playing the game. It maintains all the 
 * strategy and exposes an interface for stepping through optimized 
 * moves.
 * 
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 9 - 602.202.82
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <limits>
#include "board.h"

class Player {
  private:
    /**
     * The board we are playing on.
     */
    Board *board;

    /**
     * A simple array containing our best move, index 0 is the weight of the move
     * index 1 is the card to move and index 2 is the column to move the card to.
     */
    int bestMove[3] = { 
      std::numeric_limits<int>::min(), -1, -1 
    };

    /**
     * This is the only function that should need to be edited when optimizing 
     * gameplay. The heuristic follows the rule that bigger weights are considered
     * the best.
     */
    int weightForMove(int, int);

    /**
     * Given a board state the player assess the available moves on the board and
     * picks the best move according to it's weight and saves it as the next 
     * move to make.
     */
    void collectMoves();

  public:
    Player(Board *board); 
    Player(Player &) = delete;
    Player(Player &&) = delete;

    /** 
     * for using the player like an iterator and iterating over
     * moves.
     */
    bool hasNextMove();

    /**
     * To move the game forward the player makesMoves. The board is changed
     * and if there are no moves and there is a reserve the reserve is dealt 
     * in search of a move to make.
     */
    std::string makeMove();
};


#endif
