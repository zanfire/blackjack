#ifndef HAND_HPP__
#define HAND_HPP__

// STL
#include <vector>
#include <algorithm>
#include <random>
// Blackjack
#include "card.hpp"

class Hand {
private:
  char const* name_;
  std::vector<Card> hand_;

public:
  Hand(char const* name) : name_(name) {
  }

  void add(Card const& c) {
    hand_.push_back(c);
  }

  bool bust() {
    int sum = std::accumulate(hand_.begin(), hand_.end(), 0,
                              [](int const& v, Card const& c) { return v + c.value(false); });
    return sum > 21;
  }

  bool is_soft_17() {
    int sum = std::accumulate(hand_.begin(), hand_.end(), 0,
                              [](int const& v, Card const& c) { return v + c.value(false); });
    return sum >= 17;
  }

  /** 
   * Compute the value nearest to 21. (ACE is 1 or 11)
   */
  int best_value() {
    int low = value(false);
    int high = value(true);
    // No difference so no problem.
    if (low == high) return low;
    // If high is busted use low.
    if (high > 21) return low;
    return high;
  }

  /**
   * @brief Compute total value
   * 
   * @param ace11 if count the ACE as 11 instead of 1.
   * @return int total value
   */
  int value(bool ace11 = false) {
    return std::accumulate(hand_.begin(), hand_.end(), 0, [ace11](int const& v, Card const& c) { return v + c.value(ace11); });
  }

  /**
   * @brief Dump on stdout deck.
   * 
   */
  void dump() {
    std::string line;
    for (auto& c : hand_) {
      line += c.to_string() + ", ";
    }
    if (!line.empty()) line.erase(line.size()-2);
    
    std::cout << "Hand " << name_ << ": " << line << " (";
    int l = value(false);
    int h = value(true);
    std::cout << l;
    if (l != h) {
      std::cout << "/" << h;
    } 
    std::cout << ")" << std::endl;
  }
};

#endif