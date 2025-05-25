#include "Game.hpp"

namespace coup{
    Game::Game(const vector<string> &players) :
        _players{},_currentPlayer{nullptr} {
        // check if the number of players is valid
        if(players.size() < MIN_PLAYERS || players.size() > MAX_PLAYERS)
            throw invalid_number_of_players_exception(players.size());

        // sets the players for the game
        for(const string &playerName : players){
            // create a new player pointer for the current player
            Player *newPlayer = nullptr;

            // roll a random role for this player
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> dist(0, 5);
            int randomRole = dist(gen);
            
            switch(randomRole){
                case 0:
                    newPlayer = new Spy(playerName);
                    break;
                case 1:
                    newPlayer = new Merchant(playerName);
                    break;
                case 2:
                    newPlayer = new Judge(playerName);
                    break;
                case 3:
                    newPlayer = new Governor(playerName);
                    break;
                case 4:
                    newPlayer = new General(playerName);
                    break;
                case 5:
                    newPlayer = new Baron(playerName);
                    break;
            }

            // adds the player to the vector of players
            (this->_players).push_back(newPlayer);
        }

        // sets the current player to the first player
        (this->_currentPlayer) = _players[0];

        // start the first players turn
        this->_currentPlayer->prepareForTurn();
    }

    Game::~Game(){
        // deletes all the players remaining in the game
        for(int i = 0; i< (this->_players).size(); i++){
            // delete the player
            delete (this->_players)[i];

            // set the pointer to null
            (this->_players)[i] = nullptr;
        }
    }

    vector<string> Game::players() const {
        // creates a vector of strings to hold the names of the players
        vector<string> playersNames;
        // goes through all the players in the game
        for(int i = 0; i < (this->_players).size(); i++){
            // adds the name of the player to the vector
            playersNames.push_back((this->_players[i])->getName());
        }
        // returns the vector of player names
        return playersNames;
    }

    string Game::turn() const {
        // returns the name of the current player
        return (this->_currentPlayer)->getName();
    }

    string Game::role() const {
        // returns the role of the current player
        return (this->_currentPlayer)->getRole();
    }

    vector<string> Game::info() const {
        // creates a vector of strings to hold the info of the current player
        vector<string> playerInfo;
        // adds the name of the player to the vector
        playerInfo.push_back((this->_currentPlayer)->getName());
        // adds the role of the player to the vector
        playerInfo.push_back((this->_currentPlayer)->getRole());
        // adds the number of coins the player has to the vector
        playerInfo.push_back(to_string((this->_currentPlayer)->coins()));
        // adds the number of actions left for this player to perform in this turn
        playerInfo.push_back(to_string((this->_currentPlayer)->getRemainingActions()));
        
        // returns the vector of player info
        return playerInfo;
    }

    int Game::playAction(const string &action, Player *target, bool undone){
        // get the current players turn
        Player *currentPlayer = (this->_players)[currentPlayerIndex()];
        // check if the action is valid for the current player
        if(!currentPlayer->isValidAction(action))
            throw invalid_action_exception(action, currentPlayer->getName());

        // check if the player wants to end their turn
        if(action == "endTurn"){
            // end the turn
            currentPlayer->endTurn();
            // advance the game to the next player
            this->nextTurn();
            // if the action was to end the turn, return true (-1)
            return -1;
        }
        
        // perform the appropriate action for the player
        if(action == "gather")
            currentPlayer->gather();
        if(action == "tax")
            currentPlayer->tax();
        if(action == "bribe"){
            currentPlayer->bribe();
        }
        if(action == "arrest"){
            // check if the target player is valid
            if(target == nullptr)
                throw player_not_in_game_exception(target->getName());

            // try to arrest the target player
            currentPlayer->arrest(*target);
        } 
        if(action == "sanction"){
            // check if the target player is valid
            if(target == nullptr)
                throw player_not_in_game_exception(target->getName());

            // try to sanction the target player
            currentPlayer->sanction(*target);
        }
        if(action == "coup"){
            // check if the target player is valid
            if(target == nullptr)
                throw player_not_in_game_exception(target->getName());

            // try to coup the target player
            currentPlayer->coup(*target, undone);
            // if the coup was successful (meaning no exceptions were thrown)
            // remove the target player from the game
            this->removePlayer(*target);
        }
        
        // check if the player wants to do a specialized action
        // no need to check that the player is of the required roles for these actions,
        // as this is done earlier in the function
        if(action == "invest"){ 
            Baron *baron = (Baron *)currentPlayer;
            baron->invest();
        }
        if(action == "spyOn"){
            Spy *spy = (Spy *)currentPlayer;
            return spy->spyOn(*target);
        }
        if(action == "blockArrest"){
            Spy *spy = (Spy *)currentPlayer;
            spy->blockArrest(*target);
        }

        // if the action was not to end the turn, return false (-2)
        return -2;
    }

    void Game::undoAction(const Player& player, const string &undoAction){
        // check if the action is valid for the player
        if(!player.isValidUndoAction(undoAction))
            throw invalid_action_exception(undoAction, player.getName());

        // no need to check that the player is of the required roles for these actions,
        // as this is done earlier in the function
        if(undoAction == "undoBribe"){
            Judge *judge = (Judge *)&player;
            judge->undoBribe(*this->_currentPlayer);
        }
        if(undoAction == "undoTax"){
            Governor *governor = (Governor *)&player;
            governor->undoTax(*this->_currentPlayer);
        }
        if(undoAction == "undoCoup"){
            General *general = (General *)&player;
            general->undoCoup(*this->_currentPlayer);
        }
    }

