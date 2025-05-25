#include "main.hpp"


void printPlayers(const vector<string> &players){
    for(int i = 0; i < players.size(); i++){
        string playerName = players[i];
        cout << i << ". " << playerName << " " << endl;
    }
}

void printPlayerInfo(const vector<string> &info){
    cout << "Name: " << info[0] << endl;
    cout << "Role: " << info[1] << endl;
    cout << "Coins: " << info[2] << endl;
    cout << "Actions left: " << info[3] << endl;
}

int main(){
    system("clear"); // cleans the console from the command lines
    // create a game with 6 players
    //Game game = Game({"Alice", "Bob", "Charlie", "Dave", "Eve", "Frank"});
    // create a game with 2 players
    Game game = Game({"Alice", "Bob"});

    // print the names of all the players
    cout << "Players in the game: \n";
    printPlayers(game.players());


    // starts to run game
    while(true){
        // if the game is over, announce the winner and exit the loop
        try{
            string winnerName = game.winner();
            cout << "The winner is: " << winnerName << endl;
            break;
        }
        // if it is not over, continue the game
        catch(game_not_over_exception &e){
            // gets the name of the current player
            string currentPlayerName = game.turn();

            // prints the current player
            cout << "Current player: " << currentPlayerName <<  endl;
            
            while(true){
                // prints the current player's info
                printPlayerInfo(game.info());

                // prints the actions available to the current player
                game.printValidActions();

                // gets the action the player wants to perform
                string action;
                cout << "Enter action: ";
                cin >> action;

                // check the action is valid
                while(!game.isValidAction(action)){
                    cout << "Invalid action, please enter a valid action:" << endl;
                    cin >> action;
                }
                Player *targetPlayer = nullptr;
                // check if the action requires a target player
                if(game.isTargetRequired(action)){
                    // gets the target player for the action
                    int targetPlayerIndex;
                    cout << "Enter target player: ";
                    printPlayers(game.players());
                    cin >> targetPlayerIndex;
                    while(targetPlayerIndex < 0 || targetPlayerIndex >= game.playersCount()){
                        cout << "Invalid player index" << endl;
                        cin >> targetPlayerIndex;
                    }
                    // sets the target player
                    targetPlayer = game.getPlayerByIndex(targetPlayerIndex);
                }
                

                // attempt to perform the action
                try{
                    // since in reality the coup is handled by the game itself,
                    // the best way to handle an undo of the coup is to "undo" it before it takes place
                    bool undoCoup = false;
                    // make sure that the coup is valid and can be preformed by the player
                    if(game.isValidCoup(targetPlayer), action == "coup"){
                        // check if there are any generals in the game (except for the current player)
                        vector<Player*> generals = game.allOfRole("General");
                        for(const Player *general : generals){
                            // ask the player if they want to undo the coup
                            cout << general->getName() << " would you want to undo the coup on ";
                            cout << targetPlayer->getName() << "? (y/n): ";
                            string ans;
                            cin >> ans;
                            if(ans == "y"){
                                // undo the coup
                                game.undoAction(*general, "undoCoup");
                                cout << "Coup undone for " << currentPlayerName << endl;
                                // set the undo coup flag to true
                                undoCoup = true;

                                // breaks the loop
                                break;
                            }
                        }
                    }

                    // if the turn is over, break the loop
                    int res = game.playAction(action, targetPlayer, undoCoup); // saves the result of the action
                    if(res == -1){
                        break;
                    }

                    // check if the action provided is an undo-able action
                    if(action == "bribe"){
                        // gets all judges in the game that are not the current player
                        vector<Player*> judges = game.allOfRole("Judge");
                        for(const Player *judge : judges){
                            // ask the player if they want to undo the bribe
                            cout << judge->getName() <<" would you want to undo the bribe for ";
                            cout << currentPlayerName << "? (y/n): ";
                            string ans;
                            cin >> ans;
                            if(ans == "y"){
                                // undo the bribe
                                game.undoAction(*judge, "undoBribe");
                                cout << "Bribe undone for " << currentPlayerName << endl;
                                // breaks the loop
                                break;
                            }
                        }
                    }
                    if(action == "tax"){
                        // gets all the governors in the game that are not the current player
                        vector<Player*> governors = game.allOfRole("Governor");
                        for(const Player *governor : governors){
                            // ask the player if they want to undo the tax
                            cout << governor->getName() << " would you want to undo the tax for ";
                            cout << currentPlayerName << "? (y/n): ";
                            string ans;
                            cin >> ans;
                            if(ans == "y"){
                                // undos the taxation
                                game.undoAction(*governor ,"undoTax");
                                cout << "Tax blocked for " << currentPlayerName << endl;
                                // breaks the loop
                                break;
                            }
                        }
                    }
                    if(action == "spyOn"){
                        // in the case of the spyOn action, res will hold the number of coins the target player has
                        cout << targetPlayer->getName() << " has " << res << " coins" << endl;
                    }

                }
                // the action can fail for multiple reasons
                catch(invalid_action_exception &e){
                    cout << e.what() << endl;
                }
                catch(player_not_in_game_exception &e){
                    cout << e.what() << endl;
                }
                catch(out_of_actions_exception &e){
                    cout << e.what() << endl;
                }
                catch(blocked_action_exception &e){
                    cout << e.what() << endl;
                }
                catch(not_enough_coins_exception &e){
                    cout << e.what() << endl;
                }
                catch(double_jeopardy_exception &e){
                    cout << e.what() << endl;
                }
                catch(illegal_action_on_self_exception &e){
                    cout << e.what() << endl;
                }
                catch(undo_coup_exception &e){
                    cout << e.what() << endl;
                }
            }
            // clear the console
            system("clear");
        }
    }

    

    return 0;
}