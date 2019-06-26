#ifndef CARD_HPP__
#define CARD_HPP__

#include <vector>
#include <set>
#include <string>

enum Face {
  FACE_ACE = 1,
  FACE_TWO,
  FACE_THREE,
  FACE_FOUR,
  FACE_FIVE,
  FACE_SIX,
  FACE_SEVEN,
  FACE_EIGHT,
  FACE_NINE,
  FACE_JACK,
  FACE_QUEEN,
  FACE_KING,
};

enum Suit {
  SUIT_CLUB = 1,
  SUIT_DIAMOND,
  SUIT_HEART,
  SUIT_SPADE
};


static char to_friendly_string(Face f) {
  if (f == FACE_ACE) return 'A';
  if (f == FACE_JACK) return 'J';
  if (f == FACE_QUEEN) return 'Q';
  if (f == FACE_KING) return 'K';
  if ((int)f >= (int)FACE_TWO && (int)f <= (int)FACE_NINE) {
    // Use char '0' aka 48 as base value + the face
    return (char)((int)'0' + (int)f);
  }
  return '?';
}

static char const* to_friendly_string(Suit s) {
  switch(s) {
    case SUIT_CLUB:     return "\xE2\x99\xA3";
    case SUIT_DIAMOND:  return "\xE2\x99\xA6";
    case SUIT_HEART:    return "\xE2\x99\xA5";
    case SUIT_SPADE:    return "\xE2\x99\xA0";
  }
  return "?";
}

class Card {
private:
  Face face_;
  Suit suit_;

public:
  Card(Face f, Suit s) : face_(f), suit_(s) {
  }

  std::string to_string() const {
    return std::string() + to_friendly_string(face_) + ' ' + to_friendly_string(suit_);
  }

  int value(bool ace11 = false) const {
    if (face_ == FACE_ACE) return ace11 ? 11 : 1;
    if ((int)face_ >= (int)FACE_TWO && (int)face_ <= (int)FACE_NINE) {
      return (int)face_;
    }
    if ((int)face_ >= (int)FACE_JACK && (int)face_ <= (int)FACE_KING) {
      return 10;
    }
    std::abort();
    return 0;
  }
};

#endif