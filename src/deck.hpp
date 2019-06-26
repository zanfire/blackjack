#ifndef DECK_HPP__
#define DECK_HPP__

// STL
#include <vector>
#include <algorithm>
#include <random>
// Blackjack
#include "card.hpp"

/**
 * @brief Class that model a deck.
 * 
 * At the beggining it is an ordered deck.
 */
class Deck {
private:
  std::vector<Card> deck_;

public:
  Deck() {
    // Create the initial deck.
    for (int i = SUIT_CLUB; i <= (int)SUIT_SPADE; i++) {
      for (int y = FACE_ACE; y <= (int)FACE_KING; y++) {
        deck_.push_back(Card((Face)y, (Suit)i));
      }
    }
  }

  /**
   * @brief Randmoly shuffle deck.
   */
  void shuffle() {
    static std::random_device rd;
    static std::mt19937 g(rd());
    
    std::shuffle(deck_.begin(), deck_.end(), g);
  }

  /**
   * @brief Draw one card from the deck.
   * 
   * @return Card 
   */
  Card draw() {
    if (!deck_.empty()) {
      Card c = deck_.back();
      deck_.pop_back();
      return c;
    }
    // This supposed to not happen but in theory can happen. 
    std::abort();
    return Card(true); // create a special card that is invalid.
  }
};

#endif