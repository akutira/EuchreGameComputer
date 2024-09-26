// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "unit_test_framework.h"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());
    delete alice;

    Player * bowl = Player_factory("bowl", "Human");
    ASSERT_EQUAL("bowl", bowl->get_name());
    delete bowl;
}

TEST(test_insertion) {
    Player * bob = Player_factory("Bob", "Simple");
    ostringstream oss2;
    oss2 << * bob;
    ASSERT_EQUAL(oss2.str(), "Bob");
    delete bob;
}

TEST(test_make_trump_1) {
    Player * bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, SPADES));
    bob->add_card(Card(JACK, DIAMONDS));
    bob->add_card(Card(JACK, HEARTS));
    bob->add_card(Card(JACK, SPADES));
    bob->add_card(Card(TEN, SPADES));
    Suit trump;    

    Card d(KING, DIAMONDS);
    bool orderup2 = bob->make_trump(d, false, 1, trump);
    ASSERT_TRUE(orderup2);
    ASSERT_EQUAL(trump, DIAMONDS);

    delete bob;

    Player * Rico = Player_factory("Rico", "Simple");
    Rico->add_card(Card(NINE, DIAMONDS));
    Rico->add_card(Card(JACK, DIAMONDS));
    Rico->add_card(Card(JACK, HEARTS));
    Rico->add_card(Card(QUEEN, DIAMONDS));
    Rico->add_card(Card(TEN, DIAMONDS));    

    Suit trump1;
    Card g(KING, HEARTS);
    bool orderup4 = Rico->make_trump(g, false, 1, trump1);
    ASSERT_TRUE(orderup4);
    ASSERT_EQUAL(trump1, HEARTS);

    delete Rico;

    Player * Dell = Player_factory("Dell", "Simple");
    Dell->add_card(Card(NINE, DIAMONDS));
    Dell->add_card(Card(JACK, DIAMONDS));
    Dell->add_card(Card(JACK, HEARTS));
    Dell->add_card(Card(QUEEN, DIAMONDS));
    Dell->add_card(Card(TEN, DIAMONDS));   

    Suit trump3;
    Card l(KING, CLUBS);

    bool orderup6 = Dell->make_trump(l, true, 2, trump3);
    ASSERT_TRUE(orderup6); 
    ASSERT_EQUAL(trump3, SPADES);   

    delete Dell;

    Player * Barbara = Player_factory("Barbara", "Simple");
    Barbara->add_card(Card(NINE, SPADES));
    Barbara->add_card(Card(TEN, SPADES));
    Barbara->add_card(Card(JACK, SPADES));
    Barbara->add_card(Card(KING, HEARTS));
    Barbara->add_card(Card(ACE, HEARTS));   

    Suit trump4 = DIAMONDS;
    Card test(JACK, DIAMONDS);

    bool orderup7 = Barbara->make_trump(test, false, 2, trump4);
    ASSERT_TRUE(orderup7); 
    ASSERT_EQUAL(trump4, HEARTS);   

    delete Barbara;    
}

TEST(test_lead_card) {
    Player * cindy = Player_factory("Cindy", "Simple");
    cindy->add_card(Card(NINE, HEARTS)); 
    cindy->add_card(Card(JACK, DIAMONDS));
    cindy->add_card(Card(TEN, HEARTS)); 
    cindy->add_card(Card(TEN, SPADES)); // 2nd discard
    cindy->add_card(Card(ACE, CLUBS)); // 1st lead_card

    Card card_led = cindy->lead_card(HEARTS); 
    Card ans(ACE, CLUBS);
    ASSERT_EQUAL(card_led, ans);

    Card c(TEN, DIAMONDS); // 2nd lead_card
    cindy->add_and_discard(c);
    Card card_led_2 = cindy->lead_card(HEARTS);
    ASSERT_EQUAL(card_led_2, c);

    delete cindy;
}

