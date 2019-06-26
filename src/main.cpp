// Base headers
#include <stdio.h>
#include <locale.h>
#include <iostream>

#include "blackjack.hpp"

int main(int argc, char **argv) {

  // Set UTF-8 for unicode symbols.
  setlocale(LC_ALL, "en_US.UTF-8");

  std::cout << "Blackjack game" << std::endl;
  // This is the game.
  Blackjack bj;
  // Loop on the menu
  while (true) {
    
    std::cout << "Do you want start a new round? exit or show leaderboard?" << std::endl; 
    PlayResult res = bj.play();
    bj.dump_leaderboard();
  }
  return EXIT_SUCCESS;
}

