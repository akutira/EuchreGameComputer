// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <array>
#include <sstream>
#include "Pack.h"
#include "Card.h"

using namespace std;

Pack::Pack() {
for (int s = 0; s < 4; ++s) {
    Suit suit = static_cast<Suit>(s);
    for (int r = 0; r < 6; ++r) {
      Rank rank = static_cast<Rank>(r + 7);
       cards[s * 6 + r] = Card(rank, suit);
  
    }
  }
  next = 0;
}

Pack::Pack(std::istream& pack_input) {
  for (int s = 0; s < 4; ++s) {
    for (int r = 0; r < 6; ++r) {
      Suit suit = static_cast<Suit>(s);
      Rank rank = static_cast<Rank>(r + 7);
      pack_input >> cards[s * 6 + r];
      cards[s * 6 + r] = Card(rank, suit);
    }
  }
  next = 0;
}

Card Pack::deal_one() {
  if (next >= 24) {
    next = 0;
  }
  Card deltCard = cards[next];
  ++next;
  return deltCard;
}

void Pack::reset() {
  next = 0;
}

void Pack::shuffle() {
  for (int i = 0; i < 7; i++) {
    Card halfdeck1[PACK_SIZE];
    Card halfdeck2[PACK_SIZE];
    for (int k = 0; k < 12; k++) {
      halfdeck1[k] = cards[k];
    }
    for (int k = PACK_SIZE/2; k < PACK_SIZE; k++) {
      halfdeck2[k - (PACK_SIZE/2)] = cards[k];
    }
    for (int k = 0; k < PACK_SIZE; k++){
      if (k % 2 == 0) {
        cards[k] = halfdeck2[k / 2];
      } else {
        cards[k] = halfdeck1[k / 2];
      }
    }
  }
  next = 0;
}

bool Pack::empty() const {
  if (next == PACK_SIZE) {
    return true;
  }
  else {
    return false;
  }
}