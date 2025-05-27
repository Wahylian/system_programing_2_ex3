#include "Merchant.hpp"
namespace coup{
    Merchant::Merchant(const string &name): Player(name, "Merchant") {
        // nothing to do here
    }

    Merchant::~Merchant() {
        // nothing to do here
    }

    void Merchant::prepareForTurn(){
        // checks how many coins this player has
        if(this->coins() >= 3)
            this->_coins++;
        // gives this player an action
        this->_remainingActions++;
    }
}