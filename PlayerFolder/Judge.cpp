#include "Judge.hpp"
namespace coup{
    Judge::Judge(const string &name): Player(name, "Judge") {
        // nothing to do here
    }

    Judge::~Judge() {
        // nothing to do here
    }

    void Judge::undoBribe(Player &other) const{
        // checks if other is this player
        if(this == &other)
            throw illegal_action_on_self_exception("Undo Bribe");

        // check if the other player's last action was a bribe
        if(other.getLastAction() != "bribe")
            throw undo_wrong_action_exception(other.getName(), "bribe", other.getLastAction());

        // removes the additional action from the other player
        other._remainingActions--;
    }

}