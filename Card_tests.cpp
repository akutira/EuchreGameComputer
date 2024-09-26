// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card two_spades = Card();
    ASSERT_EQUAL(two_spades.get_rank(), TWO);
    ASSERT_EQUAL(two_spades.get_suit(), SPADES);
    
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

TEST(test_suit_next) {
    Suit c1 = Suit_next(HEARTS);
    ASSERT_EQUAL(DIAMONDS, c1);
    Suit c2 = Suit_next(DIAMONDS);
    ASSERT_EQUAL(HEARTS, c2);
    Suit c3 = Suit_next(SPADES);
    ASSERT_EQUAL(CLUBS, c3);
    Suit c4 = Suit_next(CLUBS);
    ASSERT_EQUAL(SPADES, c4);

}

TEST(test_card_init) {
    Card queen_diamonds = Card(QUEEN, DIAMONDS);
    Card king_clubs = Card(KING, CLUBS);
    Card jack_spades = Card(JACK, SPADES);
    ASSERT_EQUAL(queen_diamonds.get_rank(), QUEEN);
    ASSERT_EQUAL(queen_diamonds.get_suit(), DIAMONDS);
    ASSERT_EQUAL(queen_diamonds.get_suit(CLUBS), DIAMONDS);
    ASSERT_EQUAL(king_clubs.get_suit(CLUBS), CLUBS);
    ASSERT_EQUAL(jack_spades.get_suit(CLUBS), CLUBS);

}

TEST(test_card_is_right_bower) {
    Card jack_spades = Card(JACK, SPADES);
    Card jack_clubs = Card(JACK, CLUBS);
    Card six_diamonds = Card(SIX, DIAMONDS);
    ASSERT_TRUE(jack_spades.is_right_bower(SPADES));
    ASSERT_FALSE(jack_clubs.is_right_bower(SPADES)); 
    ASSERT_FALSE(six_diamonds.is_right_bower(DIAMONDS));
}

TEST(test_card_is_left_bower) {
    Card jack_diamonds = Card(JACK, DIAMONDS);
    Card jack_hearts = Card(JACK, HEARTS);
    Card six_diamonds = Card(SIX, DIAMONDS);
    ASSERT_TRUE(jack_diamonds.is_left_bower(HEARTS));
    ASSERT_FALSE(six_diamonds.is_left_bower(HEARTS));
    ASSERT_FALSE(jack_hearts.is_left_bower(HEARTS));
}

TEST(test_card_is_face_or_ace) {
    Card jack_diamonds = Card(JACK, DIAMONDS);
    Card ace_clubs = Card(ACE, CLUBS);
    Card two_spades = Card(TWO, SPADES);
    ASSERT_TRUE(jack_diamonds.is_face_or_ace());
    ASSERT_TRUE(ace_clubs.is_face_or_ace());
    ASSERT_FALSE(two_spades.is_face_or_ace());
}

TEST(test_card_is_trump) {
    Card jack_diamonds = Card(JACK, DIAMONDS);
    Card jack_hearts = Card(JACK, HEARTS);
    Card six_diamonds = Card(SIX, DIAMONDS);
    Card seven_hearts = Card(SEVEN, HEARTS);
    ASSERT_FALSE(seven_hearts.is_trump(DIAMONDS));
    ASSERT_TRUE(jack_diamonds.is_trump(DIAMONDS));
    ASSERT_TRUE(jack_hearts.is_trump(DIAMONDS));
    ASSERT_TRUE(six_diamonds.is_trump(DIAMONDS));
}

TEST(test_card_extraction) {
    istringstream iss("Ace of Hearts");
    Card c;
    iss >> c;
    ASSERT_EQUAL(c, Card(ACE, HEARTS));

    Card three_spades = Card(NINE, CLUBS);
    ostringstream oss;
    oss << three_spades;
    ASSERT_EQUAL(oss.str(), "Nine of Clubs");
}

