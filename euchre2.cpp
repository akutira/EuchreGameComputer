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
        Pack pack;
        Suit trump;
        int even_points;
        int odd_points;
        bool even_order_up;
        bool odd_order_up;
        int points_to_win;
        bool shuffle;

    public:

        Game(vector<Player*> playerin, int ptw, Pack card, bool shuff){
            this->players = playerin;
            this->points_to_win = ptw;
            this->pack = card;
            this->shuffle = shuff;
            this->even_points = 0;
            this->odd_points = 0;
            this->even_order_up = false;
            this->odd_order_up = false;
        }
        void play(){
            Player* dealer = players[0];
            Player* leader = players[1];
            bool winner = false;
            int handcounter = 0;
            while (!winner) {
                    if (shuffle) {
                this->pack.shuffle();
                }
                else {
                this->pack.reset();
                }
            this->odd_order_up = false;
            this->even_order_up = false;

                cout<<"Hand "<<handcounter<<endl;
                deal(dealer);

                Card upCard = this->pack.deal_one();
                cout<<upCard<<" turned up"<<endl;
                bool if_trump = make_trump(1, upCard, dealer);
                if (if_trump){
                    dealer->add_and_discard(upCard);
                }
                else if (!if_trump) {
                    make_trump(2, upCard, dealer);
                }
                winner = take_trick(leader);
                dealer = next_player(dealer);
                leader = next_player(leader);
                handcounter++;
            }
        }
        void set_up_table(){
            if (shuffle) {
                this->pack.shuffle();
            }
            else {
                this->pack.reset();
            }
            // this->odd_order_up = false;
            // this->even_order_up = false;
        }
        void deal(Player * dealer){
            cout<<dealer->get_name()<<" deals"<<endl;
            Player* player = next_player(dealer);
            int dealamount = 3;
            for(int i = 0; i < 4; ++i){
                for (int i = 0; i < dealamount; i++){
                    player->add_card(pack.deal_one());
                }
                if (dealamount == 3) { 
                    dealamount = 2;
                    }
                else {dealamount = 3;}
                player = next_player(player);
            }
            dealamount = 2;
            for(int i = 0; i < 4; ++i){
                for (int i = 0; i < dealamount; i++){
                    player->add_card(pack.deal_one());
                }
                if (dealamount == 3){ dealamount = 2;}
                else {dealamount = 3;}
                player = next_player(player);
            }
        }
        //         void deal(Player* dealer) {
