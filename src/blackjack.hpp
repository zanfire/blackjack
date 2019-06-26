#ifndef BLACKJACK_HPP__
#define BLACKJACK_HPP__

// STL
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <thread>
//

#include <wchar.h>

// Blackjack
#include "card.hpp"
#include "hand.hpp"
#include "deck.hpp"

constexpr char underline[] = {0x1b,'[','4',';','3','9','m',0};
constexpr char bold_green[] = {0x1b,'[','1',';','3','2','m',0};
constexpr char bold_red[] = {0x1b,'[','1',';','3','1','m',0};
constexpr char bold_yellow[] = {0x1b,'[','1',';','3','3','m',0};
constexpr char normal[] = {0x1b,'[','0',';','3','9','m',0};

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
  Deck deck_;
  int rounds_ = 0;
  unsigned long result_counters_[3] = {0, 0, 0};
  int sleep_between_actions_ms_ = 700;
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
  PlayResult play() {
    rounds_ += 1;
    std::cout << std::endl << "Playing round " << rounds_ << " ..." << std::endl;
    if ((rounds_ % 6) == 0) {
      std::cout << " \xE2\x8C\x9B reshuffle ..." << std::endl;
      wait();
      deck_ = Deck();
      deck_.shuffle();
    }
    Hand dealer("dealer");
    Hand player("player one");

    // Give players two cards.
    Card c = deck_.draw();
    std::cout << " \xE2\x8C\x9B player draw " << c.to_string() << " ..." << std::endl;
    player.add(c);
    wait();
    c = deck_.draw();
    std::cout << " \xE2\x8C\x9B player draw " << c.to_string() << " ..." << std::endl;
    player.add(c);
    wait();

    // Show user their hand.
    player.dump();

    // if players have bust dealer WIN??
    if (player.bust()) { // cannot happen 10 + 10 == 20.
      return record_result(PLAY_RESULT_DEALER_WIN);
    }
    // give dealer a hand
    c = deck_.draw();
    std::cout << " \xE2\x8C\x9B dealer draw " << c.to_string() << " ..." << std::endl;
    dealer.add(c);
    wait();
    // Europe use only one card, american use two card, one covered.
    //p.add(deck_.draw());
    dealer.dump();

    // Main loop.

    while (ask_hit_or_stand()) {
      // hit
      c = deck_.draw();
      std::cout << " \xE2\x8C\x9B player draw " << c.to_string() << " ..." << std::endl;
      player.add(c);
      wait();
      player.dump();
    
      if (player.bust()) {
        // Player loss.
        return record_result(PLAY_RESULT_DEALER_WIN);
      }
    }

    while (!dealer.is_soft_17()) {
      c = deck_.draw();
      std::cout << " \xE2\x8C\x9B dealer draw " << c.to_string() << std::endl;
      dealer.add(c);
      wait();
      dealer.dump();
    }

    if (dealer.bust()) {
      // dealer is over 21, loss.
      return record_result(PLAY_RESULT_PLAYER_WIN);
    }
  
    wait();
    if (dealer.best_value() == player.best_value()) {
      // Tie.
      return record_result(PLAY_RESULT_TIE);
    }
    if (dealer.best_value() > player.best_value()) {
      return record_result(PLAY_RESULT_DEALER_WIN);
    }
    else {
      return record_result(PLAY_RESULT_PLAYER_WIN);
    }
  }

  void dump_leaderboard() {
    dump_leaderboard_line("player", PLAY_RESULT_PLAYER_WIN);
    dump_leaderboard_line("dealer", PLAY_RESULT_DEALER_WIN);
    dump_leaderboard_line("tie", PLAY_RESULT_TIE);
  }

private:
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