TEST(test_lead_card_2) {
    Player * cindy = Player_factory("Cindy", "Simple");
    cindy->add_card(Card(NINE, SPADES));
    cindy->add_card(Card(KING, SPADES));
    cindy->add_card(Card(TEN, SPADES));
    cindy->add_card(Card(JACK, CLUBS));
    cindy->add_card(Card(ACE, DIAMONDS));

    
    Card card_led = cindy->lead_card(HEARTS);
    Card ans(ACE, DIAMONDS);
    ASSERT_EQUAL(card_led, ans);

    Card card_led_2 = cindy->lead_card(SPADES);
    Card ans_2(JACK, CLUBS);
    ASSERT_EQUAL(card_led_2, ans_2);

    
    Card card_led_3 = cindy->lead_card(CLUBS);
    Card ans_3(KING, SPADES);
    ASSERT_EQUAL(card_led_3, ans_3);
    
    Card card_led_4 = cindy->lead_card(DIAMONDS);
    Card ans_4(TEN, SPADES);
    ASSERT_EQUAL(card_led_4, ans_4);

    Card card_led_5 = cindy->lead_card(SPADES);
    Card ans_5(NINE, SPADES);
    ASSERT_EQUAL(card_led_5, ans_5);

    delete cindy;
}

TEST(test_simple_player_play_card_2) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(JACK, CLUBS));
  bob->add_card(Card(TEN, HEARTS));

  ASSERT_TRUE(Card(JACK, CLUBS).is_trump(SPADES));
  ASSERT_TRUE(Card(JACK, CLUBS).is_left_bower(SPADES));
  // Verify the card Bob played
  ASSERT_EQUAL(bob->play_card(Card(TEN, CLUBS), SPADES), Card(TEN, HEARTS));
  delete bob;
}

TEST(test_add_and_discard_right_left_bower){
  Player * bob = Player_factory("bob", "Simple");
  
  bob->add_card(Card(JACK, SPADES)); // 1
  bob->add_card(Card(JACK, CLUBS)); // 2
  bob->add_card(Card(JACK, DIAMONDS));
  bob->add_card(Card(JACK, HEARTS)); // 3
  bob->add_card(Card(NINE, SPADES)); // discard

  Card upcard = Card(TEN, HEARTS);
  Suit trump = HEARTS;
  bob->add_and_discard(upcard);
  
  ASSERT_EQUAL(bob->play_card(Card(TEN, SPADES), trump), Card(JACK, SPADES));
  ASSERT_EQUAL(bob->play_card(Card(NINE, CLUBS), trump), Card(JACK, CLUBS));
  ASSERT_EQUAL(bob->play_card(Card(NINE, HEARTS), trump), Card(JACK, HEARTS)); // right bower
  ASSERT_EQUAL(bob->play_card(Card(QUEEN, HEARTS), trump), Card(JACK, DIAMONDS)); // left bower (technically a hearts)
  ASSERT_EQUAL(bob->play_card(Card(TEN, DIAMONDS), trump), Card(TEN,HEARTS)); 

  delete bob;
}

TEST(test_player_insertion) {
  // Create a Human player
  Player * human = Player_factory("NotRobot", "Human");

  // Print the player using the stream insertion operator
  ostringstream oss1;
  oss1 << * human;

  // Verify that the output is the player's name
  ASSERT_EQUAL(oss1.str(), "NotRobot");

  // Create a Simple player
  Player * alice = Player_factory("Alice", "Simple");

  // Print the player using the stream insertion operator
  ostringstream oss2;
  oss2 << *alice;
  ASSERT_EQUAL(oss2.str(), "Alice");

  // Clean up players that were created using Player_factory()
  delete human;
  delete alice;
}


TEST(test_simple_player_make_trump) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob makes tump
  Card nine_spades(NINE, SPADES);
  Suit trump;
  bool orderup = bob->make_trump(
    nine_spades,    // Upcard
    true,           // Bob is also the dealer
    1,              // First round
    trump           // Suit ordered up (if any)
  );

  // Verify Bob's order up and trump suit
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, SPADES);

  delete bob;
}

TEST(test_simple_player_lead_card) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob adds a card to his hand and discards one card
  bob->add_and_discard(
    Card(NINE, HEARTS) // upcard
  );

  // Bob leads
  Card card_led = bob->lead_card(HEARTS);

  // Verify the card Bob selected to lead
  Card ace_spades(ACE, SPADES);
  ASSERT_EQUAL(card_led, ace_spades); //check led card

  delete bob;
}

