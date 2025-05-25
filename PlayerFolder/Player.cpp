#include "Player.hpp"

namespace coup{
    Player::Player(const string &name, const string &role): 
        _name{name}, _role{role}, _coins{0}, 
        _remainingActions{0}, _lastArrested{nullptr}, _blockedActions(BLOCKABLE_ACTIONS, 0),
        _lastAction{""} {
        // nothing to do in the actual constructor
    }

    Player::Player(const Player &other): 
        _name{other._name}, _role{other._role}, _coins{other._coins}, 
        _remainingActions{other._remainingActions}, _lastArrested{other._lastArrested},
         _blockedActions(other._blockedActions), _lastAction{other._lastAction} {
        // nothing to do in the copy constructor
    }

    Player::~Player() {
        // sets lastArrested to null to avoid a pointer to a deleted object
        this->_lastArrested = nullptr; 
    }

    string Player::getName() const {
        // return the name of the player
        return this->_name; 
    }

    string Player::getRole() const {
        // return the role of the player
        return this->_role; 
    }

    int Player::coins() const {
        // return the number of coins the player has
        return this->_coins; 
    }

    unsigned int Player::getRemainingActions() const {
        // return the number of actions left for the player to perform in this turn
        return this->_remainingActions; 
    }

    const Player* Player::getLastArrested() const {
        // return the last player arrested by this player
        return this->_lastArrested; 
    }

    bool Player::isOutOfActions() const {
        // check if the player has any more actions left
        return this->_remainingActions == 0; 
    }

    bool Player::canCoup(const Player &target) const{
        int coupCost = 7; // the price of a coup is 7 coins

        // check if the player has an action remaining
        if(this->isOutOfActions())
            return false; // the player cannot coup if they don't have an action remaining

        // check if the player has enough coins for this action
        if(this->coins() < coupCost)
            return false; // the player cannot coup if they don't have enough coins
    
        // check if the player is attempting to coup themselves
        if(this == &target)
            return false; // the player cannot coup themselves

        // if the three checks were passed, the player can coup the target
        return true;
    }

    vector<string> Player::getValidActions() const{
        // creates the vector with the actions available to all players
        vector<string> validActions{"gather", "tax", "bribe", "arrest", "sanction", "coup", "endTurn"}; 
        // check the player's role
        if(this->getRole() == "Baron"){
            // adds the invest action to the vector
            validActions.push_back("invest");
        }
        else if(this->getRole() == "Spy"){
            // adds the spyOn and blockArrest actions to the vector
            validActions.push_back("spyOn");
            validActions.push_back("blockArrest");
        }
        return validActions;
    }

    vector<string> Player::getValidUndoActions() const{
        // create a vector to hold the valid undo actions
        vector<string> validUndoActions{}; 

        // check the player's role
        if(this->getRole() == "General"){
            // add the undoCoup action to the vector
            validUndoActions.push_back("undoCoup");
        }
        if(this->getRole() == "Judge"){
            // add the undoBribe action to the vector
            validUndoActions.push_back("undoBribe");
        }
        if(this->getRole() == "Governor"){
            // add the undoTax action to the vector
            validUndoActions.push_back("undoTax");
        }
        // any other player role does not have any undo actions
        return validUndoActions;
    }

    const string& Player::getLastAction() const {
        // returns the last action performed by this player
        return this->_lastAction; 
    }

    void Player::gather(){
        // check if the playe has an action remaining
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->_name);
        // check if the gather action is blocked for this player
        if(this->isGatherBlocked())
            throw blocked_action_exception("gather");
        
        // adds 1 coin to this player
        this->_coins += 1;
        // decrease the actions left for this player by 1
        this->_remainingActions--;

