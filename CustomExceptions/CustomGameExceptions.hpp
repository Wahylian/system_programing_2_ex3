#pragma once
#include <string>
#include <exception>
#include <stdexcept>

// an exception that is thrown when an invalid amount of players is attempting to play the game
class invalid_number_of_players_exception : public std::invalid_argument{
    public:
        explicit invalid_number_of_players_exception(int playersCount) : 
        std::invalid_argument("There must be between 2 and 6 players in the game, but there are " + std::to_string(playersCount) + " players") 
        {
            
        }
};

// an exception that is thrown when a player tries to perform an action that is not valid
class invalid_action_exception : public std::invalid_argument{
    public:
        explicit invalid_action_exception(const std::string &actionName, const std::string &playerName) : 
        std::invalid_argument("The provided action: " + actionName + " is not a valid action for the player: " + playerName) 
        {
            
        }
};

// an excption that is thrown when a player tries to perform an action on a player that is not in the game
class player_not_in_game_exception : public std::invalid_argument{
    public:
        explicit player_not_in_game_exception(const std::string &playerName) : 
        std::invalid_argument(playerName + " is not a part of the game") 
        {
            
        }
};

// an exception that is thrown when there is attempt to get the winner but there isn't one yet
class game_not_over_exception : public std::runtime_error{
    public:
        explicit game_not_over_exception() : 
        std::runtime_error("The game is not over yet") 
        {
            
        }
};

// an exception that is thrown when the current player is not found
class current_player_not_found_exception : public std::runtime_error{
    public:
        explicit current_player_not_found_exception() : 
        std::runtime_error("The current player is not found") 
        {
            
        }
};

// an exception that is thrown when attempting to delete the current player
class deleting_current_player_exception : public std::invalid_argument{
    public:
        explicit deleting_current_player_exception(const std::string &playerName) : 
        std::invalid_argument("Cannot delete the current player: " + playerName) 
        {
            
        }
};