TEST(test_card_less) {
Card ace_diamonds = Card(ACE, DIAMONDS);
Card ace_clubs = Card(ACE, CLUBS);
Card nine_clubs = Card(NINE, CLUBS);
Card nine_hearts = Card(NINE, HEARTS);
Card ace_spades = Card(ACE, SPADES);
Card nine_diamonds = Card(NINE, DIAMONDS);
Card nine_spades = Card(NINE, SPADES);
Card ace_hearts = Card(ACE, HEARTS);
Card jack_clubs = Card(JACK, CLUBS);
//Card nine_hearts = Card(NINE, HEARTS);
    ASSERT_TRUE(Card_less(ace_clubs, ace_diamonds, SPADES));
    ASSERT_FALSE(Card_less(nine_spades, nine_spades, CLUBS));
    ASSERT_TRUE(Card_less(ace_clubs, ace_spades, SPADES));
    ASSERT_TRUE(Card_less(nine_spades, jack_clubs, SPADES));
    ASSERT_TRUE(jack_clubs.is_trump(SPADES));
    ASSERT_TRUE(Card_less(ace_clubs, nine_spades, SPADES));
    ASSERT_TRUE(Card_less(nine_spades, nine_clubs, ace_diamonds, HEARTS));
    ASSERT_FALSE(Card_less(ace_diamonds, nine_spades, nine_diamonds, HEARTS));
    ASSERT_TRUE(Card_less(nine_hearts, nine_clubs, ace_hearts, CLUBS));
    ASSERT_TRUE(Card_less(nine_hearts, nine_spades, ace_diamonds, SPADES));
    ASSERT_TRUE(Card_less(nine_diamonds, jack_clubs, ace_diamonds, SPADES));
    ASSERT_TRUE(Card_less(nine_hearts, nine_spades, ace_spades, SPADES));
}

TEST(test_card_self_comparison) {
    Card three_spades = Card(THREE, SPADES);
    Card nine_spades = Card(NINE, SPADES);
    Card nine_hearts = Card(NINE, HEARTS);
    //Card ace_spades = Card(ACE, SPADES);
    Card three_diamonds = Card(THREE, DIAMONDS);
    ASSERT_TRUE(three_spades < nine_hearts);
    ASSERT_TRUE(nine_spades < nine_hearts);
    ASSERT_TRUE(three_spades <= three_diamonds);
    ASSERT_FALSE(three_spades > three_spades);
    ASSERT_TRUE(nine_hearts >= nine_spades);
    ASSERT_TRUE(nine_hearts == nine_hearts);
    ASSERT_FALSE(nine_spades != nine_spades);
}


TEST(test_card_less1) {
Card jack_spades = Card(JACK, SPADES);
Card ace_hearts = Card(ACE, HEARTS);
Card queen_hearts = Card(QUEEN, HEARTS);
Card nine_diamonds = Card(NINE, DIAMONDS);
//ASSERT_TRUE(jack_spades.is_right_bower(HEARTS));
    ASSERT_TRUE(Card_less(jack_spades, ace_hearts, jack_spades, HEARTS));
    ASSERT_TRUE(Card_less(queen_hearts, ace_hearts, jack_spades, HEARTS));
    ASSERT_TRUE(Card_less(nine_diamonds, ace_hearts, jack_spades, HEARTS));

Card ten_clubs = Card(TEN, CLUBS);
Card queen_diamonds = Card(QUEEN, DIAMONDS);
Card nine_clubs = Card(NINE, CLUBS);
Card jack_clubs = Card(JACK, CLUBS);
    ASSERT_TRUE(Card_less(nine_clubs, ten_clubs, jack_clubs, SPADES));
    ASSERT_TRUE(jack_clubs.is_left_bower(SPADES));
    ASSERT_TRUE(Card_less(ten_clubs, queen_diamonds, jack_clubs, SPADES));
    ASSERT_TRUE(Card_less(nine_clubs, queen_diamonds, jack_clubs, SPADES));
}


// Add more test cases here

TEST_MAIN()
