#include "Pack.hpp"
#include "Player.hpp"
#include "Card.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>

using namespace std;

class Game {
public:
  Game(Pack &packIn, bool doShuffleIn, int pointsToWinIn,
       const vector<Player*> &playersIn)
    : pack(packIn),
      doShuffle(doShuffleIn),
      pointsToWin(pointsToWinIn),
      players(playersIn) {
    dealer = 0;
    handNum = 0;
    team0Points = 0;
    team1Points = 0;
  }

  void play() {
    while (team0Points < pointsToWin && team1Points < pointsToWin) {
      startHand(); 

      Suit trump = SPADES; 
      int maker = -1; 

      bool made = make_trump(trump, maker);
      assert(made); 

      playTricks(trump, maker); 
      finishHand(trump, maker);

      dealer = nextPlayer(dealer);
      handNum++;
    }
    printGameWinners();
  }

private:
  Pack &pack;
  bool doShuffle;
  int pointsToWin;
  vector<Player*> players;

  int dealer;
  int handNum;

  int team0Points;
  int team1Points;

  Card upcard = Card();

  int nextPlayer(int idx) const {
    return (idx + 1) % 4;
  }

  int teamOf(int idx) const {
    return idx % 2;
  }

  void shuffleOrReset() {
    if (doShuffle) {
      pack.shuffle();
    } 
    else {
      pack.reset();
    }
  }

  void startHand() {
    cout << "Hand " << handNum << endl;
    cout << players[dealer]->get_name() << " deals" << endl;

    shuffleOrReset();
    deal();

    upcard = pack.deal_one();
    cout << upcard << " turned up" << endl;
  }

void deal() {
    int eldest = nextPlayer(dealer);

    int batch[8] = {3, 2, 3, 2, 2, 3, 2, 3};
    int who = eldest;

    for (int b = 0; b < 8; b++) {
      for (int c = 0; c < batch[b]; c++) {
        players[who]->add_card(pack.deal_one());
      }
      who = nextPlayer(who);
    }
  }

bool make_trump(Suit &trumpOut, int &makerOut) {
    int eldest = nextPlayer(dealer);

    // round 1
    int who = eldest;
    for (int turn = 0; turn < 4; turn++) {
      bool isDealer = false;
      if (who == dealer) {
        isDealer = true;
      }

      Suit orderSuit = trumpOut; 
      bool ordered = players[who]->make_trump(upcard, isDealer, 1, orderSuit);

      if (ordered) {
        trumpOut = orderSuit;
        makerOut = who;
        cout << players[who]->get_name() << " orders up " << trumpOut << endl;
        players[dealer]->add_and_discard(upcard);
        cout << endl;
        return true;
      }
      else {
        cout << players[who]->get_name() << " passes" << endl;
      }

      who = nextPlayer(who);
    }

    // round 2
    who = eldest;
    for (int turn = 0; turn < 4; turn++) {
      bool isDealer = false;
      if (who == dealer) {
        isDealer = true;
      }

      Suit orderSuit = trumpOut;
      bool ordered = players[who]->make_trump(upcard, isDealer, 2, orderSuit);

      if (ordered) {
        trumpOut = orderSuit;
        makerOut = who;
        cout << players[who]->get_name() << " orders up " << trumpOut << endl;
        cout << endl;
        return true;
      }
      else {
        cout << players[who]->get_name() << " passes" << endl;
      }

      who = nextPlayer(who);
    }
    return false;
  }

  void playTricks(Suit trump, int maker) {
    int leader = nextPlayer(dealer);

    int team0Tricks = 0;
    int team1Tricks = 0;

    for (int trick = 0; trick < 5; trick++) {
      vector<Card> played(4);
      vector<int> whoPlayed(4);

      Card led = players[leader]->lead_card(trump);
      played[0] = led;
      whoPlayed[0] = leader;
      cout << led << " led by " << players[leader]->get_name() << endl;

      int who = nextPlayer(leader);
      for (int k = 1; k < 4; k++) {
        Card c = players[who]->play_card(led, trump);
        played[k] = c;
        whoPlayed[k] = who;
        cout << c << " played by " << players[who]->get_name() << endl;
        who = nextPlayer(who);
      }

      int best = 0;
      for (int k = 1; k < 4; k++) {
        if (Card_less(played[best], played[k], led, trump)) {
          best = k;
        }
      }

      int winner = whoPlayed[best];
      cout << players[winner]->get_name() << " takes the trick" << endl;
      cout << endl;

      if (teamOf(winner) == 0) {
        team0Tricks++;
      }
      else {
        team1Tricks++;
      }
      leader = winner;
    }

    lastTeam0Tricks = team0Tricks;
    lastTeam1Tricks = team1Tricks;

  }

  int lastTeam0Tricks = 0;
  int lastTeam1Tricks = 0;

  void finishHand(Suit, int maker) {
    int makerTeam = teamOf(maker);
    
    int makerTricks;

    if (makerTeam == 0) {
      makerTricks = lastTeam0Tricks;
    }
    else {
      makerTricks = lastTeam1Tricks;
    }

    int points = 0;
    bool march = false;
    bool euchred = false;

    if (makerTricks >= 3) {
      if (makerTricks == 5) {
        points = 2;
        march = true;
      } 
      else {
        points = 1;
      }
      addPoints(makerTeam, points);
      printHandWinners(makerTeam);
    } 
    else {
      points = 2;
      euchred = true;
      addPoints(1 - makerTeam, points);
      printHandWinners(1 - makerTeam);
    }

    if (march) {
      cout << "march!" << endl;
    } 
    else if (euchred) {
      cout << "euchred!" << endl;
    }

    printScore();
    cout << endl;
  }

  void addPoints(int team, int points) {
    if (team == 0) {
        team0Points += points;
    }
    else {
        team1Points += points;
    }
  }

  void printHandWinners(int team) {
    if (team == 0) {
      cout << players[0]->get_name() << " and " << players[2]->get_name()
           << " win the hand" << endl;
    } 
    else {
      cout << players[1]->get_name() << " and " << players[3]->get_name()
           << " win the hand" << endl;
    }
  }

  void printScore() {
    cout << players[0]->get_name() << " and " << players[2]->get_name()
         << " have " << team0Points << " points" << endl;

    cout << players[1]->get_name() << " and " << players[3]->get_name()
         << " have " << team1Points << " points" << endl;
  }

  void printGameWinners() {
    if (team0Points >= pointsToWin) {
      cout << players[0]->get_name() << " and " << players[2]->get_name()
           << " win!" << endl;
    } 
    else {
      cout << players[1]->get_name() << " and " << players[3]->get_name()
           << " win!" << endl;
    }
  }
};

//static void printUsage() {
//  cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
//  << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
//  << "NAME4 TYPE4" << endl;
//}

//int main(int argc, char **argv) {
  // Read command line args and check for errors
  //Game game(/* game details */);
  //game.play();
//}
