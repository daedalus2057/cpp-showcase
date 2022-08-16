/**
 * Implementation for board.h
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 9 - 602.202.82
 */
#include "board.h"
#include "scorp.h"
#include <iostream>

Board::Board(const Deck& deal) {

  for (int i=0; i<49; i++) {
    int current_col = i % 7;
    int current_head = col_heads[current_col];
    int current_tail = col_tails[current_col];

    mem[deal[i]] = new Card(deal[i], isopen_by_default(i));

    auto c = mem[deal[i]];
    move(c->position(), current_col);
  }

  // the last three are the reserve;
  for (int i=49; i < 52; i++) {
    mem[deal[i]] = new Card(deal[i], false); 
    move(deal[i], 7);
  }
}

const Card* Board::reserve() { 
  if (col_heads[7] != -1) {
    return mem[col_heads[7]]; 
  }

  return &nilcard;
}


const Card* Board::cardFor(int idx) { 
  if (idx < 0 || idx > 51) return &nilcard;
  return mem[idx]; 
}

const Card* Board::colHead(int idx) { 
  if (col_heads[idx] == -1) return &nilcard;

  return mem[col_heads[idx]]; 
}

const Card* Board::colTail(int idx) { 
  if (col_tails[idx] == -1) return &nilcard;
  return mem[col_tails[idx]]; 
}

void Board::setColTail(int col, int tail) {
  col_tails[col] = tail;
}

void Board::setColHead(int col, int head) {
  col_heads[col] = head;
}

void Board::display(std::ostream& outs) {
  if (col_heads[7] != -1) {
    auto rc = mem[col_heads[7]]->position();
    outs << "Reserve: ";
    while (rc != -1) {
      outs << mem[rc]->as_string() << " > ";
      rc = mem[rc]->get_cnext();
    }

    outs << "X" << std::endl;
  }

  for (int i=0; i < 7; i++) {
    outs << std::endl << i << ": ";
    auto c = colHead(i);
    if (c->position() == -1) {
      outs << "X";
      continue;
    }

    outs << c->as_string() << " > ";
    int limit = 0;
    while ((c->get_cnext() != -1) && (limit < 52)) {
      c = cardFor(c->get_cnext());
      outs << c->as_string() << " > ";
      limit++;
    }

    outs << "X"; 
  }


  if (isCompleteGame()) {
    outs << "\n\n******GAME COMPLETED*******\n\n";
  }
}

void Board::dealReserve() {
  if (col_heads[7] == -1)  return; 

  for (int i=0; i < 3 && col_tails[7] != -1; i++) {
    mem[col_tails[7]]->openup();
    move(col_tails[7], i);
  }
}

move_result Board::move(const int cardIdx, const int to_col) {
  Card *c = mem[cardIdx];

  move_result result = 0;

  // save the col_tail we are
  int card_tail_for_moving_card = (c->col() != -1) ? col_tails[c->col()] : c->position();
  // save our original column
  int original_card_col = c->col();

  // if we are last in the column, empty the column head/tail
  if ((c->col() != -1) && (c->get_cprev() == -1)) {
    setColHead(c->col(), -1);
    setColTail(c->col(), -1);
  }

  // otherwise remove this card from our parent's cnext
  // if the parent is closed, open it
  // and make our parent the new tail
  if ((c->col() != -1) && (c->get_cprev() != -1)) {
    auto parent = mem[c->get_cprev()];
    parent->unpend();

    if (!parent->isopen()) {
      parent->openup();
      result |= FLIP;
    }

    setColTail(c->col(), parent->position());

    c->set_cprev(-1);
  }


  // for us and all our children -- reset col
  c->col(to_col);
  int child_cnext = c->get_cnext();

  while (child_cnext != -1) {
    auto tmp = mem[child_cnext];
    tmp->col(to_col);
    child_cnext = tmp->get_cnext();
  }

  // begin the actual move
  if (col_heads[to_col] == -1) { // moving to an empty column
    setColHead(to_col, c->position());
    setColTail(to_col, card_tail_for_moving_card);
  } else { 
    if ( // check if this card is the next card in the sequence... 
        (mem[col_tails[to_col]]->position() == c->position()+1) &&  
        // unless the previous card was an ace and then it's a false positive
        (((mem[col_tails[to_col]]->position())%13) != 0)) {  

      int orderhead;
      // if the previous does not have an orderhead then it is the only
      // card in the sequence and therefore we don't want to elide this card
      if (mem[col_tails[to_col]]->orderhead() != -1) { // elide!

        // we want to take the current tail's orderhead and 
        // make it our orderhead as well as setting the orderhead
        // next to our node. Unless orderhead is -1 and then we 
        // do nothing but append

        // get the current orderhead
        orderhead = mem[col_tails[to_col]]->orderhead();
      } else {
        // the previous node in the sequence is the first in the sequence
        // this means that we should not do any of the skipping
        orderhead = mem[col_tails[to_col]]->position();
      }

      auto oh = mem[orderhead];
      // the orderhead sequence has increased in size by one
      oh->inc();

      // set the next/ordertail of the orderhead to our node
      //oh->ordertail(c->position());
      // not only are we the tail but the tail is the next in
      // our orderhead column sequence (skipping anything between)
      //oh->append(c);

      // make the orderhead our orderhead
      c->orderhead(orderhead); 
      // and make our sequence use the new orderhead
      int ni = c->get_cnext();
      int nv = c->position()-1;
      while (ni != -1 && nv % 13 != 0) {
        if (mem[ni]->position() != nv) {
          break;
        }
        
        oh->inc();
        mem[ni]->orderhead(orderhead);
        ni = mem[ni]->get_cnext();
        nv = nv - 1;
      }

      if (oh->iscomplete()) {
        result |= COMPLETE;
      }
    }

    mem[col_tails[to_col]]->append(c);
    setColTail(to_col, card_tail_for_moving_card);

    result |= SUCCESS;
  }

  return result;
}

bool Board::isCompleteGame() {
  // check for a completed game...
  bool complete = true;
  for (int i=0; i<4; i++) {
    auto k = mem[(i*13)+12];
    if (!k->iscomplete()) {
      complete = false;
      break;
    }
  }

  return complete;
}
