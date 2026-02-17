#include "Player.hpp"
#include <cassert>
#include <iostream>
#include <algorithm>

using namespace std;

class SimplePlayer : public Player {
public:
  SimplePlayer(const string &nameIn) : name(nameIn) {}

  const string & get_name() const override {
    return name;
  }

  void add_card(const Card &c) override {
    assert(hand.size() < MAX_HAND_SIZE);
    hand.push_back(c);
  }

  bool make_trump(const Card &upcard, bool isDealer, int round, Suit &orderUpSuit) const override {
    assert(round == 1 || round == 2);

    if (round == 1) {
      Suit trump = upcard.get_suit();
      int count = countGoodCards(trump);

      if (count >= 2) {
        orderUpSuit = trump;
        return true;
      }
      return false;
    }

    Suit next = Suit_next(upcard.get_suit());
    int count = countGoodCards(next);

    if (count >= 1 || isDealer) {
      orderUpSuit = next;
      return true;
    }
    return false;
  }

  void add_and_discard(const Card &upcard) override {
    Suit trump = upcard.get_suit();
    hand.push_back(upcard);

    size_t lowest = 0;
    for (size_t i = 1; i < hand.size(); i++) {
      if (Card_less(hand[i], hand[lowest], trump)) {
        lowest = i;
      }
    }
    hand.erase(hand.begin() + lowest);
  }

  Card lead_card(Suit trump) override {
    int best = -1;

    for (size_t i = 0; i < hand.size(); i++) {
      if (!hand[i].is_trump(trump)) {
        if (best == -1 || hand[best] < hand[i]) {
      best = i;
        }
      }
    }

    if (best == -1) {
      best = 0;
      for (size_t i = 1; i < hand.size(); i++) {
        if (Card_less(hand[best], hand[i], trump)) {
          best = i;
        }
      }
    }
  
    Card result = hand[best];
    hand.erase(hand.begin() + best);
    return result;
  }

  Card play_card(const Card &ledCard, Suit trump) override {
    Suit ledSuit = ledCard.get_suit(trump);

    bool canFollow = false;
    for (size_t i = 0; i < hand.size(); i++) {
      if (hand[i].get_suit(trump) == ledSuit) {
        canFollow = true;
        break;
      }
    }

    int pick = 0;

    if (canFollow) {
      pick = -1;
      for (size_t i = 0; i < hand.size(); i++) {
        if (hand[i].get_suit(trump) == ledSuit) {
          if (pick == -1 || Card_less(hand[pick], hand[i], ledCard, trump)) {
            pick = i;
          }
        }
      }
    }
    
    else {
      for (size_t i = 1; i < hand.size(); i++) {
        if (Card_less(hand[i], hand[pick], ledCard, trump)) {
          pick = i;
        }
      }
    }

    Card result = hand[pick];
    hand.erase(hand.begin() + pick);
    return result;
  }

private:
  string name;
  vector<Card> hand;

  int countGoodCards(Suit trump) const {
    int count = 0;
    for (int i = 0; i < hand.size(); i++) {
      if (hand[i].is_trump(trump) && hand[i].is_face_or_ace()) {
        count++;
      }
    }
    return count;
  }
};

class HumanPlayer : public Player {
public:
  HumanPlayer(const string &nameIn) : name(nameIn) {}

  const string & get_name() const override {
    return name;
  }

  void add_card(const Card &c) override {
    assert(hand.size() < MAX_HAND_SIZE);
    hand.push_back(c);
    sort(hand.begin(), hand.end());
  }

  bool make_trump(const Card &, bool, int, Suit &orderUpSuit) const override {
    printHand();
    cout << "Human player " << name << ", please enter a suit, or \"pass\":" << endl;

    string decision;
    cin >> decision;

    if (decision == "pass") {
      return false;
    }

    orderUpSuit = string_to_suit(decision);
    return true;
  }

  void add_and_discard(const Card &upcard) override {
    printHand();
    cout << "Discard upcard: [-1]" << endl;
    cout << "Human player " << name << ", please select a card to discard:" << endl;

    int idx;
    cin >> idx;

    if (idx == -1) {
      return;
    }

    hand.erase(hand.begin() + idx);
    hand.push_back(upcard);
    sort(hand.begin(), hand.end());
  }

  Card lead_card(Suit) override {
    printHand();
    cout << "Human player " << name << ", please select a card:" << endl;

    int idx;
    cin >> idx;

    Card result = hand[idx];
    hand.erase(hand.begin() + idx);
    return result;
  }

  Card play_card(const Card &, Suit trump) override {
    return lead_card(trump);
  }

private:
  string name;
  vector<Card> hand;

  void printHand() const {
    for (size_t i = 0; i < hand.size(); i++) {
      cout << "Human player " << name << "'s hand: " << "[" << i << "] " << hand[i] << endl;
    }
  }
};

Player * Player_factory(const string &name, const string &strategy) {
  if (strategy == "Simple") {
    return new SimplePlayer(name);
  }
  if (strategy == "Human") {
    return new HumanPlayer(name);
  }
  assert(false);
  return nullptr;
}

ostream & operator<<(ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}

