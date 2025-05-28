#pragma once
#include "../CustomExceptions/CustomPlayerExceptions.hpp"
#include <iostream>
#include <vector>
using namespace std;

namespace coup{
    // an abstract class that represents a player in the game
    class Player{
        friend class Player; // allows other players to access the protected methods of this instance of player
        // since all subclasses of player are players too they need access to the protected methods of this instance
        friend class Spy; 
        friend class Merchant; 
        friend class Judge; 
        friend class Governor;
        friend class General;
        friend class Baron;
        friend class Game; // allows the game to access the protected methods of player
        protected:
            string _name; // the name of the player
            string _role; // the role of the player
            int _coins; // the number of coins the player has
            unsigned int _remainingActions; // the number of actions left for the player to perform this turn
            const Player *_lastArrested; // the last player arrested by this player, this player cannot change it
            const int BLOCKABLE_ACTIONS = 3; // there are 3 actions that can be outright blocked
            vector<char> _blockedActions; // these are the actions tax, gather and arrest 
            string _lastAction; // the last action performed by this player, used for undo actions
        public:
            // constructor 
            Player(const string &name, const string &role);

            // copy constructor
            Player(const Player &other);

            // destructor
            virtual ~Player();

            #pragma region Getters
            // gets the name of this player
            string getName() const;

            // gets the role of this player
            string getRole() const;

            // gets the number of coins this player has
            int coins() const;

            // gets the number of actions left for this player to perform in this turn
            unsigned int getRemainingActions() const;

            // gets the last player arrested by this player
            const Player* getLastArrested() const;

            // returns a vector of strings with the actions this player can perform
            vector<string> getValidActions() const;

            // returns a vector of strings with the undo actions this player can perform
            vector<string> getValidUndoActions() const;

            const string& getLastAction() const; 
            #pragma endregion

            // returns true if this player is out of actions, else false
            bool isOutOfActions() const; 

            // returns true if the player can perform the coup action on the target, else false
            bool canCoup(const Player &target) const;

            #pragma region Actions
            // the player receives 1 coin from the bank, this action costs nothing and can be blocked via the "sanction" action
            void gather();

            // the player takes 2 coins from the bank, this action costs nothing but can be blocked via certain roles or actions
            virtual void tax();

            // the player pays 4 coins to preform 2 actions
            void bribe();

            // the player chooses another player and takes a coin from them.
            // it cannot be used on the same player twice in a row
            void arrest(Player &other);

            // the player chooses another player and blocks them from using economical actions (gather and tax) until their next turn.
            // this action costs 3 coins.
            void sanction(Player &other);

            // the player chooses another player and eliminates them from the game completely.
            // this action costs 7 coins and can be blocked under specific conditions.
            // throws nothing on success, on failure will throw an excpetion
            void coup(const Player &other, bool undone = false);

            // the player chooses to end their turn
            // this action removes all actions they may have left as well as lifts any blocks on them
            void endTurn();
            #pragma endregion

            // sets up the player to be able to perform an action
            virtual void prepareForTurn();

            #pragma region blockCheckers
            // a check if the tax action is blocked 
            bool isTaxBlocked() const;

            // a check if the gather action is blocked
            bool isGatherBlocked() const;

            // a check if the arrest action is blocked
            bool isArrestBlocked() const;
            #pragma endregion

            // override the assignment operator
            Player& operator=(const Player &other);

            // allows the player to be printed
            friend ostream& operator<<(ostream &out, const Player &player);

            // returns true if the provided action is a possible action for this player
            // NOTE: the undo actions are not included in this check
            bool isValidAction(const string &action) const;

            // returns true if the provided action is a possible action for this player
            bool isValidUndoAction(const string &undoAction) const;

        protected:
            // function used to turn this class into an abstract class
            virtual void virtualFunction() = 0;

            // adds coins to the player
            void addCoins(int amount);

            // remove coins from the player (cannot remove more coins than the player has)
            void removeCoins(int amount);

            // clears the last arrested player of this player
            void clearLastArrested();

            #pragma region block Actions
            // blocks this player from using the tax action
            void blockTax();
            
            // blocks this player from using the gather action
            void blockGather();

            // blocks this player from using the arrest action
            void blockArrest();
            #pragma endregion
    };
}