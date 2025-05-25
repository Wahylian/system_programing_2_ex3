#include "testGeneral.hpp"

TEST_CASE("Test General Construction"){
    SUBCASE("Test General Constructor"){
        General g = General("John");
        
        // check that the values are correct
        CHECK(g.getName() == "John");
        CHECK(g.getRole() == "General");
        CHECK(g.coins() == 0);
        CHECK(g.getRemainingActions() == 0);
        CHECK(g.getLastArrested() == nullptr);
        CHECK(g.getLastAction() == ""); 
    }
    SUBCASE("Test General Copy Constructor"){
        General g1 = General("John");
        // give g1 some coins
        g1.prepareForTurn();
        g1.gather();
        g1.prepareForTurn();
        g1.gather();

        // create a copy of g1
        General g2 = General(g1);

        // check that the values are correct
        CHECK(g2.getName() == g1.getName());
        CHECK(g2.getRole() == g1.getRole());
        CHECK(g2.coins() == g1.coins());
        CHECK(g2.getRemainingActions() == g1.getRemainingActions());
        CHECK(g2.getLastArrested() == g1.getLastArrested());
        CHECK(g2.getLastAction() == g1.getLastAction());

        // check that the players are not the same object
        CHECK(&g1 != &g2);
    }
    SUBCASE("Test General Assignment Operator"){
        SUBCASE("Assignment to Self"){
            General g1 = General("John");
            // give g1 some coins
            g1.prepareForTurn();
            g1.gather();
            g1.prepareForTurn();
            g1.gather();

            // save the address of g1
            void *pG1 = &g1;

            // pragmas to allow for self assignment
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wself-assign-overloaded"
            g1 = g1; // Self-assignment
            #pragma clang diagnostic pop

            // check that the values are correct
            CHECK(g1.getName() == "John");
            CHECK(g1.getRole() == "General");
            CHECK(g1.coins() == 2);
            CHECK(g1.getRemainingActions() == 0);
            CHECK(g1.getLastArrested() == nullptr);
            CHECK(g1.getLastAction() == "gather");
        
            // check that the player is saved in the same address
            CHECK(pG1 == &g1);
        }
        SUBCASE("Assignment to General"){
            General g1 = General("John");
            // give g1 some coins
            g1.prepareForTurn();
            g1.gather();
            g1.prepareForTurn();
            g1.gather();

            // assign g1 to g2
            General g2 = g1;

            // check that the values are correct
            CHECK(g2.getName() == g1.getName());
            CHECK(g2.getRole() == g1.getRole());
            CHECK(g2.coins() == g1.coins());
            CHECK(g2.getRemainingActions() == g1.getRemainingActions());
            CHECK(g2.getLastArrested() == g1.getLastArrested());
            CHECK(g2.getLastAction() == g1.getLastAction());

            // check that the players are not the same object
            CHECK(&g1 != &g2);
        }
    }
}

TEST_CASE("Test General Valid Actions"){
    SUBCASE("Test Valid Actions"){
        General g = General("John");

        // create a list of the excpected valid actions of the general
        vector<string> expectedValidActions = {"gather", "tax", "bribe", "arrest", "sanction", "coup", "endTurn"};
            
        // get the valid actions for the player
        vector<string> actualValidActions = g.getValidActions();

        // check that the vectors are the same
        CHECK(expectedValidActions == actualValidActions);

        // check that the general accepts the valid actions
        for(const string &action : expectedValidActions){
            CHECK(g.isValidAction(action) == true);
        }

        // check that the undoCoup action is not accepted as a valid action
        CHECK(!g.isValidAction("undoCoup") == true); // general special action

        // check that the general does not accept actions of any other type of player
        CHECK(!g.isValidAction("invest") == true); // baron special action
        CHECK(!g.isValidAction("spyOn") == true); // spy special action
        CHECK(!g.isValidAction("undoBribe") == true); // judge special action
        CHECK(!g.isValidAction("undoTax") == true); // governor special action
        CHECK(!g.isValidAction("blockArrest") == true); // spy special action

        // check that the general does not accept any other actions (not real actions)
        CHECK(!g.isValidAction("test") == true);
        CHECK(!g.isValidAction("yatzee") == true);
        CHECK(!g.isValidAction("banana") == true);
    }
    SUBCASE("Test Valid Undo Actions"){
        General g = General("John");

        // get a vector of the supposed valid undo actions of the general
        vector<string> validUndoActions = g.getValidUndoActions();
        // should be 1 valid undo action since the general is a general
        CHECK(validUndoActions.size() == 1);
        // check that the undo action is "undoCoup"
        CHECK(validUndoActions[0] == "undoCoup");

        // check that the general accepts only the undoCoup action
        CHECK(g.isValidUndoAction("undoCoup") == true);
        CHECK(!g.isValidUndoAction("undoBribe") == true); // judge special action
        CHECK(!g.isValidUndoAction("undoTax") == true); // governor special action

        // check that the general does not accept any other actions as undo actions (not real actions)
        CHECK(!g.isValidUndoAction("test") == true);
        CHECK(!g.isValidUndoAction("yatzee") == true);
    }
}

