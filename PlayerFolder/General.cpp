#include "General.hpp"
namespace coup{
    General::General(const string &name): Player(name, "General") {
        // nothing to do here
    }

    General::~General() {
        // nothing to do here
    }

    void General::undoCoup(Player &performer){
        // undo's the coup action performed by the performer on another player
        // check if the performer is this player
        if(this == &performer) // a player cannot undo their own coup action
            throw undo_self_action_exception("coup");
        
        // check that the performer's last action was a coup
        if(performer.getLastAction() != "coup")
            throw undo_wrong_action_exception(performer.getName(), "coup", performer.getLastAction());
    }
}