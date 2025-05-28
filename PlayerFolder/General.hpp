#pragma once
#include "Player.hpp"
namespace coup{
    class General: public Player{
        public:
            // constructor
            General(const string &name);

            // destructor
            ~General();

            // the general can pay 5 coins to undo the performance of the coup action against a player (including themselves).
            void undoCoup(Player &performer);

        protected:    
            // to avoid the class being abstract
            void virtualFunction() {
                // nothing to do here
            }
    };
}