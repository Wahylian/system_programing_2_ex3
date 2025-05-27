#pragma once
#include <string>
#include <exception>
#include <stdexcept>

// an exception that is thrown when a player tries to perform an action when they run out of actions in this turn
class out_of_actions_exception : public std::runtime_error{
    public:
        explicit out_of_actions_exception(const std::string &playerName) : 
        std::runtime_error(playerName + " is out of actions") 
        {

        }
};

// an exception that is thrown when a player tries to perform a blocked action
class blocked_action_exception : public std::runtime_error{
    public:
        explicit blocked_action_exception(const std::string &actionName) : 
        std::runtime_error("the action " + actionName + " was blocked") 
        {

        }
};

// an exception that is thrown when a player tries to perfrom an action that they cannot afford
class not_enough_coins_exception : public std::runtime_error{
    public:
        explicit not_enough_coins_exception(const std::string &actionName, int cost) : 
        std::runtime_error("not enough coins to perform " + actionName + " need at least " + std::to_string(cost) + " coins") 
        {

        }
};

// an exception that is thrown when a player tries to arrest the same player twice in a row
class double_jeopardy_exception : public std::invalid_argument{
    public:
        explicit double_jeopardy_exception(const std::string &playerName) :
        std::invalid_argument("cannot arrest " + playerName + " twice in a row") 
        {

        }
};

class illegal_action_on_self_exception : public std::invalid_argument{
    public:
        explicit illegal_action_on_self_exception(const std::string &actionName) :
        std::invalid_argument("cannot perform " + actionName + " on self") 
        {

        }
};

class illegal_player_assignment_exception : public std::invalid_argument{
    public:
        explicit illegal_player_assignment_exception(const std::string &className, const std::string &classAssigned) :
        std::invalid_argument("cannot assign " + classAssigned + " to " + className) 
        {

        }
};

class undo_coup_exception : public std::runtime_error{
    public:
        explicit undo_coup_exception(const std::string &playerName) :
        std::runtime_error("the coup on " + playerName + " was undone") 
        {

        }
};

class must_play_coup_exception : public std::runtime_error{
    public:
        explicit must_play_coup_exception(const std::string &playerName) :
        std::runtime_error(playerName + " must attempt a coup on another player") 
        {

        }
};

class undo_wrong_action_exception : public std::runtime_error{
    public:
        explicit undo_wrong_action_exception(const std::string &performer, const std::string &undoName,\
             const std::string &actionName = "") :
        std::runtime_error("last action performed by "+ performer +" was not " + undoName\
             + " it was " + actionName) 
        {

        }
};

class undo_self_action_exception : public std::runtime_error{
    public:
        explicit undo_self_action_exception(const std::string &actionName) :
        std::runtime_error("you cannot undo your own " + actionName + " action") 
        {

        }
};