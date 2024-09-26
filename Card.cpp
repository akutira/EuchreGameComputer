// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <array>
#include "Card.h"

using namespace std;

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  is >> str;
  rank = string_to_rank(str);
  return is;
}



/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  is >> str;
  suit = string_to_suit(str);
  return is;
}


/////////////// Write your implementation for Card below ///////////////

Card::Card() : rank(TWO), suit(SPADES) {}

Card::Card(Rank rank_in, Suit suit_in) : rank(rank_in), suit(suit_in) {}

Rank Card::get_rank() const { return rank; }

Suit Card::get_suit() const { return suit; }

Suit Card::get_suit(Suit trump) const {
  if (rank == JACK && suit == Suit_next(trump)) { 
  return trump;
  }
  return suit;
}

bool Card::is_face_or_ace() const {
  return rank >= JACK || rank == ACE;
}

bool Card::is_right_bower(Suit trump) const {
  return suit == trump && rank == JACK;
}

bool Card::is_left_bower(Suit trump) const {
  Suit next_suit = Suit_next(trump);
  return rank == JACK && suit == next_suit;
}

bool Card::is_trump(Suit trump) const {
  return suit == trump || is_left_bower(trump);
}

std::ostream & operator<<(std::ostream &os, const Card &card) {
  os << RANK_NAMES[card.get_rank()] << " of " << SUIT_NAMES
  [static_cast<int>(card.get_suit())];
  return os;
}

std::istream & operator>>(std::istream &is, Card &card) {
  std::string rank_name;
  is >> rank_name;
  std::string of;
  is >> of;
  std::string suit_name;
  is >> suit_name;

  // Find the rank
  bool rank_found = false;
  for (int i = 0; i < static_cast<int>(Rank::ACE) + 1; i++) {
    if (rank_name == RANK_NAMES[i]) {
      card.rank = static_cast<Rank>(i);
      rank_found = true;
      break;
    }
  }
  if (!rank_found) {
    return is;
  }

  // Find the suit
  if (suit_name == "Spades") {
    card.suit = Suit::SPADES;
  } else if (suit_name == "Hearts") {
    card.suit = Suit::HEARTS;
  } else if (suit_name == "Diamonds") {
    card.suit = Suit::DIAMONDS;
  } else if (suit_name == "Clubs") {
    card.suit = Suit::CLUBS;
  } else {
    return is;
  }

  return is;
}


bool operator<(const Card &lhs, const Card &rhs) {
  if (lhs.get_rank() != rhs.get_rank()) {
    return lhs.get_rank() < rhs.get_rank();
  }
  return lhs.get_suit() < rhs.get_suit();
}

bool operator<=(const Card &lhs, const Card &rhs) {
  return (lhs < rhs) || (lhs == rhs);
}

bool operator>(const Card &lhs, const Card &rhs) {
  if (lhs.get_rank() != rhs.get_rank()) {
    return lhs.get_rank() > rhs.get_rank();
  }
  return lhs.get_suit() > rhs.get_suit();
}

bool operator>=(const Card &lhs, const Card &rhs) {
  return (lhs > rhs) || (lhs == rhs);
}

bool operator==(const Card &lhs, const Card &rhs) {
  if (lhs.get_suit() == rhs.get_suit()) {
    return (lhs.get_rank() == rhs.get_rank());
  }
  return false; 
}

bool operator!=(const Card &lhs, const Card &rhs) {
  return !(lhs == rhs);
}

Suit Suit_next(Suit suit) {
  if (suit == Suit::SPADES)
    return Suit::CLUBS;
  else if (suit == Suit::HEARTS)
    return Suit::DIAMONDS;
  else if (suit == Suit::DIAMONDS)
    return Suit::HEARTS;
  else if (suit == Suit::CLUBS)
    return Suit::SPADES;
  else
    return Suit::SPADES;
}

bool Card_less(const Card &a, const Card &b, Suit trump) {
  if (a == b) {
    return false;
  }
  if (a.is_trump(trump) && b.is_trump(trump)) {
    if(a.is_right_bower(trump)) {
      return false;
    } else if (b.is_right_bower(trump)) {
      return true;
    } else if (a.is_left_bower(trump)) {
      return false;
    } else if (b.is_left_bower(trump)) {
      return true;
    } else {
      return a < b;
    }
  } 
  if (!a.is_trump(trump) && b.is_trump(trump)) {
      return true;
  } else if (a.is_trump(trump) && !b.is_trump(trump)) {
      return false;
  } else {
    return a < b;
  }
}

bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump) {
  if (a == b) {
    return false;
  }
  Card lc = led_card;
  if (led_card.is_left_bower(trump)) {
      lc = Card(JACK, trump);
  }
  if (a.is_trump(trump) && b.is_trump(trump)) {
    if (a.is_right_bower(trump)) {
      return false;
    } else if (b.is_right_bower(trump)) { 
      return true; 
    } else if (a.is_left_bower(trump)) {
      return false;
    } else if (b.is_left_bower(trump)) {
      return true;
    } else { 
      return a < b;
    } 
  } 
  if (a.is_trump(trump) && !b.is_trump(trump)) {
    return false; 
  }
  else if (!a.is_trump(trump) && b.is_trump(trump)) {
    return true;
  }
  else if (b.get_suit() == lc.get_suit() && a.get_suit() != lc.get_suit()) {
    return true;
  }
  else if (b.get_suit() != lc.get_suit() && a.get_suit() == lc.get_suit()) {
    return false;
  } 
  return  a < b; 
} 

// NOTE: We HIGHLY recommend you check out the operator overloading
// tutorial in the project spec before implementing
// the following operator overload functions:
//   operator<<
//   operator>>
//   operator<
//   operator<=
//   operator>
//   operator>=
//   operator==
//   operator!=
