#include "Spy.hpp"
namespace coup{
    Spy::Spy(const string &name): Player(name, "Spy") {
        // nothing to do here
    }

    Spy::~Spy() {
        // nothing to do here
    }

    int Spy::spyOn(Player &other){
        // if the player is this player, throw an excpetion
        if(this == &other)
            throw illegal_action_on_self_exception("Spy On");
        
        // returns the number of coins the other player has
        return other.coins();
    }

    void Spy::blockArrest(Player &other){
        // check if the other player is this player
        if(this == &other)
            throw illegal_action_on_self_exception("block the arrest action");
        
        // block the other player from using the arrest action
        other.blockArrest();
    }
}