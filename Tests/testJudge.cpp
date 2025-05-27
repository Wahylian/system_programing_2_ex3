#include "testJudge.hpp"

TEST_CASE("Test Judge Construction"){
    SUBCASE("Test Judge Constructor"){
        Judge j = Judge("John");
        
        // check that the values are correct 
        CHECK(j.getName() == "John");
        CHECK(j.getRole() == "Judge");
        CHECK(j.coins() == 0);
        CHECK(j.getRemainingActions() == 0);
        CHECK(j.getLastArrested() == nullptr);
        CHECK(j.getLastAction() == "");
    }

    SUBCASE("Test Judge Copy Constructor"){
        Judge j1 = Judge("John");

        // give j1 some coins
        j1.prepareForTurn();
        j1.gather();
        j1.prepareForTurn();
        j1.gather();

        // create a copy of j1
        Judge j2 = Judge(j1);

        // check that the values are correct
        CHECK(j2.getName() == j1.getName());
        CHECK(j2.getRole() == j1.getRole());
        CHECK(j2.coins() == j1.coins());
        CHECK(j2.getRemainingActions() == j1.getRemainingActions());
        CHECK(j2.getLastArrested() == j1.getLastArrested());
        CHECK(j2.getLastAction() == j1.getLastAction());

        // check that the players are not the same object
        CHECK(&j1 != &j2);
    }

    SUBCASE("Test Judge Assignment Operator"){
        SUBCASE("Assignment to Self"){
            Judge j1 = Judge("John");
            // give j1 some coins
            j1.prepareForTurn();
            j1.gather();
            j1.prepareForTurn();
            j1.gather();

            // save the address of j1
            void *pJ1 = &j1;

            // pragmas to allow for self assignment
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wself-assign-overloaded"
            j1 = j1; // Self-assignment
            #pragma clang diagnostic pop

            // check that the values are correct
            CHECK(j1.getName() == "John");
            CHECK(j1.getRole() == "Judge");
            CHECK(j1.coins() == 2);
            CHECK(j1.getRemainingActions() == 0);
            CHECK(j1.getLastArrested() == nullptr);
            CHECK(j1.getLastAction() == "gather");
        
            // check that the player is saved in the same address
            CHECK(pJ1 == &j1);
        }
        SUBCASE("Assignment to Another Player"){
            Judge j1 = Judge("John");

            // give j1 some coins
            j1.prepareForTurn();
            j1.gather();
            j1.prepareForTurn();
            j1.gather();

            // give j1 an action
            j1.prepareForTurn();

            // assign j1 to j2
            Judge j2 = j1;

            // check that the values are correct
            CHECK(j2.getName() == j1.getName());
            CHECK(j2.getRole() == j1.getRole());
            CHECK(j2.coins() == j1.coins());
            CHECK(j2.getRemainingActions() == j1.getRemainingActions());
            CHECK(j2.getLastArrested() == j1.getLastArrested());
            CHECK(j2.getLastAction() == j1.getLastAction());

            // check that the players are not the same object
            CHECK(&j1 != &j2);
        }
    }
}

