#include "testMerchant.hpp"

TEST_CASE("Test Merchant Construction"){
    SUBCASE("Test Merchant Constructor"){
        Merchant m = Merchant("John");

        // check that the values are correct
        CHECK(m.getName() == "John");
        CHECK(m.getRole() == "Merchant");
        CHECK(m.coins() == 0);
        CHECK(m.getRemainingActions() == 0);
        CHECK(m.getLastArrested() == nullptr);
        CHECK(m.getLastAction() == "");
    }
    SUBCASE("Test Merchant Copy Constructor"){
        Merchant m1 = Merchant("John");
        // give m1 some coins
        m1.prepareForTurn();
        m1.gather();
        m1.prepareForTurn();
        m1.gather();

        // create a copy of m1
        Merchant m2 = Merchant(m1);

        // check that the values are correct
        CHECK(m2.getName() == m1.getName());
        CHECK(m2.getRole() == m1.getRole());
        CHECK(m2.coins() == m1.coins());
        CHECK(m2.getRemainingActions() == m1.getRemainingActions());
        CHECK(m2.getLastArrested() == m1.getLastArrested());
        CHECK(m2.getLastAction() == m1.getLastAction());

        // check that the players are not the same object
        CHECK(&m1 != &m2);
    }
    SUBCASE("Test Merchant Assignment Operator"){
        SUBCASE("Assignment to Self"){
            Merchant m = Merchant("John");

            // give m some coins
            m.prepareForTurn();
            m.gather();
            m.prepareForTurn();
            m.gather();

            // save the address of m
            void *pM = &m;

            // pragmas to allow for self assignment
            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wself-assign-overloaded"
            m = m; // Self-assignment
            #pragma clang diagnostic pop

            // check that the values are correct
            CHECK(m.getName() == "John");
            CHECK(m.getRole() == "Merchant");
            CHECK(m.coins() == 2);
            CHECK(m.getRemainingActions() == 0);
            CHECK(m.getLastArrested() == nullptr);
            CHECK(m.getLastAction() == "gather");

            // check that the player is saved in the same address
            CHECK(pM == &m);
        }
        SUBCASE("Assignment to Another Merchant"){
            Merchant m1 = Merchant("John");

            // give m1 some coins
            m1.prepareForTurn();
            m1.gather();
            m1.prepareForTurn();
            m1.gather();

            Merchant m2 = Merchant("Doe"); // create a new player

            // assign m1 to m2
            m2 = m1;

            // check that the values are correct
            CHECK(m2.getName() == m1.getName());
            CHECK(m2.getRole() == m1.getRole());
            CHECK(m2.coins() == m1.coins());
            CHECK(m2.getRemainingActions() == m1.getRemainingActions());
            CHECK(m2.getLastArrested() == m1.getLastArrested());
            CHECK(m2.getLastAction() == m1.getLastAction());

            // check that the players are not the same object
            CHECK(&m1 != &m2);
        }
    }
}

TEST_CASE("Test Merchant Valid Actions"){
    SUBCASE("Test Merchant Valid Actions"){
        Merchant m = Merchant("John");

        // create a vector of the expected valid actions of m
        vector<string> expectedValidActions = {"gather", "tax", "bribe", "arrest", "sanction", "coup", "endTurn"};    
        
        // get the valid actions of m
        vector<string> validActions = m.getValidActions();
        
        // check that both vectors are the same
        CHECK(validActions == expectedValidActions);

        // check that the merchant accepts the valid actions
        for(const string& action : validActions) {
            CHECK(m.isValidAction(action) == true);
        }

        // check that the merchant does not accept actions of any other type of player
        CHECK(!m.isValidAction("undoBribe") == true); // judge special action
        CHECK(!m.isValidAction("invest") == true); // baron special action
        CHECK(!m.isValidAction("blockArrest") == true); // spy special action
        CHECK(!m.isValidAction("spyOn") == true); // spy special action
        CHECK(!m.isValidAction("undoCoup") == true); // general special action
        CHECK(!m.isValidAction("undoTax") == true); // governor special action

        // check that the merchant does not accept any other actions (not real actions)
        CHECK(!m.isValidAction("test") == true);
        CHECK(!m.isValidAction("yatzee") == true);
        CHECK(!m.isValidAction("banana") == true); 
    }
    SUBCASE("Test Merchant Valid Undo Actions"){
        Merchant m = Merchant("John");

        // get a vector of the supposed valid undo actions of the merchant
        vector<string> validUndoActions = m.getValidUndoActions();
        
        // should be 0 valid undo actions since the merchant has no undo actions
        CHECK(validUndoActions.size() == 0);

        // check that the merchant does not accept any undo actions
        CHECK(!m.isValidUndoAction("undoCoup") == true); // general special action
        CHECK(!m.isValidUndoAction("undoBribe") == true); // judge special action
        CHECK(!m.isValidUndoAction("undoTax") == true); // governor special action

        // check that the merchant does not accept any other actions as undo actions (not real actions)
        CHECK(!m.isValidUndoAction("test") == true);
        CHECK(!m.isValidUndoAction("yatzee") == true);
        CHECK(!m.isValidUndoAction("banana") == true);
    }
}

