// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <vector>
#include "Player.h"
#include "Pack.h"
#include <string>
#include <cassert>

using namespace std;
class Game{
    private:
        vector<Player*> players;
        Pack deck;
        Suit trump;
        Card upcard;
        bool shuffle;
        int team2_points;
        int team1_points;
        bool team2_orderup;
        bool team1_orderup;
        int points_to_win;

    public:

        Game(vector<Player*> playmakers, Pack cards, bool shuffled, int finish_line_points){
            players = playmakers;
            points_to_win = finish_line_points;
            deck = cards;
            team2_points = 0;
            team1_points = 0;
            team2_orderup = false;
            team1_orderup = false;
            shuffle = shuffled;
        }
        void play() {
            Player* dealer = players[0];
            Player* leader = players[1];
            int hand = 0;
            set_table(dealer, leader, hand);
        }
        void set_table(Player* dealer, Player* leader, int hand) {
            if (shuffle) {
                deck.shuffle();
            } else {
                deck.reset();
            }
            team1_orderup = false;
            team2_orderup = false;
            cout << "Hand " << hand <<endl;
            deal(dealer);

            upcard = deck.deal_one();
            cout << upcard << " turned up" << endl;
            bool if_trump = make_trumpGame(upcard, dealer, upcard.get_suit(), 1);
            if (if_trump){
                dealer->add_and_discard(upcard);
            } else if (!if_trump) {
                make_trumpGame(upcard, dealer, upcard.get_suit(), 2);
            }
            bool winner = trick_taking(leader);
            dealer = next_turn(dealer);
            leader = next_turn(leader);
            if (!winner) {
                set_table(dealer, leader, hand + 1);
            }
        }
        void deal(Player* dealer) {
            int round = 1;
            cout << dealer->get_name() << " deals" << endl;
            Player* player = next_turn(dealer);
            while (round < 3) {
                if (round == 1) {
                    int deal_amounts[4] = {3, 2, 3, 2};
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < deal_amounts[i]; j++) {
                            player->add_card(deck.deal_one());
                        }
                        player = next_turn(player);
                    }
                } else if (round == 2) {
                    int deal_amounts[4] = {2, 3, 2, 3};
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < deal_amounts[i]; j++) {
                            player->add_card(deck.deal_one());
                        }
                        player = next_turn(player);
                    }
                }
                round++;
            }
        }
        bool make_trumpGame(const Card &upcard, Player* dealer, Suit upsuit, int round){
            assert(round == 1 || round == 2);
            Player* trump_maker = next_turn(dealer);
            for (int i = 0; i < 4; i++){  
                upsuit = upcard.get_suit();
                bool orderedup = true;
                if (trump_maker == dealer){
                    orderedup = trump_maker->make_trump(upcard, true, round, upsuit);
                }
                else {
                    orderedup = trump_maker->make_trump(upcard, false, round, upsuit);
                }
                if (orderedup) { 
                    trump = upsuit;
                    cout << trump_maker->get_name() << " orders up " << trump << endl;
                    cout << endl;
                    if (trump_maker == players[0] || trump_maker == players[2]) {
                        team1_orderup = true;
                    } else {
                        team2_orderup = true;
                    }
                    return true;
                }
                else if(orderedup = true){
                    cout << trump_maker->get_name() << " passes" << endl;
                }
                
                trump_maker = next_turn(trump_maker);
           }
            return false;
        }
        Player* play_hand(Suit trump, Player* leader){
            Card deck[4];
            Card ledcard = leader->lead_card(trump);
            deck[0] = ledcard;
            cout << ledcard << " led by " << leader->get_name() << endl;
            for (int i = 1; i < 4; i++){
                leader = next_turn(leader);
                Card playedCard = leader->play_card(ledcard, trump);
                deck[i] = playedCard;
                cout << playedCard << " played by " << leader->get_name() << endl;
            }
            
            Player* bestPlayer = next_turn(leader);
            Player* nextPlayer = next_turn(bestPlayer);
            Card bestCard = deck[0];
            for (int i = 1; i < 4; i++){
                if (Card_less(bestCard, deck[i], ledcard, trump) == true){
                    bestCard = deck[i];
                    bestPlayer = nextPlayer;
                }
                nextPlayer = next_turn(nextPlayer);
            }
            cout << bestPlayer->get_name() << " takes the trick" << endl;
            cout << endl;
            return bestPlayer;
        }
        bool trick_taking(Player* leader){
            int team1_round_points = 0;
            int team2_round_points = 0;
            for (int i = 0; i < 5; i++){
                Player* trick_winner = play_hand(trump, leader);
                leader = trick_winner;
                if (trick_winner == players[1] || trick_winner == players[3]){
                    team2_round_points = team2_round_points + 1;
                }
                else{
                    team1_round_points = team1_round_points + 1;
                }
            }
            
            bool endgame = false;
            if (team2_round_points > team1_round_points){
                endgame = score_board(team2_round_points, 2);
            } else {
                endgame = score_board(team1_round_points, 1);
            }

            cout << players[0]->get_name() << " and " << players[2]->get_name()
            << " have " << team1_points << " points" << endl;
            cout << players[1]->get_name() << " and " << players[3]->get_name()
            << " have " << team2_points << " points" << endl;
            cout << endl;

            if (endgame == true) {
                if (team1_points >= points_to_win){
                    cout << players[0]->get_name() << " and " << 
                    players[2]->get_name() << " win!" << endl;
                }
                else if (team2_points >= points_to_win){
                    cout << players[1]->get_name() << " and " << 
                    players[3]->get_name() << " win!" << endl;
                }
            }
            return endgame;
        }

        bool score_board(int end_round_winner, int team_winner){
            if (team_winner == 1){
                cout << players[0]->get_name() << " and " << players[2]->get_name() 
                << " win the hand" << endl;
                if (!team1_orderup){
                    team1_points = team1_points + 2;
                    cout << "euchred!" << endl;
                } else if (end_round_winner == 5){
                    team1_points = team1_points + 2;
                    cout << "march!" << endl;
                } else {
                    team1_points = team1_points + 1;
                }
            } else if (team_winner == 2) {
                cout << players[1]->get_name() << " and " << players[3]->get_name() 
                << " win the hand" << endl;
                if (!team2_orderup){
                    team2_points = team2_points + 2;
                    cout << "euchred!" << endl;
                } else if (end_round_winner == 5){
                    team2_points = team2_points + 2;
                    cout << "march!" << endl;
                } else {
                    team2_points = team2_points + 1;
                }
            }

            if (team2_points >= points_to_win || team1_points >= points_to_win) {
                return true;
            } else {
                return false;
            }
        }
        Player* next_turn(Player* player){
            if (player == players[0]){
                return players[1];
            }
            else if (player == players[1]){
                return players[2];
            }
            else if (player == players[2]) {
                return players[3];
            }
            else if (player == players[3]) {
                return players[0];
            }
            else{
                return players[0];
            }
        }
};

