// Project UID 1d9f47bfc76643019cfbf037641defe1


#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <cassert>
#include "Player.h"
#include "Card.h"
#include <algorithm>


using namespace std;

class SimplePlayer : public Player {
  private:
    std::vector<Card> hand;
    int position;
    std::string name;
    
    
  public:
    SimplePlayer(std::string name):name(name){
    }
    
    const std::string & get_name() const override{
        return name;
    }
    
    
    void add_card (const Card &c) override{
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);
        sort(hand.begin(), hand.end());
    }
    
    bool make_trump(const Card &upcard, bool is_dealer, int round, 
    Suit &order_up_suit) const override{
      assert(round == 1 || round == 2);
      int num_trumpface = 0;
      int num_nextface = 0;
      for(int i = 0; i < MAX_HAND_SIZE; i++){
        if(hand[i].is_trump(upcard.get_suit()) && (hand[i].is_face_or_ace())){
          num_trumpface++;
        }
      }
      if(round == 1){
        if(num_trumpface >= 2){
          order_up_suit = upcard.get_suit();
          return true;
        }
        return false;
      } 
      else if (round == 2){
        for(int i = 0; i < MAX_HAND_SIZE; i++){
          if(hand[i].is_trump(Suit_next(upcard.get_suit())) 
          && (hand[i].is_face_or_ace())){
          num_nextface++;
          }
        }
        if(is_dealer == true){
          order_up_suit = Suit_next(upcard.get_suit());
          return true;
        } else if(num_nextface >= 1){
          Suit order_up_color = Suit_next(upcard.get_suit());
          order_up_suit = order_up_color;
          return true;
        } else {
          return false;
        }  
      }
        return false;
    }
    
    void add_and_discard(const Card &upcard) override {
      assert(hand.size() >= 1);
      hand.push_back(upcard);
      Card* min_card = &hand[0];
      int worst_suit = 0;
      for (size_t i = 1; i < hand.size(); i++) {
        if (Card_less(hand[i], *min_card, upcard.get_suit())) {
          min_card = &hand[i];
          worst_suit = i;
        }
      }
      hand.erase(hand.begin() + worst_suit);
    }
    
    Card lead_card(Suit trump) override {
      sort(hand.begin(), hand.end());
      assert(hand.size() >= 1);
      int highest_non_trump_index = -1;
      for (int i = 0; i < hand.size(); i++) {
        if(!hand[i].is_trump(trump)) {
          if (highest_non_trump_index == -1 || hand[i] 
          > hand[highest_non_trump_index]) {
            highest_non_trump_index = i;
          }
        }
      }
      if(highest_non_trump_index != -1) {
        Card card = hand[highest_non_trump_index];
        hand.erase(hand.begin() + highest_non_trump_index);
        return card;
      }
      int highest_trump_index = 0;
      for(int i = 1; i < hand.size(); i++) {
        if(hand[i].is_right_bower(trump)) {
          highest_trump_index = i;
          break;
        }
        if(hand[i].is_left_bower(trump)) {
          highest_trump_index = i;
          break;
        }
        if(Card_less(hand[highest_trump_index], hand[i], trump) == true) {
          highest_trump_index = i;
        }
      }
      Card card = hand[highest_trump_index];
      hand.erase(hand.begin() + highest_trump_index);
      return card;
    }
    
   Card play_card(const Card& led_card, Suit trump) override {
      assert(1 <= hand.size() && hand.size() <= 5);
      int best_suit_index = -1, worst_suit_index = -1;
      for (size_t i = 0; i < hand.size(); i++) {
        bool is_lead_suit = (hand[i].get_suit(trump) == led_card.get_suit(trump));
        if (is_lead_suit && (best_suit_index == -1)) {
          best_suit_index = i;
        } else if (is_lead_suit && Card_less(hand[best_suit_index], hand[i],
        led_card, trump)) {
          best_suit_index = i;
        }  
      }
      for (size_t i = 0; i < hand.size(); i++) {
        bool is_not_lead_suit = (hand[i].get_suit(trump) != led_card.get_suit(trump));
        if (is_not_lead_suit && (worst_suit_index == -1)) {
          worst_suit_index = i;
        } else if (is_not_lead_suit && Card_less(hand[i], hand[worst_suit_index], 
        led_card, trump)) {
          worst_suit_index = i;
        }
      }
      if (best_suit_index == -1) {
        Card card = hand[worst_suit_index];
        hand.erase(hand.begin() + worst_suit_index);
        return card;
      }
      Card card = hand[best_suit_index];
      hand.erase(hand.begin() + best_suit_index);
      return card;
   }  

    ~SimplePlayer() override{}
};

class HumanPlayer: public Player {
  private:
    std::vector<Card> hand;
    int position;
    std::string name;
    void print_hand() const {
  for (size_t i = 0; i < hand.size(); ++i)
    cout << "Human player " << name << "'s hand: "
         << "[" << i << "] " << hand[i] << "\n";
  }

  public:
    HumanPlayer(std::string name):name(name){
    }
    
    const std::string & get_name() const override{
        return name;
    }
    
    void add_card(const Card &c) override {
        assert(hand.size() <= MAX_HAND_SIZE);
        hand.push_back(c);
        sort(hand.begin(), hand.end());
    }
    
    bool make_trump(const Card &upcard, bool is_dealer, int round, 
    Suit &order_up_suit) const override {
      print_hand();
      cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
      string decision;
      cin >> decision;
      if (decision != "pass") {
        Suit ordered_up = string_to_suit(decision);
        order_up_suit = ordered_up;
        return true;
      } else {
        return false;
      }
    }

    void add_and_discard(const Card &upcard) override {
      assert(hand.size() > 0);
      sort(hand.begin(), hand.end());
      print_hand();
      cout << "Discard upcard: [-1]\n";
      cout << "Human player " << name << ", please select a card to discard:\n";
      int decision;
      cin >> decision;
      if (decision != -1) {
        hand.push_back(upcard);
        hand.erase(hand.begin() + decision);
      } 
      sort(hand.begin(), hand.end());
    } 
    
    Card lead_card(Suit trump) override{
      assert(hand.size() >= 1);
      sort(hand.begin(), hand.end());
      print_hand();
      cout << "Human player " << name << ", please select a card:\n";
      int decision;
      cin >> decision;
      Card selected_card = hand[decision];
      hand.erase(hand.begin() + decision);
      return selected_card;
    }

    Card play_card(const Card &led_card, Suit trump) override{
      assert(hand.size() >= 1);
      sort(hand.begin(), hand.end());
      print_hand();
      cout << "Human player " << name << ", please select a card:\n";
      int decision;
      cin >> decision;
      Card selected_card = hand[decision];
      hand.erase(hand.begin() + decision);
      return selected_card;       
    }
    
     ~HumanPlayer() override{}
};


Player* Player_factory(const std::string &name,
                        const std::string &strategy) {
  if (strategy == "Simple") {
    return new SimplePlayer(name);
  }
  if( strategy == "Human"){
      return new HumanPlayer(name);
  }
  assert(false);
  return nullptr;
}


std::ostream &operator<<(std::ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}



