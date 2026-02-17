#include "Pack.hpp"
#include <cassert>

using namespace std;

Pack::Pack() : next(0) {
  int idx = 0;
  for (int s = SPADES; s <= DIAMONDS; ++s) {
    Suit suit = static_cast<Suit>(s);
    for (int r = NINE; r <= ACE; ++r) {
      Rank rank = static_cast<Rank>(r);
      cards[idx] = Card(rank, suit);
      ++idx;
    }
  }
}

Pack::Pack(istream &pack_input) : next(0) {
  for (int i = 0; i < PACK_SIZE; ++i) {
    pack_input >> cards[i];
  }
}

Card Pack::deal_one() {
  assert(next < PACK_SIZE);
  return cards[next++];
}

void Pack::reset() {
  next = 0;
}

void Pack::shuffle() {
  for (int j = 0; j < 7; ++j) {
    array<Card, PACK_SIZE> temp;
    for (int i = 0; i < PACK_SIZE / 2; ++i) {
      temp[2 * i] = cards[PACK_SIZE / 2 + i]; 
      temp[2 * i + 1] = cards[i];     
    }
    cards = temp;
  }
  next = 0;
}

bool Pack::empty() const {
  return next >= PACK_SIZE;
}
