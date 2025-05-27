#pragma once
#include "doctest.h"
#include "../PlayerFolder/Player.hpp"
#include "../PlayerFolder/Spy.hpp"
#include "../PlayerFolder/Merchant.hpp"
#include "../PlayerFolder/Judge.hpp"
#include "../PlayerFolder/Governor.hpp"
#include "../PlayerFolder/General.hpp"
#include "../PlayerFolder/Baron.hpp"

using namespace coup;

// a class to allow for the testing of a player
// allows testing of private functions and the basic roles of players without using any special roles
class TestPlayer : public Player {
    public:
        // allows access to the functions through the test class
        using Player::addCoins;
        using Player::removeCoins;
        using Player::blockTax;
        using Player::blockGather;
        using Player::blockArrest;
        using Player::clearLastArrested;

        TestPlayer(const string &name) : Player(name, "testPlayer")
        { 
            // nothing to do here
        }

        ~TestPlayer() {
            // nothing to do here
        }      
    protected:
        // to avoid the class being abstract
        void virtualFunction() {
                // nothing to do here
        }
};