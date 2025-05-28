#pragma once
#include "Player.hpp"
namespace coup{
    class Spy: public Player{
        public:
            // constructor
            Spy(const string &name);

            // destructor
            ~Spy();

            // the spy can see the amount of coins of another player
            // this action costs nothing and doesn't take a turn
            int spyOn(Player &other);

            // the spy can block the arrest action of another player
            // this action costs nothing and doesn't take a turn
            void blockArrest(Player &other);

        protected:
            // to avoid the class being abstract
            void virtualFunction() {
                // nothing to do here
            }
    };
}