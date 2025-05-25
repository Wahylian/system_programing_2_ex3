#include "testBaron.hpp"

TEST_CASE("Test Baron Construction"){
    SUBCASE("Test Baron Constructor"){
        Baron b = Baron("John");
        
        // check that the values are correct
        CHECK(b.getName() == "John");
        CHECK(b.getRole() == "Baron");
        CHECK(b.coins() == 0);
        CHECK(b.getRemainingActions() == 0);
        CHECK(b.getLastArrested() == nullptr);
        CHECK(b.getLastAction() == ""); 
    }
    SUBCASE("Test Baron Copy Constructor"){
        Baron b1 = Baron("John");
        // give b1 coins
        b1.prepareForTurn();
        b1.tax();
        b1.prepareForTurn();
        b1.gather();

        // create a copy of b1
        Baron b2 = Baron(b1);

        // check that the values are correct
        CHECK(b2.getName() == b1.getName());
        CHECK(b2.getRole() == b1.getRole());
        CHECK(b2.coins() == b1.coins());
        CHECK(b2.getRemainingActions() == b1.getRemainingActions());
        CHECK(b2.getLastArrested() == b1.getLastArrested());
        CHECK(b2.getLastAction() == b1.getLastAction());

        // check that the players are not the same object
        CHECK(&b1 != &b2);
    }
    SUBCASE("Test Baron Assignment Operator"){
        SUBCASE("Assignment to Self"){
            Baron b1 = Baron("John");
            // give b1 coins
            b1.prepareForTurn();
            b1.tax();
            b1.prepareForTurn();
            b1.gather();

            // save the address of b1
            void *pB1 = &b1;

            // pragmas to allow for self assignment
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wself-assign-overloaded"
            b1 = b1; // Self-assignment
            #pragma clang diagnostic pop

            // check that the values are correct
            CHECK(b1.getName() == "John");
            CHECK(b1.getRole() == "Baron");
            CHECK(b1.coins() == 3);
            CHECK(b1.getRemainingActions() == 0);
            CHECK(b1.getLastArrested() == nullptr);
            CHECK(b1.getLastAction() == "gather"); 
        
            // check that the player is saved in the same address
            CHECK(&b1 == pB1);
        }
        SUBCASE("Assignment to Other"){
            Baron b1 = Baron("John");
            // give b1 coins
            b1.prepareForTurn();
            b1.tax();
            b1.prepareForTurn();
            b1.gather();

            // assign b1 to b2
            Baron b2 = b1;

            // check that the values are correct
            CHECK(b2.getName() == b1.getName());
            CHECK(b2.getRole() == b1.getRole());
            CHECK(b2.coins() == b1.coins());
            CHECK(b2.getRemainingActions() == b1.getRemainingActions());
            CHECK(b2.getLastArrested() == b1.getLastArrested());
            CHECK(b2.getLastAction() == b1.getLastAction());
            // check that the players are not the same object
            CHECK(&b1 != &b2);
        }
    }
}

TEST_CASE("Test Baron Valid Actions"){
    SUBCASE("Test Baron Valid Actions"){
        Baron b = Baron("John");

        // create a list of the excpected valid actions of the baron
        vector<string> expectedValidActions = {"gather", "tax", "bribe", "arrest", "sanction", "coup", "endTurn", "invest"};
            
        // get the valid actions for the player
        vector<string> actualValidActions = b.getValidActions();

        // check that the vectors are the same
        CHECK(expectedValidActions == actualValidActions);

        // check that the baron accepts the valid actions
        for(const string &action : expectedValidActions){
            CHECK(b.isValidAction(action) == true);
        }

        // check that the baron does not accept actions of any other type of player
        CHECK(!b.isValidAction("undoCoup") == true); // general special action
        CHECK(!b.isValidAction("spyOn") == true); // spy special action
        CHECK(!b.isValidAction("undoBribe") == true); // judge special action
        CHECK(!b.isValidAction("undoTax") == true); // governor special action
        CHECK(!b.isValidAction("blockArrest") == true); // spy special action

        // check that the baron does not accept any other actions (not real actions)
        CHECK(!b.isValidAction("test") == true);
        CHECK(!b.isValidAction("yatzee") == true);
        CHECK(!b.isValidAction("banana") == true);
    }
    SUBCASE("Test Baron Valid Undo Actions"){
        Baron b = Baron("John");

        // get a vector of the supposed valid undo actions of the baron
        vector<string> validUndoActions = b.getValidUndoActions();
        // should be 0 valid undo actions since the baron has no undo actions
        CHECK(validUndoActions.size() == 0);

        // check that the baron does not accept any undo actions
        CHECK(!b.isValidUndoAction("undoCoup") == true); // general special action
        CHECK(!b.isValidUndoAction("undoBribe") == true); // judge special action
        CHECK(!b.isValidUndoAction("undoTax") == true); // governor special action
        // check that the baron does not accept any other actions as undo actions (not real actions)
        CHECK(!b.isValidUndoAction("test") == true);
        CHECK(!b.isValidUndoAction("yatzee") == true);
    }
}

TEST_CASE("Test Baron Special Interactions"){
    SUBCASE("Test Invest"){
        SUBCASE("Test Investment with Enough Money"){
            Baron b = Baron("John");
            // give b some coins
            b.prepareForTurn();
            b.gather();
            b.prepareForTurn();
            b.gather();
            b.prepareForTurn();
            b.gather();

            // check that the baron has 3 coins
            CHECK(b.coins() == 3);

            // attempt to invest
            b.prepareForTurn();
            CHECK_NOTHROW(b.invest());

            // check that the baron has 6 coins
            CHECK(b.coins() == 6);
        }
        SUBCASE("Test Investment with Not Enough Money"){
            Baron b = Baron("John");
            // give b some coins
            b.prepareForTurn();
            b.gather();
            b.prepareForTurn();
            b.gather();

            // check that the baron has 2 coins
            CHECK(b.coins() == 2);

            // attempt to invest
            CHECK_THROWS(b.invest());

            // check that the baron still has 2 coins
            CHECK(b.coins() == 2);
        }
    }
    SUBCASE("Test Sanction Passive"){
        Baron b1 = Baron("John");
        // give b1 some coins
        b1.prepareForTurn();
        b1.tax();
        b1.prepareForTurn();
        b1.tax();

        // give b1 an action
        b1.prepareForTurn();

        // create a baron to test against
        Baron b2 = Baron("Doe");

        // check that b2 has no coins
        CHECK(b2.coins() == 0);
        
        // check that the financial actions in b2 are not blocked
        CHECK(!b2.isTaxBlocked() == true);
        CHECK(!b2.isGatherBlocked() == true);

        // attempt to sanction b2
        CHECK_NOTHROW(b1.sanction(b2)); // should not throw an exception
        // check that b1 has no actions left
        CHECK(b1.getRemainingActions() == 0);
        // check that the financial actions in b2 are blocked
        CHECK(b2.isTaxBlocked() == true);
        CHECK(b2.isGatherBlocked() == true);

        // check that b2 has 1 coin as refund
        CHECK(b2.coins() == 1);
    }
}