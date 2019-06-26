// STL
#include <stdio.h>
#include <locale.h>
#include <iostream>

#include "blackjack.hpp"

/**
 * @brief Helper function that ask for an hint.
 * @return char hint (lowercase).
 */
static char ask_hint() {
  char c = getchar();
  // flush the stdin.
  char ch;
  while ((ch = getchar()) != '\n' && ch != EOF) continue;
  return tolower(c);
}

int main(int argc, char **argv) {
  // Set UTF-8 for unicode symbols.
  setlocale(LC_ALL, "en_US.UTF-8");

  std::cout << std::endl << " \xF0\x9F\x94\xA5  Blackjack game \xF0\x9F\x94\xA5" << std::endl << std::endl;
  // Create the main game.
  Blackjack bj;
  // Loop on the menu options.
  while (true) {
    std::cout << "Do you want start a " << underline << "N" << normal << "ew round? " << underline << "E" << normal << "xit or " << underline << "S" << normal << "how leaderboard? ";
    char hint = ask_hint();
    // take action from the hint.
    if (hint == 'n') bj.play();
    else if (hint == 's') bj.dump_leaderboard();
    else if (hint == 'e') break;
  }
  return EXIT_SUCCESS;
}
