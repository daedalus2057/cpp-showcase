/**
 * Implementation of player.h.
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 9 - 602.202.82
 */
#include "player.h"
#include <iostream>
#include <climits>

Player::Player(Board *board) {
	this->board = board;
	collectMoves();
}

int Player::weightForMove(int card, int col) {
  // simple heuristic: prefer bigger numbers and smaller cols
  return ((double)card / col) * 100;
}

void Player::collectMoves() {
  for (int cidx=0; cidx < 7; cidx++) {
    const Card *ct = board->colTail(cidx);
    int ctp = ct->position();
    
    if ((ctp != -1) && (ctp % 13 != 0)) {
      const Card *nextCard = board->cardFor(ctp - 1);
      if (nextCard->isopen() && (nextCard->col() != cidx)) {
        // is this our best move yet?
        int w = weightForMove(nextCard->position(), cidx);
        if (w > bestMove[0]) {
          bestMove[0] = w;
          bestMove[1] = nextCard->position();
          bestMove[2] = cidx;
        }
      }
    }

    if (ctp == -1) { // we have an empty col
      // any kings can be moved here
      for (int k=0; k<4; k++) {
        const Card *nextCard = board->cardFor((k * 13)+12);
        if (nextCard->isopen() && 
            (nextCard->col() != cidx) && 
            (nextCard->get_cprev() != -1)) {
          
          // is this our best move yet?
          int w = weightForMove(nextCard->position(), cidx);
          if (w > bestMove[0]) {
            bestMove[0] = w;
            bestMove[1] = nextCard->position();
            bestMove[2] = cidx;
          }
        }
      }
    }
  }
}

bool Player::hasNextMove() {
  return bestMove[1] != -1;
}

std::string Player::makeMove() {
  if (!hasNextMove()) 
    throw std::invalid_argument("no moves to make");

  int original_col = board->cardFor(bestMove[1])->col();

  move_result result = board->move(bestMove[1], bestMove[2]);

  std::string r = board->cardFor(bestMove[1])->as_string() +
    " moved to column " + std::to_string(bestMove[2]);

  if ((result & FLIP) == FLIP) {
    r += "\nFlipped over a card in column " + std::to_string(original_col);
  }

  if ((result & COMPLETE) == COMPLETE) {
    r += "\nSequence completed.";
  }

  // reset best move
  bestMove[0] = INT_MIN;
  bestMove[1] = -1;
  bestMove[2] = -1;

  // get the next move
  collectMoves();

  // if we have no move and we have reserve
  // dump reserve and try again...
  if ((bestMove[1] == -1) && (board->reserve()->position() != -1)) {
    board->dealReserve();
    r += "\nReserve dealt.";
    collectMoves();
  }

  return r;
}
