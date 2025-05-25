#include "testSpy.hpp"

TEST_CASE("Test Spy Construction"){
    SUBCASE("Test Spy Constructor"){
        Spy s = Spy("John");

        // check that the values are correct
        CHECK(s.getName() == "John");
        CHECK(s.getRole() == "Spy");
        CHECK(s.coins() == 0);
        CHECK(s.getRemainingActions() == 0);
        CHECK(s.getLastArrested() == nullptr);
        CHECK(s.getLastAction() == "");
    }
    SUBCASE("Test Spy Copy Constructor"){
        Spy s1 = Spy("John");
        // give s1 some coins
        s1.prepareForTurn();
        s1.gather();
        s1.prepareForTurn();
        s1.gather();

        // create a copy of s1
        Spy s2 = Spy(s1);

        // check that the values are correct
        CHECK(s2.getName() == s1.getName());
        CHECK(s2.getRole() == s1.getRole());
        CHECK(s2.coins() == s1.coins());
        CHECK(s2.getRemainingActions() == s1.getRemainingActions());
        CHECK(s2.getLastArrested() == s1.getLastArrested());
        CHECK(s2.getLastAction() == s1.getLastAction());

        // check that the players are not the same object
        CHECK(&s1 != &s2);
    }
    SUBCASE("Test Spy Assignment Operator"){
        SUBCASE("Assignment to Self"){
            Spy s = Spy("John");

            // give s some coins
            s.prepareForTurn();
            s.gather();
            s.prepareForTurn();
            s.gather();

            // save the address of s
            void *pS = &s;

            // pragmas to allow for self assignment
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wself-assign-overloaded"
            s = s; // Self-assignment
            #pragma clang diagnostic pop

            // check that the values are correct
            CHECK(s.getName() == "John");
            CHECK(s.getRole() == "Spy");
            CHECK(s.coins() == 2);
            CHECK(s.getRemainingActions() == 0);
            CHECK(s.getLastArrested() == nullptr);
            CHECK(s.getLastAction() == "gather");

            // check that the players are the same object
            CHECK(pS == &s);
        }
        SUBCASE("Assignment to Another Spy"){
            Spy s1 = Spy("John");

            // give s1 some coins
            s1.prepareForTurn();
            s1.gather();
            s1.prepareForTurn();
            s1.gather();

            // assign s1 to s2
            Spy s2 = s1;
            // check that the values are correct
            CHECK(s2.getName() == s1.getName());
            CHECK(s2.getRole() == s1.getRole());
            CHECK(s2.coins() == s1.coins());
            CHECK(s2.getRemainingActions() == s1.getRemainingActions());
            CHECK(s2.getLastArrested() == s1.getLastArrested());
            CHECK(s2.getLastAction() == s1.getLastAction());

            // check that the players are not the same object
            CHECK(&s1 != &s2);
        }
    }
}

TEST_CASE("Test Spy Valid Actions"){
    SUBCASE("Test Spy Valid Actions"){
        Spy s = Spy("John");

        // create a vector of the excpected valid actions of s
        vector<string> expectedValidActions = {"gather", "tax", "bribe", "arrest", "sanction", "coup", "endTurn", "spyOn", "blockArrest"};
    
        // get the valid actions of s
        vector<string> validActions = s.getValidActions();

        // check that both vectors are the same
        CHECK(validActions == expectedValidActions);

        // check that the spy accepts the valid actions
        for(const string &action : validActions){
            CHECK(s.isValidAction(action) == true);
        }

        // check that the spy does not accept actions of any other type of player
        CHECK(!s.isValidAction("undoBribe") == true); // judge special action
        CHECK(!s.isValidAction("invest") == true); // baron special action
        CHECK(!s.isValidAction("undoCoup") == true); // general special action
        CHECK(!s.isValidAction("undoTax") == true); // governor special action

        // check that the spy does not accept any other actions (not real actions)
        CHECK(!s.isValidAction("test") == true);
        CHECK(!s.isValidAction("yatzee") == true);
        CHECK(!s.isValidAction("banana") == true); 
    }
    SUBCASE("Test Spy Valid Undo Actions"){
        Spy s = Spy("John");

        // get a vector of the supposed valid undo actions of the spy
        vector<string> validUndoActions = s.getValidUndoActions();
        
        // should be 0 valid undo actions since the spy has no undo actions
        CHECK(validUndoActions.size() == 0);

        // check that the spy does not accept any undo actions
        CHECK(!s.isValidUndoAction("undoCoup") == true); // general special action
        CHECK(!s.isValidUndoAction("undoBribe") == true); // judge special action
        CHECK(!s.isValidUndoAction("undoTax") == true); // governor special action

        // check that the spy does not accept any other actions as undo actions (not real actions)
        CHECK(!s.isValidUndoAction("test") == true);
        CHECK(!s.isValidUndoAction("yatzee") == true);
        CHECK(!s.isValidUndoAction("banana") == true);
    }
}

TEST_CASE("Test Spy Special Interactions"){
    SUBCASE("Test Spy On"){
        SUBCASE("Test Spy On Self"){
            Spy s = Spy("John");

            // give s some coins
            s.prepareForTurn();
            s.gather();
            s.prepareForTurn();
            s.gather();

            // check that the spy has 2 coins
            CHECK(s.coins() == 2);

            // attempt to spy on self
            CHECK_THROWS(s.spyOn(s));

            // check that the spy still has 2 coins
            CHECK(s.coins() == 2);
        }
        SUBCASE("Test Spy On Another Player"){
            Spy s1 = Spy("John");
            Spy s2 = Spy("Doe");

            // give s2 some coins
            s2.prepareForTurn();
            s2.gather();
            s2.prepareForTurn();
            s2.gather();

            // check that s2 has 2 coins
            CHECK(s2.coins() == 2);

            // attempt to spy on s2
            int coins = s1.spyOn(s2);

            // check that the spy got the correct amount of coins
            CHECK(coins == s2.coins());
        }
    }
    SUBCASE("Test Block Arrest"){
        SUBCASE("Test Block Arrest on Self"){
            Spy s = Spy("John");

            // attempt to block arrest on self
            CHECK_THROWS(s.blockArrest(s));

            // check that the spy still has no blocked actions
            CHECK(!s.isArrestBlocked() == true);
        }
        SUBCASE("Test Block Arrest on Another Player"){
            Spy s1 = Spy("John");
            Spy s2 = Spy("Doe");

            // check that s2 has no blocked actions
            CHECK(!s2.isArrestBlocked() == true);

            // attempt to block arrest on s2
            s1.blockArrest(s2);

            // check that the arrest action of s2 is blocked
            CHECK(s2.isArrestBlocked() == true);
        }
    }
}