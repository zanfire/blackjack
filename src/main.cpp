// Base headers
#include <stdio.h>
#include <locale.h>
#include <iostream>

#include "blackjack.hpp"

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

  std::cout << "Blackjack game" << std::endl;
  // This is the game.
  Blackjack bj;
  // Loop on the menu
  while (true) {
    std::cout << "Do you want start a " << underline << "N" << normal << "ew round? " << underline << "E" << normal << "xit or " << underline << "S" << normal << "how leaderboard?" << std::endl;
    char hint = ask_hint();
    if (hint == 'n') bj.play();
    else if (hint == 's') bj.dump_leaderboard();
    else if (hint == 'e') break;
  }
  return EXIT_SUCCESS;
}

