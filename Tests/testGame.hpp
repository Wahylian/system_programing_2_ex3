#pragma once
#include "doctest.h"
#include "../Game.hpp"
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
// to be able to use the testPlayer class if needed
#include "testPlayer.hpp"

using namespace coup;
using namespace std;

// a class to allow for the testing of a game
// allows testing of private functions and the parameters of the game
class TestGame : public Game {
    public:
        // allows access to the functions through the test class
        using Game::currentPlayerIndex;
        using Game::nextPlayerIndex;
        using Game::removePlayer;
        using Game::isPlayerInGame;
        using Game::isGameOver;
        using Game::getPlayers;


        TestGame(const vector<string> &players) : Game(players)
        { 
            // nothing to do here
        }

        // creates a game with players of specific roles
        // this is a constructor only for testing purposes
        TestGame(const vector<string> &players, const vector<string> &roles) : Game({"dummy1", "dummy2"})
        {
            // makes sure the number of players and roles match
            if(players.size() != roles.size()) {
                throw invalid_argument("Number of players and roles must match");
            }

            // makes sure the number of players is within the limits
            if(players.size() < this->MIN_PLAYERS || players.size() > this->MAX_PLAYERS) {
                throw invalid_number_of_players_exception(players.size());
            }

            // sets the current player to null
            this->_currentPlayer = nullptr;

            // delete all the players in the game
            while(this->_players.size() > 0) {
                // gets a pointer to the last player in the vector
                Player *player = this->_players.back();

                // removes the last player from the vector
                this->_players.pop_back();

                // deletes the player
                delete player;

                // sets the pointer to null
                player = nullptr;
            }
            
            // create players with the provided roles
            for(int i = 0; i < players.size(); i++) {
                Player *newPlayer = nullptr;

                // get the player name and role
                string playerName = players[i];
                string playerRole = roles[i];

                // create a new player with the provided role
                if(playerRole == "Spy") {
                    newPlayer = new Spy(playerName);
                } else if(playerRole  == "Merchant") {
                    newPlayer = new Merchant(playerName);
                } else if(playerRole  == "Judge") {
                    newPlayer = new Judge(playerName);
                } else if(playerRole  == "Governor") {
                    newPlayer = new Governor(playerName);
                } else if(playerRole  == "General") {
                    newPlayer = new General(playerName);
                } else if(playerRole == "Baron") {
                    newPlayer = new Baron(playerName);
                } else {
                    throw invalid_argument("Unknown role: " + playerRole);
                }

                // adds the player to the vector of players
                this->_players.push_back(newPlayer);
            }

            // sets the current player to the first player
            this->_currentPlayer = this->_players[0];
            
            // start the first players turn
            this->_currentPlayer->prepareForTurn();
        }

        ~TestGame() {
            // nothing to do here
        }
};