TEST_CASE("Test Merchant Special Interactions"){
    SUBCASE("Test Prepare For Turn Passive"){
        Merchant m = Merchant("John");

        // give the merchant 4 coins
        m.prepareForTurn();
        m.tax();
        m.prepareForTurn();
        m.tax();

        // check that the merchant has 4 coins
        CHECK(m.coins() == 4);

        // prepare for turn
        m.prepareForTurn();
        // check that the merchant now has 5 coins
        CHECK(m.coins() == 5);
    }
    SUBCASE("Test Arrest Passive"){
        SUBCASE("Test Arrest With Less Than 2 Coins"){
            Merchant m1 = Merchant("John");
            Merchant m2 = Merchant("Doe");

            // give m2 a coin
            m2.prepareForTurn();
            m2.gather();

            // check that m2 has 1 coins
            CHECK(m2.coins() == 1);

            // attempt to arrest m2
            m1.prepareForTurn();
            CHECK_NOTHROW(m1.arrest(m2)); // should not throw an exception

            // check that m2 has no coins left
            CHECK(m2.coins() == 0);

            // check that m1 has no actions left
            CHECK(m1.getRemainingActions() == 0);

            // check that m1 still has no coins
            CHECK(m1.coins() == 0);

            // check that m1's last arrested player is m2
            CHECK(m1.getLastArrested() == &m2);
        }
        SUBCASE("Test Arrest With 2 Coins"){
            Merchant m1 = Merchant("John");
            Merchant m2 = Merchant("Doe");

            // give m2 some coins
            m2.prepareForTurn();
            m2.tax();

            // check that m2 has 2 coins
            CHECK(m2.coins() == 2);

            // attempt to arrest m2
            m1.prepareForTurn();
            CHECK_NOTHROW(m1.arrest(m2)); // should not throw an exception

            // check that m2 has no coins left
            CHECK(m2.coins() == 0);

            // check that m1 has no actions left
            CHECK(m1.getRemainingActions() == 0);

            // check that m1 still has no coins
            CHECK(m1.coins() == 0);

            // check that m1's last arrested player is m2
            CHECK(m1.getLastArrested() == &m2);
        }
        SUBCASE("Test Arrest With More Than 2 Coins"){
            Merchant m1 = Merchant("John");
            Merchant m2 = Merchant("Doe");

            // give m2 some coins
            m2.prepareForTurn();
            m2.tax();
            m2.prepareForTurn();
            m2.tax();

            // check that m2 has 4 coins
            CHECK(m2.coins() == 4);

            // attempt to arrest m2
            m1.prepareForTurn();
            CHECK_NOTHROW(m1.arrest(m2)); // should not throw an exception

            // check that m2 has 2 coins left
            CHECK(m2.coins() == 2);

            // check that m1 has no actions left
            CHECK(m1.getRemainingActions() == 0);

            // check that m1 still has no coins
            CHECK(m1.coins() == 0);

            // check that m1's last arrested player is m2
            CHECK(m1.getLastArrested() == &m2);
        }
    }
}