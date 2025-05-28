#include "Governor.hpp"
namespace coup{
    Governor::Governor(const string &name): Player(name, "Governor") {
        // nothing to do here
    }

    Governor::~Governor() {
        // nothing to do here
    }

    void Governor::tax(){
        int taxAmount = 3; // the amount of coins the governor can tax

        // check if the player has enough actions left
        if(this->_remainingActions == 0)
            throw out_of_actions_exception(this->_name);

        // check if the tax action is blocked
        if(this->isTaxBlocked())
            throw blocked_action_exception("tax");

        this->addCoins(taxAmount); // increase the coin count by 3

        this->_remainingActions--; // decrease the actions left by 1

        this->_lastAction = "tax"; // set the last action to tax
    }

    void Governor::undoTax(Player &other) const{
        // checks if other is this player
        if(this == &other)
            throw undo_self_action_exception("tax");

        int taxAmount = 2; // the amount of coins to remove from other
        if(other.getRole() == "Governor")
            taxAmount = 3; // if other is a governor, remove 3 coins instead of 2

        // check if the other player's last action was a tax
        if(other.getLastAction() != "tax")
            throw undo_wrong_action_exception(other.getName(), "tax", other.getLastAction());
        
        // remove the coins from the other player
        other.removeCoins(taxAmount);
    }
}