//     int round = 1;
//     cout << dealer->get_name() << " deals" << endl;
//     Player* player = next_player(dealer);
//     while (round <= 2) {
//     if (round == 1) {
//         int deal_amounts[4] = {2, 3, 3, 2};
//         for (int i = 0; i < 4; i++) {
//             for (int j = 0; j < deal_amounts[i]; j++) {
//                 player->add_card(pack.deal_one());
//             }
//             player = next_player(player);
//         }
//     } else if (round == 2) {
//         int deal_amounts[4] = {2, 3, 2, 3};
//         for (int i = 0; i < 4; i++) {
//             for (int j = 0; j < deal_amounts[i]; j++) {
//                 player->add_card(pack.deal_one());
//             }
//             player = next_player(player);
//         }
//     }
//     round++;
//     }
// }
        bool make_trump(int round, const Card &upcard, Player* dealer){
            assert(round == 1 || round == 2);

            Player* current = next_player(dealer);
            Suit upsuit = upcard.get_suit();
            for (int i = 0; i < 4; i++){  
                bool decision;

                if (dealer == current){
                    decision = current->make_trump(upcard, true, round, upsuit);
                }
                else {
                    decision = current->make_trump(upcard, false, round, upsuit);
                }
                if (decision) { 
                    trump = upsuit;
                    cout << current->get_name() << 
                    " orders up " << trump << endl;
                    cout << endl;
                    if (current == players[0] || current == players[2])
                    {this->odd_order_up = true;}
                    else {this->even_order_up = true;}

                    return true;
                }
                else{
                    cout << current->get_name() << " passes" << endl;
                }
                
                current = next_player(current);
            }
            return false;
        }
        Player* play_trick(Player* leader){
            Card pack[4];
            Card ledcard = leader->lead_card(trump);
            pack[0] = ledcard;
            cout << ledcard << " led by " << leader->get_name() << endl;

            for (int i = 1; i < 4; i++){
                leader = next_player(leader);
                Card nextcard = leader->play_card(ledcard, trump);
                pack[i] = nextcard;
                cout << nextcard << " played by " << leader->get_name() << endl;
            }

            Card maxcard = pack[0];
            Player* maxplayer = next_player(leader); 
            Player* currplayer = next_player(maxplayer);
            for (int i = 1; i < 4; i++){
                if (Card_less(maxcard, pack[i], ledcard, trump)){
                    maxcard = pack[i];
                    maxplayer = currplayer;
                }
                currplayer = next_player(currplayer);
            }
            cout << maxplayer->get_name() << " takes the trick" << endl;
            cout << endl;
            return maxplayer;
        }
        bool take_trick(Player* leader){
            int evencount = 0;
            int oddcount = 0;
            bool win = false;
            for (int i = 0; i < 5; i++){
                Player* winner = play_trick(leader);
                leader = winner;
                if (winner == players[1] || winner == players[3]){
                    evencount++;
                }
                else{
                    oddcount++;
                }
            }
            
            if (evencount > oddcount){
                win = score_trick(evencount, true);
            }
            else{
            win = score_trick(oddcount, false);
            }
            
            cout << players[0]->get_name() << " and " << players[2]->get_name()
            << " have " << odd_points << " points" << endl;

            cout << players[1]->get_name() << " and " << players[3]->get_name()
            << " have " << even_points << " points" << endl;

            cout << endl;

            if (win) {
                if (even_points >= points_to_win){
                    cout << players[1]->get_name() << " and " << 
                    players[3]->get_name() << " win!" << endl;
                }
                else{
                    cout << players[0]->get_name() << " and " << 
                    players[2]->get_name() << " win!" << endl;
                }
            }
            return win;
        }
        bool score_trick(int majority_tricks, bool even_team){
            if (even_team){
                cout << players[1]->get_name() << " and " << 
                players[3]->get_name() << " win the hand" << endl;

                if (!even_order_up){
                    even_points += 2;
                    cout << "euchred!" << endl;
                }
                else if (majority_tricks == 5){
                    even_points += 2;
                    cout << "march!" << endl;
                }
                else{
                    even_points += 1;
                }
            } else {
                cout << players[0]->get_name() << " and " << 
                players[2]->get_name() << " win the hand" << endl;

                if (!odd_order_up){
                    odd_points += 2;
                    cout << "euchred!" << endl;
                }
                else if (majority_tricks == 5){
                    odd_points += 2;
                    cout << "march!" << endl;
                }
                else{
                    odd_points += 1;
                }
            }

            if (even_points >= points_to_win || odd_points >= points_to_win) {
                return true;
            } else {
                return false;}
        }
        Player* next_player(Player* player){
            if (player == players[3]){
                return players[0];
            }
            else if (player == players[0]){
                return players[1];
            }
            else if (player == players[1]) {
                return players[2];
            }
            else{
                return players[3];
            }
        }
};

int main(int argc, char *argv[]) {
    int ptw = atoi(argv[3]);
    bool shuffle = false;
    if (strcmp(argv[2], "shuffle") == 0){ shuffle = true; }
    if (argc != 12) { 
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 1;
    }
    if (ptw < 1 || ptw > 100){
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
    if (!fin.is_open()){
        cout << "Error opening " << pack_filename << endl;
        return 1;
    }

    cout << "./euchre.exe " << pack_filename << " " 
    << argv[2] << " " << argv[3] << " " << argv[4] << " " 
    << argv[5] << " " << argv[6] << " " << argv[7] << " " 
    << argv[8] << " " << argv[9] << " " << argv[10] << " "
     << argv[11] << " " << endl;

    vector<Player*> players;
    Pack pack(fin);
    Player * p1 = Player_factory(argv[4],argv[5]);
    Player * p2 = Player_factory(argv[6],argv[7]);
    Player * p3 = Player_factory(argv[8],argv[9]);
    Player * p4 = Player_factory(argv[10],argv[11]);
    players.push_back(p1);
    players.push_back(p2);
    players.push_back(p3);
    players.push_back(p4);

    Game game(players, ptw, pack, shuffle);
    game.play();
    
    for (int i = 0; i < int(players.size()); ++i) {
        delete players[i];
    }
    return 0;
}
