#include <cassert>
#include <iostream>
#include <array>
#include "Card.hpp"

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
//EFFECTS returns the Rank corresponding to str, for example "Two" -> TWO
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
  return {};
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//REQUIRES If any input is read, it must be a valid rank
//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  if(is >> str) {
    rank = string_to_rank(str);
  }
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
//EFFECTS returns the Suit corresponding to str, for example "Clubs" -> CLUBS
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
  return {};
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//REQUIRES If any input is read, it must be a valid suit
//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  if (is >> str) {
    suit = string_to_suit(str);
  }
  return is;
}


/////////////// Write your implementation for Card below ///////////////


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


// Initialize to 2 of spades
Card::Card(){
  rank = TWO;
  suit = SPADES;
}

// Initialize specifically
Card::Card(Rank rank_in, Suit suit_in){
  rank = rank_in;
  suit = suit_in;
}

// Get rank
Rank Card::get_rank() const{
  return rank;
}

// Get suit 
Suit Card::get_suit() const{
  return suit;
}

// Get suit
Suit Card::get_suit(Suit trump) const{
  // Check if left bower is trump
  if (is_left_bower(trump)){
    return trump;
  }
  return suit;
}

// Check if card is a face card
bool Card::is_face_or_ace() const{
  return (rank==JACK || rank==QUEEN || rank==KING || rank==ACE);
}

// Check if card is jack of trump suit
bool Card::is_right_bower(Suit trump) const{
  return (rank==JACK && suit==trump);
}

// Check if card is jack of next suit
bool Card::is_left_bower(Suit trump) const{
  return (rank==JACK && suit==Suit_next(trump));
}

// Check if card is trump
bool Card::is_trump(Suit trump) const{
  return (suit==trump || is_left_bower(trump));
}


// Operator Overloading

// Print card to stream
std::ostream& operator<<(std::ostream &os, const Card &card){
  os << card.get_rank() << " of " << card.get_suit();
  return os;
}

// Read card from stream
std::istream& operator>>(std::istream &is, Card &card){
  Rank r;
  string of;
  Suit s;
  is >> r >> of >> s;
  card = Card(r, s);
  return is;
}

// lhs < rhs
bool operator<(const Card &lhs, const Card &rhs) {
  if (lhs.get_rank() != rhs.get_rank()) {
    return lhs.get_rank() < rhs.get_rank();
  }
  return lhs.get_suit() < rhs.get_suit();
}

// lhs <= rhs
bool operator<=(const Card &lhs, const Card &rhs){
  return (lhs < rhs || lhs == rhs);
}

// lhs > rhs
bool operator>(const Card &lhs, const Card &rhs){
  return !(lhs <= rhs);
}

// lhs >= rhs
bool operator>=(const Card &lhs, const Card &rhs){
  return !(lhs < rhs);
}

// lhs = rhs
bool operator==(const Card &lhs, const Card &rhs){
  return (lhs.get_rank() == rhs.get_rank() && lhs.get_suit() == rhs.get_suit());
}

// lhs != rhs
bool operator!=(const Card &lhs, const Card &rhs){
  return !(lhs == rhs);
}


// Next suit
Suit Suit_next(Suit suit){
  if (suit == HEARTS){
    return DIAMONDS;
  }
  else if (suit == DIAMONDS){
    return HEARTS;
  }
  else if (suit == SPADES){
    return CLUBS;
  }
  else{
    return SPADES;
  }
}

// a < b
bool Card_less(const Card &a, const Card &b, Suit trump) {
  // Checking equality and bowers
  if (a == b) {
    return false;
  }

  // Right bower 
  if (a.is_right_bower(trump)) {
    return false;
  }
  else if (b.is_right_bower(trump)) {
    return true;
  }

  // Left bower 
  if (a.is_left_bower(trump)) {
    return false;
  }
  else if (b.is_left_bower(trump)) {
    return true;
  }

  bool a_trump = a.is_trump(trump);
  bool b_trump = b.is_trump(trump);

  // Only b is trump
  if (a_trump != b_trump) {
    return (!a_trump && b_trump);
  }

  // Neither are trump
  return a < b;
}


// a < b
bool Card_less(const Card &a, const Card &b, const Card &led_card, Suit trump) {
  // Checking equality and bowers
  if (a == b) {
    return false;
  }
  bool a_trump = a.is_trump(trump);
  bool b_trump = b.is_trump(trump);

  // Only b is trump
  if (a_trump != b_trump) {
    return (!a_trump && b_trump);
  }

  // Both are trump
  if (a_trump && b_trump) {
    return Card_less(a, b, trump);
  }

  // Neither are trump nor led
  Suit led_suit = led_card.get_suit(trump);
  bool a_follows = (a.get_suit(trump) == led_suit);
  bool b_follows = (b.get_suit(trump) == led_suit);

  if (a_follows != b_follows) {
    return (!a_follows && b_follows);
  }

  // Neither are trump nor led
  return a < b;
}





