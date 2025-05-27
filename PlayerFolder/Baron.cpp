#include "Baron.hpp"
namespace coup{
    Baron::Baron(const string &name): Player(name, "Baron") {
        // nothing to do here
    }

    Baron::~Baron() {
        // nothing to do here
    }

    void Baron::invest(){
        int priceOfInvestment = 3; // the price of the invest action

        // check if the player has enough actions left
        if(this->_remainingActions == 0)
            throw out_of_actions_exception(this->_name);

        // checks if the player has enough coins
        if(this->_coins < priceOfInvestment)
            throw not_enough_coins_exception("invest", priceOfInvestment);

        // perform the invest action
        this->addCoins(priceOfInvestment); // increase the coin count by 3

        // remove an action from the player
        this->_remainingActions--;
    }
}