TEST(test_simple_player_play_card) {
  // Bob's hand
  Player * bob = Player_factory("Bob", "Simple");
  bob->add_card(Card(NINE, SPADES));
  bob->add_card(Card(TEN, SPADES));
  bob->add_card(Card(QUEEN, SPADES));
  bob->add_card(Card(KING, SPADES));
  bob->add_card(Card(ACE, SPADES));

  // Bob plays a card
  Card nine_diamonds(NINE, DIAMONDS);
  Card card_played = bob->play_card(
    nine_diamonds,  // Nine of Diamonds is led
    HEARTS    // Trump suit
  ); 

  // Verify the card Bob played
  ASSERT_EQUAL(card_played, Card(NINE, SPADES));
  delete bob;

  Player * Chi = Player_factory("Chi-Chih", "Simple");
  Chi->add_card(Card(KING, SPADES));
  Chi->add_card(Card(QUEEN, SPADES));
  Chi->add_card(Card(NINE, CLUBS));
  Chi->add_card(Card(TEN, CLUBS));
  Chi->add_card(Card(JACK, CLUBS));

  // Bob plays a card
  Card Jack_spades(JACK, SPADES);
  Card card_played1 = Chi->play_card(
    Jack_spades,  
    HEARTS    // Trump suit
  ); 

  // Verify the card Bob played
  ASSERT_EQUAL(card_played1, Card(KING, SPADES));
  delete Chi;

  Player * adi = Player_factory("Adi", "Simple");
  adi->add_card(Card(JACK, HEARTS));
  adi->add_card(Card(QUEEN, HEARTS));
  //adi->add_card(Card(ACE, CLUBS));
  //adi->add_card(Card(NINE, DIAMONDS));
  //adi->add_card(Card(TEN, DIAMONDS));

  Card card_led_5 = adi->lead_card(HEARTS);
  Card ans_5(JACK, HEARTS);
  ASSERT_EQUAL(card_led_5, ans_5);

  delete adi;

  Player * Dabbala = Player_factory("Dabbala", "Simple");
  //Dabbala->add_card(Card(ACE, SPADES));
  Dabbala->add_card(Card(NINE, HEARTS));
  Dabbala->add_card(Card(TEN, HEARTS));
  //Dabbala->add_card(Card(QUEEN, CLUBS));
  //Dabbala->add_card(Card(KING, CLUBS));

  Card Jack_hearts(JACK, HEARTS);
  Card card_played2 = Dabbala->play_card(
    Jack_hearts,  
    HEARTS    // Trump suit
  ); 
  ASSERT_EQUAL(card_played2, Card(TEN, HEARTS));
  delete Dabbala;

  Player * Gab = Player_factory("Gab", "Simple");
  Gab->add_card(Card(ACE, CLUBS));
  Gab->add_card(Card(KING, SPADES));
  Gab->add_card(Card(NINE, CLUBS));
  Gab->add_card(Card(QUEEN, HEARTS));
  Gab->add_card(Card(JACK, DIAMONDS));

  Card s(KING, CLUBS);
  Card card_played3 = Gab->play_card(
    s,  
    HEARTS    // Trump suit
  ); 
  ASSERT_EQUAL(card_played3, Card(ACE, CLUBS));
  delete Gab;

Player * Odysseus = Player_factory("Odysseus", "Simple");
  //Odysseus->add_card(Card(ACE, DIAMONDS));
  //Odysseus->add_card(Card(KING, DIAMONDS));
  Odysseus->add_card(Card(QUEEN, DIAMONDS));
  Odysseus->add_card(Card(NINE, CLUBS));
  Odysseus->add_card(Card(TEN, CLUBS));

  Card jack_clubs(JACK, CLUBS);
  Card card_played4 = Odysseus->play_card(
    jack_clubs,  
    SPADES    // Trump suit
  ); 
  ASSERT_EQUAL(card_played4, Card(NINE, CLUBS));
  delete Odysseus;

}




// Add more tests here

TEST_MAIN()