    void Game::nextTurn(){
        // advance the game to the next player
        int nextPlayerIndex = this->nextPlayerIndex();

        // gets the pointer to the next player
        Player *nextPlayer = (this->_players)[nextPlayerIndex];

        // sets the current player to the next player
        this->_currentPlayer = nextPlayer;

        // prepare the next player for their turn
        this->_currentPlayer->prepareForTurn();

        // returns
        return;
    }

    bool Game::isValidAction(const string &action) const{
        // check if the player has at least 10 coins
        if(this->_currentPlayer->coins() >= 10){
            // check if the player has an action remaining
            if(!this->_currentPlayer->isOutOfActions()){
                if(action != "coup") // if they have an action remaining, they must coup another player
                    return false;
                else
                    return true; 
            }
        }

        // check if the action is valid for the current player
        return this->_currentPlayer->isValidAction(action);
    }

    vector<Player *> Game::allOfRole(const string &role) const{
        // create a vector to hold the players of the specified role
        vector<Player *> playersOfRole;
        // go through all the players in the game
        for(int i = 0; i < (this->_players).size(); i++){
            // check if the player is the current player
            if((this->_players)[i] == this->_currentPlayer)
                continue; // skips the current player automatically

            // check if the player is of the specified role
            if((this->_players)[i]->getRole() == role){
                // add the player to the vector
                playersOfRole.push_back((this->_players)[i]);
            }
        }

        // returns the vector of players of the specified role
        return playersOfRole;
    }

    int Game::playersCount() const{
        // returns the number of players in the game
        return (this->_players).size();
    }

    string Game::winner() const{
        // check if the game is over
        if(!this->isGameOver())
            throw game_not_over_exception();

        // return the name of the winner
        return (this->_currentPlayer)->getName();
    }

    Player* Game::getPlayerByIndex(int index) const{
        // check if the index is out of bounds
        if(index < 0 || index >= (this->_players).size())
            throw std::out_of_range("index out of bounds");

        // return the player at the specified index
        return (this->_players)[index];
    }

    bool Game::isTargetRequired(const string &action) const{
        // check if the action is valid for the current player
        if(!this->isValidAction(action))
            throw invalid_action_exception(action, this->_currentPlayer->getName());

        // check if it is a basic action that requires a target player
        if(action == "arrest" || action == "sanction" || action == "coup")
            return true; 

        // check if it is a specialized action that requires a target player
        if(action == "blockCoup" || action == "blockArrest" || action == "spyOn" || \
           action == "undoBribe" || action == "undoTax")
            return true;

        // returns false if the action does not require a target player
        return false;
    }

    bool Game::isValidCoup(const Player *target) const{
        // if target is null, return false
        if(target == nullptr)
            return false;

        // check if the current player can coup the target player
        if(this->_currentPlayer->canCoup(*target)){
            // if the current player can coup the target player, set their last action to "coup"
            // since it might be getting undone later
            this->_currentPlayer->_lastAction = "coup";
            
            return true;
        }
        return false; // if the current player cannot coup the target player, return false
    }

    void Game::printValidActions() const{
        // prints the actions available to the current player
        cout << "Available actions for " << this->_currentPlayer->getName() << ": " << endl;
        // gets the valid actions for the current player
        vector<string> validActions = this->_currentPlayer->getValidActions();
        // prints the actions
        for(const string &action : validActions){
            cout << action << endl;
        }
    }

    vector<string> Game::validActions() const{
        // returns the valid actions for the current player
        return this->_currentPlayer->getValidActions();
    }

    int Game::currentPlayerIndex() const{
        // finds the index of the current player
        for(int i = 0; i < (this->_players).size(); i++){
            // check if the player is the current player
            if((this->_players)[i] == this->_currentPlayer)
                return i; // returns the index of the current player
        }

        // if the current player is not found, throw an exception
        throw current_player_not_found_exception();
    }

    int Game::nextPlayerIndex() const{
        // gets the index of the current player
        int currentPlayerIndex = this->currentPlayerIndex();

        // returns the index of the next player
        return (currentPlayerIndex + 1) % (this->_players).size();
    }

    void Game::removePlayer(const Player &player){
        if(this->_currentPlayer == &player)
            throw deleting_current_player_exception(player.getName());

        // check if the player is in the game
        if(!this->isPlayerInGame(player))
            throw player_not_in_game_exception(player.getName());

        // remove the player from the game
        for(int i = 0; i < (this->_players).size(); i++){
            // check if the player's last arrested player is the one to be removed
            const Player *lastArrested = (this->_players)[i]->getLastArrested();
            // check if the last arrested player is the one to be removed
            if(lastArrested == &player)
                (this->_players[i])->clearLastArrested(); // clears the last arrested player

            // check if the player is the one to be removed
            if((this->_players)[i] == &player){
                // gets a pointer to the player to be removed
                Player *temp = (this->_players)[i];
                // remove the player from the vector
                (this->_players).erase(this->_players.begin() + i);

                // delete the player
                delete temp;
                // ends the loop after deletion
                break;
            }
        }
    }

    bool Game::isPlayerInGame(const Player &player) const{
        // check if the player is in the game
        for(int i = 0; i < (this->_players).size(); i++){
            // check if the player is the one to be removed
            if((this->_players)[i] == &player)
                return true; // returns true if the player is in the game
        }

        // returns false if the player is not in the game
        return false;
    }

    bool Game::isGameOver() const{
        // check if the game is over
        if(this->_players.size() == 1)
            return true; // returns true one player remains
        return false; // the game is not over if there is more than one player
    }
}