TEST_CASE("Test Judge Valid Actions"){
    SUBCASE("Test Judge Valid Actions"){
        Judge j = Judge("John");

        // create a list of the excpected valid actions of the judge
        vector<string> expectedValidActions = {"gather", "tax", "bribe", "arrest", "sanction", "coup", "endTurn"};
        
        // get the valid actions for the player
        vector<string> validActions = j.getValidActions();

        // checks that both vectors are the same
        CHECK(expectedValidActions == validActions);

        // check that the judge accepts the valid actions
        for(const string &action : expectedValidActions){
            CHECK(j.isValidAction(action) == true);
        }

        // check that the judge does not accept the undoBribe action
        CHECK(!j.isValidAction("undoBribe") == true); // judge special action
        
        // check that the judge does not accept actions of any other type of player
        CHECK(!j.isValidAction("invest") == true); // baron special action
        CHECK(!j.isValidAction("blockArrest") == true); // spy special action
        CHECK(!j.isValidAction("spyOn") == true); // spy special action
        CHECK(!j.isValidAction("undoCoup") == true); // general special action
        CHECK(!j.isValidAction("undoTax") == true); // governor special action

        // check that the judge does not accept any other actions (not real actions)
        CHECK(!j.isValidAction("test") == true);
        CHECK(!j.isValidAction("yatzee") == true);
        CHECK(!j.isValidAction("banana") == true);
    }
    SUBCASE("Test Judge Valid Undo Actions"){
        Judge j = Judge("John");

        // get a vector of the supposed valid undo actions of the judge
        vector<string> validUndoActions = j.getValidUndoActions();
        // should be 1 valid undo action since the judge has only the undoBribe action
        CHECK(validUndoActions.size() == 1);
        // check that the undo action is "undoBribe"
        CHECK(validUndoActions[0] == "undoBribe");

        // check that the judge accepts only the undoBribe action
        CHECK(!j.isValidUndoAction("undoCoup") == true); // general special action
        CHECK(j.isValidUndoAction("undoBribe") == true); 
        CHECK(!j.isValidUndoAction("undoTax") == true); // governor special action

        // check that the judge does not accept any other actions as undo actions (not real actions)
        CHECK(!j.isValidUndoAction("test") == true);
        CHECK(!j.isValidUndoAction("yatzee") == true);
    }
}

TEST_CASE("Test Judge Special Interactions"){
    SUBCASE("Test Undo Bribe"){
        SUBCASE("Test Undo Bribe on Self"){
            Judge j = Judge("John");
            // give j enough money to bribe
            j.prepareForTurn();
            j.tax();
            j.prepareForTurn();
            j.tax();

            // give j an action
            j.prepareForTurn();
            // make j bribe
            j.bribe(); 

            // attempt to undo bribe on self
            CHECK_THROWS(j.undoBribe(j)); // should throw an exception
        }

        SUBCASE("Test Undo Bribe on Another Player"){
            SUBCASE("Test Undo Bribe on a Player that did not Bribe"){
                Judge j1 = Judge("John");
                Judge j2 = Judge("Doe");
                // give j1 an action
                j1.prepareForTurn();

                // attempt to undo j2's bribe
                CHECK_THROWS(j1.undoBribe(j2)); // should throw an exception
            }

            SUBCASE("Test Undo Bribe on a Player that did Bribe"){
                Judge j1 = Judge("John");
                Judge j2 = Judge("Doe");

                // give j2 enough money to bribe
                j2.prepareForTurn();
                j2.tax();
                j2.prepareForTurn();
                j2.tax();

                // give j2 an action
                j2.prepareForTurn();
                // make j2 bribe
                j2.bribe();

                // check that j2 has 2 actions remaining
                CHECK(j2.getRemainingActions() == 2);

                // attempt to undo j2's bribe
                CHECK_NOTHROW(j1.undoBribe(j2)); // should not throw an exception

                // check that j2 has 1 action remaining
                CHECK(j2.getRemainingActions() == 1);
            }
        }
    }

    SUBCASE("Test Sanction Passive Interaction"){
        Judge j1 = Judge("John");
        Judge j2 = Judge("Doe");

        // give j2 some money to sanction
        j2.prepareForTurn();
        j2.tax();
        j2.prepareForTurn();
        j2.tax();

        // check that j2 has 4 coins
        CHECK(j2.coins() == 4);

        // give j2 an action
        j2.prepareForTurn();

        // check that j1 has no economical actions blocked
        CHECK(j1.isGatherBlocked() == false);
        CHECK(j1.isTaxBlocked() == false);

        // make j2 sanction j1
        j2.sanction(j1);

        // check that j1 was sanctioned
        CHECK(j1.isGatherBlocked() == true);
        CHECK(j1.isTaxBlocked() == true);

        // check that j2 has 0 coins left
        CHECK(j2.coins() == 0);
    }
}