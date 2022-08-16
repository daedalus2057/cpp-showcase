/**
 * Scorp is an application that runs simulated games of the solitaire game
 * scorpion. See the usage for details on how to run the appliction.
 *
 * @author Donovan Nye <donovan.nye@gmail.com>
 * @module 9 - 602.202.82
 */
#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>

#include "board.h"
#include "player.h"
#include "scorp.h"

/**
 * usage provides the user with a user friendly description of how to use the application.
 * @param name - the name of the executable.
 */
void usage(const char* name) {
  std::cout << "\n\nUsage: " << name << " -f <filename> [ -o <filename> ] [ -n <count> ] [ -c ]\n\n";
  std::cout << "This program requires a single argument which is the name of the file containing the starting deal order to use for the game. An optional second argument [-o] can be supplied to output to a named file.\n\nThe data file should be formatted with nothing but 52 \"card values\" formated such as:";
  
  std::cout << R"(

AH 3C ... 4D KS

)";

  std::cout << "The first value should be in the set {A,2-9,10,J,Q,K} and the second value should come from {C,D,H,S} for Clubs, Diamonds, Hearts and Spades respectively. If there are not exactly 52 card values on a single line or there are invalid values the program will exit with an error.\n\nFinally, a parameter -n can be supplied with an integer to indicate the number of times the game should be played beyond the initial deal. The cards will be reshuffled on every round. By default the input deal will be played and then the game will exit.\n\n Enjoy.\n\n" << std::endl;

  std::cout << "Finally, a special mode is activated when passing a -c which disables all other modes of running.\nIn this mode a search will be performed for a complete game stopping at 10,000 games. A percentage of complete games will be displayed and nothing else. This mode is used for tuneing the game heuistics.\n\n";
}

/**
 * Runs a fixed number of simulated games and records the number of complete games
 * providing a metric for possible improvement of the game play heuristics.
 */
int metrics() {

  int complete_games = 0;
  try {
    Deck deck;
    for (int i=0; i < 100'000; i++) {
      deck.shuffle();
      Board *b = new Board(deck);
      Player *p = new Player(b);
      while (p->hasNextMove()) 
        p->makeMove();

      if (b->isCompleteGame()) complete_games++;
      delete p;
      delete b;
    }
  } catch (std::exception &e) {
    std::cerr << "EXCEPTION: " << e.what();
    return -1;
  }

  if (complete_games == 0) {
    std::cout << "There were no complete games in 100,000 tries\n\n.";
  } else {
    std::cout << "There were " << complete_games << " out of 100,000 games, for a completion rate of " << 100*((double)complete_games/100'000) << "%.\n\n";
  }

  return 0;
}

/**
 * A simple run wrapper for the normal operation of the application.
 *
 * @param data - the initial data to run with.
 * @param outs - an output stream to use for printing results.
 * @param repeat - the number of times to run the simulation.
 *
 * @return 0, -1 for success, failure
 */
int run(std::istream &data, std::ostream &outs, int repeat) {
  try {
    auto deck = parseInput(data);
    for (int i=0; i<repeat; i++) {
      Board* b = new Board(deck);
      outs << std::endl;

      outs << "Beginning game with board:" << std::endl;
      b->display(outs);

      outs << "\n\nMoves:\n";

      Player* player = new Player(b);
      while (player->hasNextMove()) {
        outs << player->makeMove() << std::endl;
      }

      outs << "\nNo more moves to make\n\n";

      outs << "Ended with board:" << std::endl;
      b->display(outs);

      outs << "\n\n";

      delete player;
      delete b;

      deck.shuffle();
    }
  } catch( std::exception& e ) {
    outs << std::endl << "EXCEPTION: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}

/**
 * main entrypoint for the application. Handles the commandline argument parsing and the launching of
 * the application. In particular it manages the opening and closing of files.
 */
int main(int argc, char** argv) {
  
  if (argc < 2) {
    usage(argv[0]);
    return 1;
  }

  const char* fname;
  const char* out_fname = nullptr;
  int repeat = 0;

  bool isInt = true;

  for (int i=1; i < argc; i++) {
    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-f", 2) == 0) {
      if (i+1 >= argc) {
        std::cout << "Error: The argument [-f] requires a parameter <filename>" << std::endl;
        return 1;
      }
      i = i + 1;
      fname = argv[i];
      continue;
    }
     
    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-h", 2) == 0) {
      usage(argv[0]);
      return 0;
    }

    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-c", 2) == 0) {
      return metrics();
    }

    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-o", 2) == 0) {
      if (i+1 >= argc) {
        std::cout << "Error: The argument [-o] requires a parameter <filename>" << std::endl;
        return 1;
      }
      i = i + 1;
      out_fname = argv[i];
      continue;
    }

    if ((strlen(argv[i]) == 2) && strncmp(argv[i], "-n", 2) == 0) {
      if (i+1 >= argc) {
        std::cout << "Error: The argument [-n] requires a parameter <count>" << std::endl;
        return 1;
      }
      i = i + 1;
      repeat = atoi(argv[i]);
      if ((repeat < 0) || (repeat > 256)) {
        std::cout << "Error: The argument to -n ( " << repeat << ") is invalid. <count> should be > 0 and < 256." << std::endl;
        return 1;
      }
      continue;
    }

    std::cout << "Error: Unknown argument [" << argv[i] << "]" << std::endl;
    usage(argv[0]);
    return 1;
  }

  // is this a readable file?
  std::ifstream data(fname);

  if (!data.is_open()) {
    std::cout << "The file [ " << fname << " ] could not be opened for reading." << std::endl;
    return 1;
  }

  // are we writing to a file?
  if (out_fname != nullptr) {
    std::ofstream outs(out_fname, std::ofstream::trunc);
    if (!outs.is_open()) {
      std::cout << "The file [ " << out_fname << " ] could not be opened for writing." << std::endl;
      return 1;
    }

    int ret = run(data, outs, repeat);

    // clean up
    outs.close();
    data.close();

    return ret;
  }

  // default case
  return run(data, std::cout, 1+repeat);
}
