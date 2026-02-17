#include "Player.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;

TEST(getName) {
  Player *p = Player_factory("Alice", "Simple");
  ASSERT_EQUAL(p->get_name(), "Alice");
  delete p;
}

TEST(round1Order) {
  Player *p = Player_factory("Alice", "Simple");
  Card upcard(NINE, HEARTS);

  p->add_card(Card(ACE, HEARTS));
  p->add_card(Card(KING, HEARTS));
  p->add_card(Card(NINE, SPADES));
  p->add_card(Card(TEN, SPADES));
  p->add_card(Card(NINE, CLUBS));

  Suit suit = SPADES;
  ASSERT_TRUE(p->make_trump(upcard, false, 1, suit));
  ASSERT_EQUAL(suit, HEARTS);

  delete p;
}

TEST(round1Pass) {
  Player *p = Player_factory("Alice", "Simple");
  Card upcard(TEN, DIAMONDS);

  p->add_card(Card(ACE, DIAMONDS));
  p->add_card(Card(NINE, SPADES));
  p->add_card(Card(TEN, SPADES));
  p->add_card(Card(NINE, CLUBS));
  p->add_card(Card(TEN, CLUBS));

  Suit suit = HEARTS;
  ASSERT_FALSE(p->make_trump(upcard, false, 1, suit));
  ASSERT_EQUAL(suit, HEARTS);

  delete p;
}

TEST(leftBowerCounts) {
  Player *p = Player_factory("Alice", "Simple");
  Card upcard(NINE, DIAMONDS);

  p->add_card(Card(JACK, HEARTS)); 
  p->add_card(Card(ACE, DIAMONDS));
  p->add_card(Card(NINE, SPADES));
  p->add_card(Card(TEN, SPADES));
  p->add_card(Card(NINE, CLUBS));

  Suit suit = SPADES;
  ASSERT_TRUE(p->make_trump(upcard, false, 1, suit));
  ASSERT_EQUAL(suit, DIAMONDS);

  delete p;
}

TEST(round2Order) {
  Player *p = Player_factory("Alice", "Simple");
  Card upcard(NINE, HEARTS);

  p->add_card(Card(KING, DIAMONDS));
  p->add_card(Card(NINE, SPADES));
  p->add_card(Card(TEN, SPADES));
  p->add_card(Card(NINE, CLUBS));
  p->add_card(Card(TEN, CLUBS));

  Suit suit = SPADES;
  ASSERT_TRUE(p->make_trump(upcard, false, 2, suit));
  ASSERT_EQUAL(suit, DIAMONDS);

  delete p;
}

TEST(round2Pass) {
  Player *p = Player_factory("Alice", "Simple");
  Card upcard(NINE, HEARTS);

  p->add_card(Card(NINE, SPADES));
  p->add_card(Card(TEN, SPADES));
  p->add_card(Card(QUEEN, SPADES));
  p->add_card(Card(KING, SPADES));
  p->add_card(Card(ACE, SPADES));

  Suit suit = CLUBS;
  ASSERT_FALSE(p->make_trump(upcard, false, 2, suit));
  ASSERT_EQUAL(suit, CLUBS);

  delete p;
}

TEST(screwDealer) {
  Player *p = Player_factory("Dealer", "Simple");
  Card upcard(NINE, CLUBS);

  p->add_card(Card(NINE, HEARTS));
  p->add_card(Card(TEN, HEARTS));
  p->add_card(Card(NINE, DIAMONDS));
  p->add_card(Card(TEN, DIAMONDS));
  p->add_card(Card(NINE, CLUBS));

  Suit suit = HEARTS;
  ASSERT_TRUE(p->make_trump(upcard, true, 2, suit));
  ASSERT_EQUAL(suit, SPADES);

  delete p;
}

TEST(discardLowest) {
  Player *p = Player_factory("Alice", "Simple");

  p->add_card(Card(NINE, SPADES));
  p->add_card(Card(TEN, SPADES));
  p->add_card(Card(JACK, SPADES));
  p->add_card(Card(QUEEN, SPADES));
  p->add_card(Card(KING, SPADES));

  Card upcard(NINE, HEARTS);
  p->add_and_discard(upcard);

  Card led(ACE, HEARTS);
  Card played = p->play_card(led, HEARTS);

  ASSERT_EQUAL(played.get_suit(HEARTS), HEARTS);

  delete p;
}

TEST(leadNonTrump) {
  Player *p = Player_factory("Alice", "Simple");
  Suit trump = HEARTS;

  p->add_card(Card(ACE, CLUBS));
  p->add_card(Card(NINE, HEARTS));
  p->add_card(Card(TEN, SPADES));
  p->add_card(Card(NINE, SPADES));
  p->add_card(Card(TEN, CLUBS));

  Card led = p->lead_card(trump);
  ASSERT_EQUAL(led, Card(ACE, CLUBS));

  delete p;
}

TEST(leadTrumpOnly) {
  Player *p = Player_factory("Alice", "Simple");
  Suit trump = DIAMONDS;

  p->add_card(Card(NINE, DIAMONDS));
  p->add_card(Card(TEN, DIAMONDS));
  p->add_card(Card(QUEEN, DIAMONDS));
  p->add_card(Card(JACK, HEARTS));   
  p->add_card(Card(JACK, DIAMONDS)); 

  Card led = p->lead_card(trump);
  ASSERT_TRUE(led.is_right_bower(trump));

  delete p;
}

TEST(playFollow) {
  Player *p = Player_factory("Alice", "Simple");
  Suit trump = HEARTS;

  p->add_card(Card(NINE, CLUBS));
  p->add_card(Card(ACE, CLUBS));
  p->add_card(Card(TEN, SPADES));
  p->add_card(Card(NINE, SPADES));
  p->add_card(Card(NINE, HEARTS));

  Card led(KING, CLUBS);
  Card played = p->play_card(led, trump);

  ASSERT_EQUAL(played, Card(ACE, CLUBS));

  delete p;
}

TEST(playLowest) {
  Player *p = Player_factory("Alice", "Simple");
  Suit trump = CLUBS;

  p->add_card(Card(ACE, SPADES));
  p->add_card(Card(KING, SPADES));
  p->add_card(Card(QUEEN, SPADES));
  p->add_card(Card(TEN, SPADES));
  p->add_card(Card(NINE, CLUBS));

  Card led(NINE, HEARTS);
  Card played = p->play_card(led, trump);

  ASSERT_EQUAL(played, Card(TEN, SPADES));

  delete p;
}

TEST(leftBowerFollow) {
  Player *p = Player_factory("Alice", "Simple");
  Suit trump = HEARTS;

  p->add_card(Card(JACK, DIAMONDS));
  p->add_card(Card(NINE, SPADES));
  p->add_card(Card(TEN, SPADES));
  p->add_card(Card(NINE, CLUBS));
  p->add_card(Card(TEN, CLUBS));

  Card led(ACE, HEARTS);
  Card played = p->play_card(led, trump);

  ASSERT_TRUE(played.is_left_bower(trump));

  delete p;
}

TEST_MAIN()

