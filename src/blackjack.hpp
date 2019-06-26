#ifndef BLACKJACK_HPP__
#define BLACKJACK_HPP__

// STL
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <thread>
// Blackjack
#include "constants.hpp"
#include "card.hpp"
#include "hand.hpp"
#include "deck.hpp"

/**
 * @brief Ask user gor hit or stand.
 * 
 * @return true 
 * @return false 
 */
static bool ask_hit_or_stand() {
  while(true) {
    std::cout << underline << "H" << normal << "it/" << underline << "S" << normal << "tand? ";

    char c = getchar();
    // flush the stdin.
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF) continue;
    if (c == 'h' || c == 'H') {
      return true;
    }
    if (c == 's' || c == 'S') {
      return false;
    }
  }
  return false;
}

enum PlayResult {
  PLAY_RESULT_TIE = 0,
  PLAY_RESULT_DEALER_WIN,
  PLAY_RESULT_PLAYER_WIN,
};

char const* to_friendly_string(PlayResult r) {
  if (r == PLAY_RESULT_TIE) return "tie";
  if (r == PLAY_RESULT_DEALER_WIN) return "dealer win";
  if (r == PLAY_RESULT_PLAYER_WIN) return "player win";
  return "?";
}

/**
 * @brief Class that provide blackjack logic.
 * 
 */
class Blackjack {
private:
  // my deck.
  Deck deck_;
  // Counter of rounds.
  int rounds_ = 0;
  // counters of results for players and dealers.
  unsigned long result_counters_[3] = {0, 0, 0};
  // default wait between each action.
  int sleep_between_actions_ms_ = 500; // 500 ms.
public:
  Blackjack() {
    deck_.shuffle();
  }

  /**
   * @brief Play.
   * 
   * @return true if player win.
   * @return false if dealer win.
   */
  void play() {
    rounds_ += 1;
    std::cout << std::endl << "Playing round " << rounds_ << " ..." << std::endl;
    if ((rounds_ % 6) == 0) {
      std::cout << " " << action_mark << " reshuffle ..." << std::endl;
      wait();
      deck_ = Deck();
      deck_.shuffle();
    }
    Hand dealer("dealer");
    Hand player("player one");

    // give player a card.
    Card c = deck_.draw();
    std::cout << " " << action_mark << " player draw " << c.to_string() << " ..." << std::endl;
    if (c.invalid()) return;
    player.add(c);
    wait();
    // give dealer a card
    c = deck_.draw();
    std::cout << " " << action_mark << " dealer draw " << c.to_string() << " ..." << std::endl;
    if (c.invalid()) return;
    dealer.add(c);
    wait();
    // Give player a card.
    c = deck_.draw();
    std::cout << " " << action_mark << " player draw " << c.to_string() << " ..." << std::endl;
    if (c.invalid()) return;
    player.add(c);
    wait();

    // Show player their hand.
    player.dump();
    // Show dealer their hand.
    dealer.dump();

    // Player turn.
    while (ask_hit_or_stand()) {
      // hit
      c = deck_.draw();
      std::cout << " " << action_mark << " player draw " << c.to_string() << " ..." << std::endl;
      if (c.invalid()) return;
      player.add(c);
      wait();
      player.dump();
    
      if (player.is_bust()) {
        // Player loss.
        record_result(PLAY_RESULT_DEALER_WIN);
        return;
      }
    }
    // Dealer turn.
    while (!dealer.is_soft_17()) {
      c = deck_.draw();
      std::cout << " " << action_mark << " dealer draw " << c.to_string() << std::endl;
      if (c.invalid()) return;
      dealer.add(c);
      wait();
      dealer.dump();
    }

    if (dealer.is_bust()) {
      // dealer is over 21, loss.
      record_result(PLAY_RESULT_PLAYER_WIN);
      return;
    }
  
    wait();
    if (dealer.best_value() == player.best_value()) {
      record_result(PLAY_RESULT_TIE);
    }
    else if (dealer.best_value() > player.best_value()) {
      record_result(PLAY_RESULT_DEALER_WIN);
    }
    else {
      record_result(PLAY_RESULT_PLAYER_WIN);
    }
  }

  /**
   * @brief Dump on the stdout the leaderboard.
   */
  void dump_leaderboard() {
    dump_leaderboard_line("player", PLAY_RESULT_PLAYER_WIN);
    dump_leaderboard_line("dealer", PLAY_RESULT_DEALER_WIN);
    dump_leaderboard_line("tie", PLAY_RESULT_TIE);
  }

private:
  /**
   * @brief Dump one line of the leaderboard.
   * 
   * @param name 
   * @param res 
   */
  void dump_leaderboard_line(char const* name, PlayResult res) {
    std::streamsize ss = std::cout.precision();
    std::cout.precision(2);
    std::cout << " \xE2\x9C\x8F " <<  std::setw(10) << name << ":\t" << 
                                      std::setw(10) << result_counters_[res] << "\t";
    if (rounds_ == 0) {
      std::cout << std::setw(10) << "0%" << std::endl;
    }
    else {
      std::cout << std::setw(10) << std::fixed << (float)result_counters_[res] / rounds_ * 100.0f  << "%" << std::endl;
    }
    std::cout.precision(ss);
  }

  /**
   * @brief Record and dump result on the output.
   * 
   * @param res 
   * @return PlayResult 
   */
  PlayResult record_result(PlayResult res) {
    result_counters_[(int)res] += 1;

    std::cout << "Round result: ";
    if (res == PLAY_RESULT_PLAYER_WIN) std::cout << bold_green;
    else if (res == PLAY_RESULT_DEALER_WIN) std::cout << bold_red;
    else if (res == PLAY_RESULT_TIE) std::cout << bold_yellow;
    std::cout << to_friendly_string(res) << normal << std::endl << std::endl;

    return res;
  }
  /**
   * @brief sleep for a pre-define amount for not rushing the game.
   */
  void wait() {
    if (sleep_between_actions_ms_ > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(sleep_between_actions_ms_));
    }
  }
};

#endif