        // set the last action to gather
        this->_lastAction = "gather";
    }

    void Player::tax(){
        // check if the player has an action remaining
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->_name);
        // check if the tax action is blocked for this player
        if(this->isTaxBlocked())
            throw blocked_action_exception("tax");
        
        // adds 2 coins to this player
        this->_coins += 2;
        // decrease the actions left for this player by 1
        this->_remainingActions--;

        // set the last action to tax
        this->_lastAction = "tax";
    }

    void Player::bribe(){
        int bribeCost = 4; // the cost of the bribe action is 4 coins
        // check if the player

        // check if the player has enough coins for this action
        if(this->_coins < bribeCost)
            throw not_enough_coins_exception("bribe", bribeCost);
        
        // removes 4 coins from this player
        this->_coins -= bribeCost;

        // increase the actions left for this player by 1
        this->_remainingActions++;

        // set the last action to bribe
        this->_lastAction = "bribe"; 
    }

    void Player::arrest(Player &other){
        // check that the player has enough actions left
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->_name);

        // check if the arrest action is blocked for this player
        if(this->isArrestBlocked())
            throw blocked_action_exception("arrest");

        // check if this player is attempting to arrest the same player twice in a row
        if(this->_lastArrested == &other)
            throw double_jeopardy_exception(other.getName());
        
        // check if this player is attempting to arrest themselves
        if(this == &other)
            throw illegal_action_on_self_exception("arrest");

        // if all the checks were passed
        // arrests the other player
        
        // the arrest action acts differently depending on the role of other
        // if the other player is a merchant, they have to pay 2 to the bank and nothing to this player
        if(other.getRole() == "Merchant"){
            // removes 2 coins from this player
            other.removeCoins(2);
        }
        // if the other player is a general, they dont pay anything
        else if(other.getRole() == "General"){
            // nothing to do here
        }
        // if the other player has any other role, they pay the fine required
        // if they have enough coins that is
        else if (other.coins() > 0){
            // removes 1 coin from the other player and adds it to this player
            other.removeCoins(1);

            // adds 1 coin to this player
            this->_coins += 1;   
        }

        // sets the last arrested player to the other player
        this->_lastArrested = &other;
        // decrease the actions left for this player by 1
        this->_remainingActions--;  

        // set the last action to arrest
        this->_lastAction = "arrest";
    }

    void Player::sanction(Player &other){
        int sanctionCost = 3; // the price of a sanction is 3 coins

        // check if the player has an action remaining
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->_name);

        // if the other player is a judge, the price of the sanction increases by 1
        if(other.getRole() == "Judge")
            sanctionCost++;
        
        // check if the player has enough coins for this action
        if(this->_coins < sanctionCost)
            throw not_enough_coins_exception("sanction", sanctionCost);
        
        // check if the other player is this player
        if(this == &other)
            throw illegal_action_on_self_exception("sanction");

        // makes the player pay the sanction
        this->_coins -= sanctionCost;

        // if the other player is a baron reimburses them 1 coin
        if(other.getRole() == "Baron"){
            other.addCoins(1);
        }

        // blocks the other player from using economical actions until the end of their next turn
        other.blockGather();
        other.blockTax();

        // decrease the actions left for this player by 1
        this->_remainingActions--;

        // set the last action to sanction
        this->_lastAction = "sanction";
    }

    void Player::coup(const Player &other, bool undone){
        int coupCost = 7; // the price of a coup is 7 coins

        // check if the player has an action remaining
        if(this->isOutOfActions())
            throw out_of_actions_exception(this->_name);

        // check if the player has enough coins for this action
        if(this->_coins < 7)
            throw not_enough_coins_exception("coup", coupCost);

        // check if the player is attempting to coup themselves
        if(this == &other)
            throw illegal_action_on_self_exception("coup");

        // removes the correct amount of coins from this player
        this->_coins -= coupCost;
        // remove an action from the player
        this->_remainingActions--;

        // set the last action to coup
        this->_lastAction = "coup";

        // after removing the coins and the action, check if the coup was undone
        if(undone)
            throw undo_coup_exception(other.getName());

    }

    void Player::endTurn(){
        // removes all actions left for this player
        this->_remainingActions = 0;

        // lifts all blocks on this player
        for(int i = 0; i < BLOCKABLE_ACTIONS; i++){
            this->_blockedActions[i] = 0;
        }
    }

    void Player::prepareForTurn(){
        // gives the player an action
        this->_remainingActions = 1;
    }

    bool Player::isTaxBlocked() const {
        // returns true if the tax action is blocked, else false
        return this->_blockedActions[0];
    }

    bool Player::isGatherBlocked() const {
        // returns true if the gather action is blocked, else false
        return this->_blockedActions[1];
    }

    bool Player::isArrestBlocked() const {
        // returns true if the arrest action is blocked, else false
        return this->_blockedActions[2];
    }

    Player& Player::operator=(const Player &other){
        // check if this is a self assignment
        if(this == &other)
            return *this; // return a reference to this object
        
        // stops the assignment of a player with a different role to avoid splicing and problems
        if(this->getRole() != other.getRole())
            throw illegal_player_assignment_exception(this->_role, other.getRole());

        // copies over the values from the other player
        this->_name = other._name;
        this->_role = other._role;
        this->_coins = other._coins;
        this->_remainingActions = other._remainingActions;
        this->_lastArrested = other._lastArrested;
        this->_lastAction = other._lastAction;
        // copies over the blocked actions
        for(int i = 0; i < BLOCKABLE_ACTIONS; i++){
            this->_blockedActions[i] = other._blockedActions[i];
        }

        // returns a reference to this object
        return *this;
    }

    ostream& operator<<(ostream &out, const Player &player){
        // adds the name of the player
        out << "name: " << player.getName() << endl;
        // adds the role of the player
        out << "role: " << player.getRole() << endl;
        // adds the number of coins the player has 
        out << "coins: " << player.coins() << endl;
        // adds the number of actions left for the player
        out << "actions left: " << player.getRemainingActions() << endl;
        // adds the last action performed by the player
        out << "last action: " << player.getLastAction() << endl;
        // returns the output stream
        return out;
    }

    bool Player::isValidAction(const string &action) const {
        // if the action is a base action any player can perform, return true
        if(action == "gather")
            return true;
        if(action == "tax")
            return true;
        if(action == "bribe")
            return true;
        if(action == "arrest")
            return true;
        if (action == "sanction")
            return true;
        if (action == "coup")
            return true;
        if (action == "endTurn")
            return true;
        
        // if the action is a specialized action, check if the player has the correct role for it
        
        // the invest action is only available to the baron
        if (action == "invest" && this->getRole() == "Baron")
            return true; 
        
        // the spy on action is only available to the spy
        if (action == "spyOn" && this->getRole() == "Spy")
            return true;
        
        // the block arrest action is only available to the spy
        if (action == "blockArrest" && this->getRole() == "Spy")
            return true;

        // returns false if the action is not valid
        return false; 
    }

    bool Player::isValidUndoAction(const string &undoAction) const {
        // if the action is a base action any player can perform, return true
        if(undoAction == "undoBribe" && this->getRole() == "Judge")
            return true;
            
        if(undoAction == "undoTax" && this->getRole() == "Governor")
            return true;

        if(undoAction == "undoCoup" && this->getRole() == "General")
            return true;

        // returns false if the action is not valid
        return false; 
    }

    void Player::addCoins(int amount){
        // check if the amount is negative
        if(amount < 0)
            throw std::invalid_argument("cannot add a negative amount of coins");
        
        // adds coins to the player
        this->_coins += amount;
    }

    void Player::removeCoins(int amount){
        // check if the amount is negative
        if(amount < 0)
            throw std::invalid_argument("cannot remove a negative amount of coins");
        
        // check if the player has enough coins
        if(this->_coins < amount)
            this->_coins = 0; // set the coin count to 0
        else
            this->_coins -= amount; // remove coins from the player
    }

    void Player::clearLastArrested(){
        // sets the last arrested player to null
        this->_lastArrested = nullptr;
    }

    void Player::blockTax(){
        // sets the tax action as blocked
        this->_blockedActions[0] = 1;
    }

    void Player::blockGather(){
        // sets the gather action as blocked
        this->_blockedActions[1] = 1;
    }
    
    void Player::blockArrest(){
        // sets the arrest action as blocked
        this->_blockedActions[2] = 1;
    }
}