#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

TEST(test_bowers){
    Card jack_clubs(JACK, CLUBS);
    Card jack_spades(JACK, SPADES);

    // Check for trump
    ASSERT_TRUE(jack_clubs.is_trump(CLUBS));
    ASSERT_TRUE(jack_spades.is_trump(SPADES));

    // Right bower
    ASSERT_TRUE(jack_clubs.is_right_bower(CLUBS));
    ASSERT_TRUE(jack_spades.is_right_bower(SPADES));

    // Left bower
    ASSERT_TRUE(jack_clubs.is_left_bower(SPADES));
    ASSERT_TRUE(jack_spades.is_left_bower(CLUBS));
}

TEST(test_operators){
    Card nine_hearts(NINE, HEARTS);
    Card nine_hearts_2(NINE, HEARTS);
    Card ten_hearts(TEN, HEARTS);

    ASSERT_TRUE(nine_hearts < ten_hearts);
    ASSERT_TRUE(ten_hearts > nine_hearts);
    ASSERT_TRUE(nine_hearts == nine_hearts_2);
    ASSERT_TRUE(nine_hearts != ten_hearts);
    ASSERT_TRUE(nine_hearts <= ten_hearts);
    ASSERT_TRUE(ten_hearts >= nine_hearts_2);
}

TEST(test_card_less){
    Card king_diamonds(KING, DIAMONDS);
    Card queen_spades(QUEEN, SPADES);
    Card jack_clubs(JACK, CLUBS);
    Card jack_spades(JACK, SPADES);
    Card ace_spades(ACE, SPADES);

    ASSERT_TRUE(Card_less(king_diamonds, queen_spades, SPADES));
    ASSERT_TRUE(Card_less(jack_clubs, jack_spades, SPADES));
    ASSERT_TRUE(Card_less(jack_spades, jack_clubs, CLUBS));

    ASSERT_TRUE(Card_less(ace_spades, jack_clubs, CLUBS));
    ASSERT_TRUE(Card_less(ace_spades, jack_spades, CLUBS));
}

TEST(test_card_less_led){
    Card ace_spades(ACE, SPADES);
    Card king_spades(KING, SPADES);
    Card nine_hearts(NINE, HEARTS);
    Card jack_hearts(JACK, HEARTS);
    Card jack_diamonds(JACK, DIAMONDS);

    ASSERT_TRUE(Card_less(ace_spades, nine_hearts, jack_hearts, HEARTS));
    ASSERT_TRUE(Card_less(king_spades, ace_spades, jack_hearts, HEARTS));
    ASSERT_TRUE(Card_less(nine_hearts, ace_spades, jack_hearts, SPADES));
    ASSERT_TRUE(Card_less(nine_hearts, jack_hearts, king_spades, HEARTS));
    ASSERT_TRUE(Card_less(ace_spades, jack_diamonds, king_spades, HEARTS));
    ASSERT_TRUE(Card_less(jack_diamonds, jack_hearts, king_spades, HEARTS));
}

TEST_MAIN()