int main(int argc, char *argv[]) {
    int finish_line_points = atoi(argv[3]);
    bool shuffle = false;
    if (strcmp(argv[2], "shuffle") == 0){ shuffle = true; }
    if (argc != 12) { 
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }
    if (finish_line_points < 1 || finish_line_points > 100){
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }
    if (strcmp(argv[2], "shuffle") != 0 && strcmp(argv[2], "noshuffle") != 0){
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }
    if (!((strcmp(argv[5], "Simple") != 0 || strcmp(argv[5], "Human") != 0) 
    && (strcmp(argv[7], "Simple") != 0 || strcmp(argv[7], "Human") != 0) 
    && (strcmp(argv[9], "Simple") != 0 || strcmp(argv[9], "Human") != 0)
    && (strcmp(argv[11], "Simple") != 0 || strcmp(argv[11], "Human") != 0))){
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }

    string pack_filename = argv[1];
    ifstream fin(pack_filename);
    Pack deck(fin);
    if (!fin.is_open()){
        cout << "Error opening " << pack_filename << endl;
        return 1;
    }

    vector<Player*> players;
    Player * p1 = Player_factory(argv[4],argv[5]);
    Player * p2 = Player_factory(argv[6],argv[7]);
    Player * p3 = Player_factory(argv[8],argv[9]);
    Player * p4 = Player_factory(argv[10],argv[11]);
    players.push_back(p1);
    players.push_back(p2);
    players.push_back(p3);
    players.push_back(p4);

    cout << "./euchre.exe " << pack_filename << " " << argv[2] << " " << argv[3] 
    << " " << argv[4] << " " << argv[5] << " " << argv[6] << " " << argv[7] << " " 
    << argv[8] << " " << argv[9] << " " << argv[10] << " " << argv[11] << " " << endl;

    Game game(players, deck, shuffle, finish_line_points);
    game.play();
    
    for (int i = 0; i < int(players.size()); ++i) {
        delete players[i];
    }
    return 0;
}