TEST_CASE("Test General Special Interactions"){
    SUBCASE("Test Undo Coup"){
        // since undo coup is a special action and is situational, it will be tested mainly in the game tests
        SUBCASE("Test Undo Coup Performed by Self"){
            General g1 = General("John");
            // give g enough coins to perform a coup
            g1.prepareForTurn();
            g1.tax();
            g1.prepareForTurn();
            g1.tax();
            g1.prepareForTurn();
            g1.tax();
            g1.prepareForTurn();
            g1.tax();

            // check that the general has 8 coins
            CHECK(g1.coins() == 8);

            General g2 = General("Doe");
            
            // perform a coup on g2
            g1.prepareForTurn();
            CHECK_NOTHROW(g1.coup(g2)); // should not throw an exception

            // make g1 attempt to undo their own coup
            CHECK_THROWS(g1.undoCoup(g1)); // should throw an exception

            // check that g1 has no actions left
            CHECK(g1.getRemainingActions() == 0);
        }
        SUBCASE("Test Undo Coup Performed by Another Player"){
            SUBCASE("Test Undo Coup Performed on Self"){
                General g1 = General("John");
                General g2 = General("Doe");

                // give g2 enough coins to perform a coup
                g2.prepareForTurn();
                g2.tax();
                g2.prepareForTurn();
                g2.tax();
                g2.prepareForTurn();
                g2.tax();
                g2.prepareForTurn();
                g2.tax();

                // g2 performs a coup on g1
                g2.prepareForTurn();
                CHECK_NOTHROW(g2.coup(g1)); // should not throw an exception

                // make g1 undo the coup performed by g2 on g1
                CHECK_NOTHROW(g1.undoCoup(g2)); // should not throw an exception

                // check that g2 has no actions left 
                CHECK(g2.getRemainingActions() == 0);
                // check that g2 has 1 coin left
                CHECK(g2.coins() == 1);
            }
            SUBCASE("Test Undo Coup Performed on Another Player"){
                General g1 = General("John");
                General g2 = General("Doe");
                General g3 = General("Smith");

                // give g2 enough coins to perform a coup
                g2.prepareForTurn();
                g2.tax();
                g2.prepareForTurn();
                g2.tax();
                g2.prepareForTurn();
                g2.tax();
                g2.prepareForTurn();
                g2.tax();

                // g2 performs a coup on g3
                g2.prepareForTurn();
                CHECK_NOTHROW(g2.coup(g3)); // should not throw an exception

                // make g1 undo the coup performed by g2 on g3
                CHECK_NOTHROW(g1.undoCoup(g2)); // should not throw an exception
                // check that g2 has no actions left
                CHECK(g2.getRemainingActions() == 0);
                // check that g2 has 1 coin left
                CHECK(g2.coins() == 1);
            }
        }
    }
    SUBCASE("Test Arrest Passive Interaction"){
        General g1 = General("John");
        // give g1 an action
        g1.prepareForTurn();
        // check that g1 has no coins
        CHECK(g1.coins() == 0);

        General g2 = General("Doe");
        // give g2 some coins
        g2.prepareForTurn();
        g2.tax();

        // attempt to arrest a player
        CHECK_NOTHROW(g1.arrest(g2)); // should not throw an exception

        // check that g1 has no actions left
        CHECK(g1.getRemainingActions() == 0);

        // check that g1 still has no coins 
        CHECK(g1.coins() == 0);
        // check that g2 has 2 coins
        CHECK(g2.coins() == 2);
    }
}