#pragma once
#include "Player.hpp"
namespace coup{
    class Merchant: public Player{
        public:
            // constructor
            Merchant(const string &name);

            // destructor
            ~Merchant();
            
            // if the merchant has at least 3 coins at the begining of their turn they receive 
            // an additional coin at the beginning of their turn
            void prepareForTurn();
        
        protected:
            // to avoid the class being abstract
            void virtualFunction() {
                // nothing to do here
            }
    };
}