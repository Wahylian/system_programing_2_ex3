#pragma once
#include "Player.hpp"
namespace coup{
    class Governor: public Player{
        public:
            // constructor
            Governor(const string &name);

            // destructor
            ~Governor();

            // the governor can take 3 coins from the bank instead of 2
            void tax();

            // the governor can undo another player's tax action
            void undoTax(Player &other) const;
        
        protected:
            // to avoid the class being abstract
            void virtualFunction() {
                // nothing to do here
            }
    };
}