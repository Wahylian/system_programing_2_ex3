#include "testGovernor.hpp"

TEST_CASE("Test Governor Construction"){
    SUBCASE("Test Governor Constructor"){
        Governor g = Governor("John");
        
        // check that the values are correct
        CHECK(g.getName() == "John");
        CHECK(g.getRole() == "Governor");
        CHECK(g.coins() == 0);
        CHECK(g.getRemainingActions() == 0);
        CHECK(g.getLastArrested() == nullptr);
        CHECK(g.getLastAction() == "");
    }

    SUBCASE("Test Governor Copy Constructor"){
        Governor g1 = Governor("John");
        // give g1 some coins
        g1.prepareForTurn();
        g1.tax();
        g1.prepareForTurn();
        g1.gather();

        // create a copy of g1
        Governor g2 = Governor(g1);

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

    SUBCASE("Test Governor Assignment Operator"){
        SUBCASE("Assignment to Self"){
            Governor g = Governor("John");

            // give g some coins
            g.prepareForTurn();
            g.gather();
            g.prepareForTurn();
            g.gather();

            // save the address of g
            void *pG = &g;

            // pragmas to allow for self assignment
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wself-assign-overloaded"
            g = g; // Self-assignment
            #pragma clang diagnostic pop

            // check that the values are correct
            CHECK(g.getName() == "John");
            CHECK(g.getRole() == "Governor");
            CHECK(g.coins() == 2);
            CHECK(g.getRemainingActions() == 0);
            CHECK(g.getLastArrested() == nullptr);
            CHECK(g.getLastAction() == "gather");

            // check that the player is saved in the same address
            CHECK(pG == &g);
        }
        SUBCASE("Assignment to Different Player"){
            Governor g1 = Governor("John");
        
            // give g1 some coins
            g1.prepareForTurn();
            g1.gather();
            g1.prepareForTurn();
            g1.gather();

            Governor g2 = Governor("Doe"); // create a new player

            // assign g1 to g2
            g2 = g1;

            // check that the players are the same
            CHECK(g1.getName() == g2.getName());
            CHECK(g1.getRole() == g2.getRole());
            CHECK(g1.coins() == g2.coins());
            CHECK(g1.getRemainingActions() == g2.getRemainingActions());
            CHECK(g1.getLastArrested() == g2.getLastArrested());
            CHECK(g1.getLastAction() == g2.getLastAction());

            // check that the players are not the same object
            CHECK(&g1 != &g2);
        }
    }
}

TEST_CASE("Test Governor Valid Actions"){
    SUBCASE("Test Governor Valid Actions"){
            Governor g = Governor("John");

            // create a list of the excpected valid actions of the governor
            vector<string> expectedValidActions = {"gather", "tax", "bribe", "arrest", "sanction", "coup", "endTurn"};
            
            // get the valid actions for the player
            vector<string> actualValidActions = g.getValidActions();

            // check that the vectors are the same
            CHECK(expectedValidActions == actualValidActions);

            // check that the governor accepts the valid actions
            for(const string &action : expectedValidActions){
                CHECK(g.isValidAction(action) == true);
            }

            // check that the governor does not accept the undoTax action
            CHECK(!g.isValidAction("undoTax") == true); // governor special action

            // check that the governor does not accept actions of any other type of player
            CHECK(!g.isValidAction("invest") == true); // baron special action
            CHECK(!g.isValidAction("undoCoup") == true); // general special action
            CHECK(!g.isValidAction("spyOn") == true); // spy special action
            CHECK(!g.isValidAction("undoBribe") == true); // judge special action
            CHECK(!g.isValidAction("blockArrest") == true); // spy special action

            // check that the governor does not accept any other actions (not real actions)
            CHECK(!g.isValidAction("test") == true);
            CHECK(!g.isValidAction("yatzee") == true);
            CHECK(!g.isValidAction("banana") == true);
    }

    SUBCASE("Test Governor Valid Undo Actions"){
            Governor g = Governor("John");

            // get a vector of the supposed valid undo actions of the governor
            vector<string> validUndoActions = g.getValidUndoActions();
            // should be 1 valid undo action since the governor has only the undoTax action
            CHECK(validUndoActions.size() == 1);
            // check that the undo action is "undoTax"
            CHECK(validUndoActions[0] == "undoTax");

            // check that the general accepts only the undoTax action
            CHECK(!g.isValidUndoAction("undoCoup") == true); // general special action
            CHECK(!g.isValidUndoAction("undoBribe") == true); // judge special action
            CHECK(g.isValidUndoAction("undoTax") == true); 

            // check that the governor does not accept any other actions as undo actions (not real actions)
            CHECK(!g.isValidUndoAction("test") == true);
            CHECK(!g.isValidUndoAction("yatzee") == true);
    }
}

TEST_CASE("Test Governor Special Interactions"){
    SUBCASE("Test Tax"){
        Governor g = Governor("John");
        
        // give g an action
        g.prepareForTurn();
        // check that the governor has no coins
        CHECK(g.coins() == 0);

        // attempt to tax
        CHECK_NOTHROW(g.tax()); // should not throw an exception

        // check that the governor has 3 coins
        CHECK(g.coins() == 3);
        // check that the governor has no actions left
        CHECK(g.getRemainingActions() == 0);
    }

    SUBCASE("Test Undo Tax"){
        SUBCASE("Undo Tax on Self"){
            Governor g = Governor("John");
            // give g an action
            g.prepareForTurn();
            // give g some coins
            g.tax(); // should have 3 coins now

            // attempt to undo tax on self
            CHECK_THROWS(g.undoTax(g));
        }   

        SUBCASE("Undo Tax on Player with Not Enough Coins"){
            // a player with no coins could not have done a tax action
            Governor g1 = Governor("John");
            Governor g2 = Governor("Doe");
            // give g1 an action
            g1.prepareForTurn();

            // attempt to undo tax on g1
            CHECK_THROWS(g2.undoTax(g1)); // should throw an exception
        }

        SUBCASE("Undo Tax on Another Governor"){
            Governor g1 = Governor("John");
            Governor g2 = Governor("Doe");
            // give g2 an action
            g2.prepareForTurn();
            // give g2 some coins
            g2.tax(); // should have 3 coins now

            // check that g2 has 3 coins
            CHECK(g2.coins() == 3);

            // attempt to undo tax on g2
            CHECK_NOTHROW(g1.undoTax(g2)); // should not throw an exception
            // check that g2 has 0 coins now
            CHECK(g2.coins() == 0);
        }

        SUBCASE("Undo Tax on Another Type of Player"){
            // a player with no coins could not have done a tax action
            Governor g1 = Governor("John");
            TestPlayer g2 = TestPlayer("Doe");

            // give g2 an action
            g2.prepareForTurn();
            // give g2 some coins
            g2.tax(); // should have 2 coins now

            CHECK(g2.coins() == 2);

            // attempt to undo tax on g2
            CHECK_NOTHROW(g1.undoTax(g2)); // should not throw an exception
            // check that g2 has 0 coins now
            CHECK(g2.coins() == 0);
        }
    }
}