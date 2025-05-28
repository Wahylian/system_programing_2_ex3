#include "testGame.hpp"

TEST_CASE("Test Game Initialization"){
    SUBCASE("Test Invalid Number of Players"){
        SUBCASE("Test Less Than 2 Players") {
            CHECK_THROWS(TestGame({"Alice"}));
        }
        SUBCASE("Test More Than 6 Players") {
            CHECK_THROWS(TestGame({"Alice", "Bob", "Charlie", "Dave", "Eve", "Frank", "Grace"}));
        }
    }

    SUBCASE("Test Valid Number of Players"){
        SUBCASE("Test 2 Players"){
            vector<string> players = {"Alice", "Bob"};
            TestGame g = TestGame(players);
            
            // check the current player is alice
            CHECK(g.turn() == "Alice");
            // check the number of players is 2
            CHECK(g.playersCount() == 2);

            // check the players are correct
            CHECK(g.players() == players); 
        }

        SUBCASE("Test 3 Players"){
            vector<string> players = {"Alice", "Bob", "Charlie"};
            TestGame g = TestGame(players);

            // check the current player is alice
            CHECK(g.turn() == "Alice");
            // check the number of players is 3
            CHECK(g.playersCount() == 3);

            // check the players are correct
            CHECK(g.players() == players);
        }

        SUBCASE("Test 4 Players"){
            vector<string> players = {"Alice", "Bob", "Charlie", "Dave"};
            TestGame g = TestGame(players);

            // check the current player is alice
            CHECK(g.turn() == "Alice");
            // check the number of players is 4
            CHECK(g.playersCount() == 4);

            // check the players are correct
            CHECK(g.players() == players);
        }

        SUBCASE("Test 5 Players"){
            vector<string> players = {"Alice", "Bob", "Charlie", "Dave", "Eve"};
            TestGame g = TestGame(players);

            // check the current player is alice
            CHECK(g.turn() == "Alice");
            // check the number of players is 5
            CHECK(g.playersCount() == 5);

            // check the players are correct
            CHECK(g.players() == players);
        }

        SUBCASE("Test 6 Players"){
            vector<string> players = {"Alice", "Bob", "Charlie", "Dave", "Eve", "Frank"};
            TestGame g = TestGame(players);

            // check the current player is alice
            CHECK(g.turn() == "Alice");
            // check the number of players is 6
            CHECK(g.playersCount() == 6);

            // check the players are correct
            CHECK(g.players() == players);
        }
    }

    SUBCASE("Test Player Values"){
        TestGame g = TestGame({"Alice", "Bob", "Charlie"});

        // get all the players
        const vector<Player *> &players = g.getPlayers();
        // check that the players are not null
        for(const Player *player : players){
            CHECK(player != nullptr);
        }
        // check that there are 3 players
        CHECK(players.size() == 3);

        // check that the players have the correct names
        CHECK(players[0]->getName() == "Alice");
        CHECK(players[1]->getName() == "Bob");
        CHECK(players[2]->getName() == "Charlie");

        // check that the players have no money
        CHECK(players[0]->coins() == 0);
        CHECK(players[1]->coins() == 0);
        CHECK(players[2]->coins() == 0);

        // at the start of the game, alice is the current player
        // check that alice is the only one with an action
        CHECK(players[0]->getRemainingActions() == 1);
        CHECK(players[1]->getRemainingActions() == 0);
        CHECK(players[2]->getRemainingActions() == 0); 

    }
}

TEST_CASE("Test Game with Multiple Players By the Same Name"){
    // create a game with 2 players with the same name and 1 player with a different name
    TestGame g = TestGame({"Alice", "Bob", "Alice"});

    // check that the game has 3 players
    CHECK(g.playersCount() == 3);

    // check that the names of the players are correct
    CHECK(g.getPlayerByIndex(0)->getName() == "Alice");
    CHECK(g.getPlayerByIndex(1)->getName() == "Bob");
    CHECK(g.getPlayerByIndex(2)->getName() == "Alice");

    // check that the two alices are not the same player
    CHECK(g.getPlayerByIndex(0) != g.getPlayerByIndex(2));
}

TEST_CASE("Test Constructors"){
    SUBCASE("Test Copy Constructor"){
        SUBCASE("Test Copy of Finished Game"){
            // create a game with two players
            TestGame g1 = TestGame({"Alice", "Bob"});

            // run the game until Alice has enough coins to coup Bob
            while(g1.getPlayers()[0]->coins() < 7){
                // have the player perform a gather action
                g1.playAction("gather");

                // pass the turn to the next player
                g1.nextTurn();
            }

            // check that the current player is Alice
            if(g1.turn() != "Alice"){
                // if not, pass the turn to her
                g1.nextTurn();
            }

            // have Alice perform a coup on Bob
            g1.playAction("coup", g1.getPlayers()[1]);

            // check that the game has ended
            CHECK_NOTHROW(g1.winner());

            // attempt to create a copy of the game
            CHECK_THROWS(TestGame(g1));
            
        
        }
        SUBCASE("Test Copy of Ongoing Game"){
            TestGame g1 = TestGame({"Alice", "Bob", "Danny"});

            // run the game for a few turns
            while(g1.getPlayers()[0]->coins() < 4){
                // have the player perform a gather action
                g1.playAction("gather");
        
                // pass the turn to the next player
                g1.playAction("endTurn");
            }

            // create a copy of the game
            TestGame g2 = TestGame(g1);

            // check that the copy has the same players
            CHECK(g2.playersCount() == g1.playersCount());

            for(int i = 0; i < g1.playersCount(); i++){
                Player *p1 = g1.getPlayerByIndex(i);
                Player *p2 = g2.getPlayerByIndex(i);

                // check that the values of the players are the same
                CHECK(p1->getName() == p2->getName());
                CHECK(p1->coins() == p2->coins());
                CHECK(p1->getRemainingActions() == p2->getRemainingActions());
                CHECK(p1->getRole() == p2->getRole());
                CHECK(p1->getLastAction() == p2->getLastAction());
                //CHECK(p1->getLastArrested() == p2->getLastArrested());

                // check that the pointers are not the same
                CHECK(p1 != p2);
            }

            // check that the current player is the same
            CHECK(g1.turn() == g2.turn());
            CHECK(g1.currentPlayerIndex() == g2.currentPlayerIndex());

            // check that the addresses of the games are different
            CHECK(&g1 != &g2);
        }
    }
    SUBCASE("Test Unique Test-Constructor"){
        SUBCASE("Test Valid Test-Constructor"){
            // randomly pick two roles
            vector<string> roles = {"Baron", "Governor", "Judge", "Spy", "General", "Merchant"};
            vector<string> selectedRoles;
            selectedRoles.push_back(roles[rand() % roles.size()]);
            selectedRoles.push_back(roles[rand() % roles.size()]);


            // create a game with two players
            TestGame g = TestGame({"Alice", "Bob"}, selectedRoles);

            // check that the players are created correctly
            CHECK(g.playersCount() == 2);
            CHECK(g.getPlayerByIndex(0)->getName() == "Alice");
            CHECK(g.getPlayerByIndex(1)->getName() == "Bob");

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // check that Alice has 0 coins
            CHECK(g.getPlayers()[0]->coins() == 0);

            // check that Bob has 0 coins
            CHECK(g.getPlayers()[1]->coins() == 0);

            // check that the role of Alice is the first selected role
            CHECK(g.getPlayers()[0]->getRole() == selectedRoles[0]);

            // check that the role of Bob is the second selected role
            CHECK(g.getPlayers()[1]->getRole() == selectedRoles[1]);
        }
        SUBCASE("Test With Invalid Specified Roles"){
            // create a game with two players and an invalid role
            CHECK_THROWS(TestGame({"Alice", "Bob"}, {"InvalidRole", "Governor"}));
            CHECK_THROWS(TestGame({"Alice", "Bob"}, {"Baron", "InvalidRole"}));
            CHECK_THROWS(TestGame({"Alice", "Bob"}, {"InvalidRole", "InvalidRole"}));
        }
        SUBCASE("Test With Different Number of Players and Roles"){
            // create a game with two players and three roles
            CHECK_THROWS(TestGame({"Alice", "Bob"}, {"Baron", "Governor", "Judge"}));
            // create a game with three players and two roles
            CHECK_THROWS(TestGame({"Alice", "Bob", "Charlie"}, {"Baron", "Governor"}));
        }
        SUBCASE("Test With Too Few Players"){
            // create a game with one player
            CHECK_THROWS(TestGame({"Alice"}, {"Baron"}));
        }
        SUBCASE("Test With Too Many Players"){
            // create a game with more than 6 players
            CHECK_THROWS(TestGame({"Alice", "Bob", "Charlie", "Dave", "Eve", "Frank", "Grace"}, \
                {"Baron", "Governor", "Judge", "Spy", "General", "Merchant", "Baron"}));
        }   
    }
}

TEST_CASE("Test Game Assignment Operator"){
    SUBCASE("Test Assignment to Self"){
        // create a game with two players
        TestGame g1 = TestGame({"Alice", "Bob"});

        // run the game for a few turns
        while(g1.getPlayers()[0]->coins() < 4){
            // have the player perform a gather action
            g1.playAction("gather");

            // pass the turn to the next player
            g1.nextTurn();
        }

        // check that the game has not ended
        CHECK_THROWS(g1.winner());

        // save the address of g1
        void *originalAddress = &g1;

        // pragmas to allow for self assignment
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wself-assign-overloaded"
        g1 = g1; // Self-assignment
        #pragma clang diagnostic pop

        // check that g1 is still the same game
        CHECK(g1.playersCount() == 2);
        CHECK(g1.getPlayers()[0]->getName() == "Alice");
        CHECK(g1.getPlayers()[1]->getName() == "Bob");
        CHECK(g1.getPlayers()[0]->coins() == 4);
        CHECK(g1.getPlayers()[1]->coins() == 3);

        // check that the current player is still Bob
        CHECK(g1.turn() == "Bob");

        // check that the address of g1 is still the same
        CHECK(originalAddress == &g1);
    }
    SUBCASE("Test Assignment of Finished Game"){
        // create a game with two players
        TestGame g1 = TestGame({"Alice", "Bob"});

        // run the game until Alice has enough coins to coup Bob
        while(g1.getPlayers()[0]->coins() < 7){
            // have the player perform a gather action
            g1.playAction("gather");

            // pass the turn to the next player
            g1.nextTurn();
        }

        // check that the current player is Alice
        if(g1.turn() != "Alice"){
            // if not, pass the turn to her
            g1.nextTurn();
        }

        // have Alice perform a coup on Bob
        g1.playAction("coup", g1.getPlayers()[1]);

        // check that the game has ended
        CHECK_NOTHROW(g1.winner());

        // create a new game
        TestGame g2 = TestGame({"Charlie", "Dave"});

        // attempt to assign the finished game to the new game
        CHECK_THROWS(g2 = g1);
    }
    SUBCASE("Test Assignment of Ongoing Game"){
        // create a game with two players
        TestGame g1 = TestGame({"Alice", "Bob"});

        // run the game for a few turns
        while(g1.getPlayers()[0]->coins() < 4){
            // have the player perform a gather action
            g1.playAction("gather");

            // pass the turn to the next player
            g1.nextTurn();
        }

        // check that the game has not ended
        CHECK_THROWS(g1.winner());

        // attempt to assign a the game
        TestGame g2 = g1;

        // check that the players are the same
        for(int i = 0; i < g1.playersCount(); i++){
            Player *p1 = g1.getPlayerByIndex(i);
            Player *p2 = g2.getPlayerByIndex(i);

            // check that the values of the players are the same
            CHECK(p1->getName() == p2->getName());
            CHECK(p1->coins() == p2->coins());
            CHECK(p1->getRemainingActions() == p2->getRemainingActions());
            CHECK(p1->getRole() == p2->getRole());
            CHECK(p1->getLastAction() == p2->getLastAction());
            //CHECK(p1->getLastArrested() == p2->getLastArrested());

            // check that the pointers are not the same
            CHECK(p1 != p2);
        }

        // check that the current player is the same
        CHECK(g1.turn() == g2.turn());
        CHECK(g1.currentPlayerIndex() == g2.currentPlayerIndex());

        // check that the addresses of the games are not the same
        CHECK(&g1 != &g2);
    }
}

TEST_CASE("Test Game Destructor"){
    TestGame *g = new TestGame({"Alice", "Bob", "Charlie"});

    // check that the game is not pointed to null
    CHECK(g != nullptr);

    // check that the game has 3 players
    CHECK(g->playersCount() == 3);

    // check that the current player is Alice
    CHECK(g->turn() == "Alice");

    // check that the players are not null
    for(const Player *player : g->getPlayers()){
        CHECK(player != nullptr);
    }

    // delete the game
    delete g;
}

TEST_CASE("Test Turn Passing"){
    TestGame g = TestGame({"Alice", "Bob", "Charlie"});

    // check that the index of the current player is 0 (Alice)
    int currentIndex = g.currentPlayerIndex();
    CHECK(currentIndex == 0);
    // check that the current player is Alice
    CHECK(g.turn() == "Alice");
    // check that alice is the only one with an action
    for(int i = 0; i < g.playersCount(); i++){
        if(i == currentIndex){
            CHECK(g.getPlayers()[i]->getRemainingActions() == 1);
        } 
        else {
            CHECK(g.getPlayers()[i]->getRemainingActions() == 0);
        }
    }

    // check that the index of the next player is 1 (Bob)
    CHECK(g.nextPlayerIndex() == 1);
        
    // pass the turn to the next player
    g.nextTurn();

    // check that the index of the current player is now 1 (Bob)
    currentIndex = g.currentPlayerIndex();
    CHECK(currentIndex == 1);
    // check that the current player is now Bob
    CHECK(g.turn() == "Bob");
    // check that bob is the only one with an action
    for(int i = 0; i < g.playersCount(); i++){
        if(i == currentIndex){
                CHECK(g.getPlayers()[i]->getRemainingActions() == 1);
        } 
        else {
            CHECK(g.getPlayers()[i]->getRemainingActions() == 0);
        }
    }

    // check that the index of the next player is 2 (Charlie)
    CHECK(g.nextPlayerIndex() == 2);
    // pass the turn to the next player
    g.nextTurn();

    // check that the index of the current player is now 2 (Charlie)
    currentIndex = g.currentPlayerIndex();
    CHECK(currentIndex == 2);
    // check that the current player is now Charlie
    CHECK(g.turn() == "Charlie");
    // check that alice is the only one with an action
    for(int i = 0; i < g.playersCount(); i++){
        if(i == currentIndex){
            CHECK(g.getPlayers()[i]->getRemainingActions() == 1);
        } 
        else {
            CHECK(g.getPlayers()[i]->getRemainingActions() == 0);
        }
    }

    // check that the index of the next player is 0 (Alice)
    CHECK(g.nextPlayerIndex() == 0);
    // pass the turn to the next player
    g.nextTurn();

    // check that the index of the current player is now 0 (Alice)
    currentIndex = g.currentPlayerIndex();
    CHECK(currentIndex == 0);
    // check that the current player is now Alice
    CHECK(g.turn() == "Alice");
    // check that alice is the only one with an action
    for(int i = 0; i < g.playersCount(); i++){
        if(i == currentIndex){
            CHECK(g.getPlayers()[i]->getRemainingActions() == 1);
        } 
        else {
            CHECK(g.getPlayers()[i]->getRemainingActions() == 0);
        }
    }
}

TEST_CASE("Player Removal"){
    SUBCASE("Removal of Current Player"){
        TestGame g = TestGame({"Alice", "Bob", "Charlie", "Dave"});

        // check that the game has 4 players
        CHECK(g.playersCount() == 4);

        // check that the current player is Alice
        CHECK(g.turn() == "Alice");

        // attempt to remove the current player (Alice)
        CHECK_THROWS(g.removePlayer(*g.getPlayers()[0])); 
    }
    SUBCASE("Removal of Non-Existent Player"){
        SUBCASE("Removal of Player with the Same Name as a Player in the Game") {
            // create a game with 4 players
            TestGame g = TestGame({"Alice", "Bob", "Charlie", "Dave"});

            // check that the game has 4 players
            CHECK(g.playersCount() == 4);

            // create a test player with the same name as a player in the game
            Player *testPlayer = new TestPlayer("Bob");

            // attempt to remove the player with the same name as a player in the game
            CHECK_THROWS(g.removePlayer(*testPlayer));

            // delete the test player
            delete testPlayer;
        }

        SUBCASE("Removal of Player with a Different Name") {
            // create a game with 4 players
            TestGame g = TestGame({"Alice", "Bob", "Charlie", "Dave"});

            // check that the game has 4 players
            CHECK(g.playersCount() == 4);

            // create a test player with a different name
            Player *testPlayer = new TestPlayer("Eve");

            // attempt to remove the player with a different name
            CHECK_THROWS(g.removePlayer(*testPlayer));

            // delete the test player
            delete testPlayer;
        }
    }
    SUBCASE("Removal of Non-Current Player"){
        SUBCASE("A Game with 2 Players with the Same Name"){
            // create a game with 2 players with the same name and 1 player with a different name
            TestGame g = TestGame({"Alice", "Bob", "Alice"});

            // check that the game has 3 players
            CHECK(g.playersCount() == 3);

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // remove the second Alice
            Player *secondAlice = g.getPlayers()[2];
            CHECK_NOTHROW(g.removePlayer(*secondAlice));

            // check that the game has 2 player left
            CHECK(g.playersCount() == 2);
            // check that the current player is still Alice
            CHECK(g.turn() == "Alice");

            // check that the only players left are Alice and Bob
            vector<string> expectedPlayers = {"Alice", "Bob"};
            CHECK(g.players() == expectedPlayers);
        }

        SUBCASE("A Game with Players of Different Names"){
            // create a game with 4 players
            TestGame g = TestGame({"Alice", "Bob", "Charlie", "Dave"});

            // get the vector of players
            const vector<Player *> &players = g.getPlayers();

            // check that the game has 4 players
            CHECK(g.playersCount() == 4);

            // check that the next player is bob
            CHECK(players[g.nextPlayerIndex()]->getName() == "Bob");

            // remove Bob from the game
            g.removePlayer(*g.getPlayers()[1]);


            // re-get the vector of players after removal
            const vector<Player *> leftPlayers = g.getPlayers();

            // check that the next player is now Charlie
            CHECK(leftPlayers[g.nextPlayerIndex()]->getName() == "Charlie");


            // check that the game has 3 players now
            CHECK(g.playersCount() == 3);
            // check that the current player is still Alice
            CHECK(g.turn() == "Alice");

            // check that the players are now Alice, Charlie and Dave
            vector<string> expectedPlayers = {"Alice", "Charlie", "Dave"};
            CHECK(g.players() == expectedPlayers);
        }

        SUBCASE("Removal of a Player that is the Last Arrested Player of Someone Else"){
            TestGame g = TestGame({"Alice", "Bob", "Charlie"});

            // have alice arrest charlie
            Player *alice = g.getPlayers()[0];
            alice->arrest(*g.getPlayers()[2]); // Alice arrests Charlie

            // check that Charlie is the last arrested player of Alice
            CHECK(alice->getLastArrested() == g.getPlayers()[2]);

            // remove Charlie from the game
            Player *charlie = g.getPlayers()[2];
            CHECK_NOTHROW(g.removePlayer(*charlie));

            // check that Charlie is no longer in the game
            CHECK(!g.isPlayerInGame(*charlie));

            // check that Alice's last arrested player is now null
            CHECK(alice->getLastArrested() == nullptr);

            // check that the game has 2 players left
            CHECK(g.playersCount() == 2);
        }

        SUBCASE("Remove a Player from a Game of 2"){
            // create a game with 2 players
            TestGame g = TestGame({"Alice", "Bob"});

            // check that the game has 2 players
            CHECK(g.playersCount() == 2);

            // remove Bob from the game
            Player *bob = g.getPlayers()[1];
            CHECK_NOTHROW(g.removePlayer(*bob));

            // check that the game has 1 player left
            CHECK(g.playersCount() == 1);
            // check that the current player is still Alice
            CHECK(g.turn() == "Alice");

            // check that the only player left is Alice
            vector<string> expectedPlayers = {"Alice"};
            CHECK(g.players() == expectedPlayers);
        }
    }
}

TEST_CASE("Test Actions"){
    // the in deapth testing of the actions themselves is done in the player classes tests
    // these are tests to check that the actions are working correctly in the game context
    // and that the game is handling and calling the correct actions
    SUBCASE("Test Gather Action"){
        TestGame g = TestGame({"Alice", "Bob"});

        // check that the current player is Alice
        CHECK(g.turn() == "Alice");

        // check that Alice has 0 coins
        CHECK(g.getPlayers()[0]->coins() == 0);

        // Alice performs the gather action
        int ret;
        CHECK_NOTHROW(ret = g.playAction("gather"));

        // check that the return value is -2 (not ending the turn)
        CHECK(ret == -2);

        // check that Alice has 1 coin after gathering
        CHECK(g.getPlayers()[0]->coins() == 1);

        // check that Alice has 0 actions left
        CHECK(g.getPlayers()[0]->getRemainingActions() == 0);
    }

    SUBCASE("Test Tax Action"){
        SUBCASE("Test Player Not Governor"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Judge", "Governor"});

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // check that Alice has 0 coins
            CHECK(g.getPlayers()[0]->coins() == 0);

            // Alice performs the tax action
            int ret;
            CHECK_NOTHROW(ret = g.playAction("tax"));

            // check that the return value is -2 (not ending the turn)
            CHECK(ret == -2);

            // check that Alice has 2 coins after taxing
            CHECK(g.getPlayers()[0]->coins() == 2);

            // check that Alice has 0 actions left
            CHECK(g.getPlayers()[0]->getRemainingActions() == 0);
        }
        SUBCASE("Test Player is Governor"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Governor", "Judge"});

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // check that Alice has 0 coins
            CHECK(g.getPlayers()[0]->coins() == 0);

            // Alice performs the tax action
            int ret;
            CHECK_NOTHROW(ret = g.playAction("tax"));

            // check that the return value is -2 (not ending the turn)
            CHECK(ret == -2);

            // check that Alice has 3 coins after taxing
            CHECK(g.getPlayers()[0]->coins() == 3);

            // check that Alice has 0 actions left
            CHECK(g.getPlayers()[0]->getRemainingActions() == 0);
        }
    }

    SUBCASE("Test Bribe Action"){
        TestGame g = TestGame({"Alice", "Bob"});

        // play until Alice has enough coins to bribe
        while(g.getPlayerByIndex(0)->coins() < 4){
            // have the player perform a tax action
            g.playAction("tax");

            // pass the turn to the next player
            g.nextTurn();
        }

        // check if the current player is Alice
        if(g.turn() != "Alice"){
            // if not, pass the turn to Alice
            g.nextTurn();
        }

        // save the amount of coins Alice has before the bribe
        int aliceCoins = g.getPlayers()[0]->coins();

        // check that Alice has 1 action left
        CHECK(g.getPlayers()[0]->getRemainingActions() == 1);

        // have alice perform the bribe action
        int ret;
        CHECK_NOTHROW(ret = g.playAction("bribe"));

        // check that the return value is -2 (not ending the turn)
        CHECK(ret == -2);

        // check that Alice has 4 less coins after bribing
        CHECK(g.getPlayers()[0]->coins() == aliceCoins - 4);

        // check that Alice has 2 actions now
        CHECK(g.getPlayers()[0]->getRemainingActions() == 2);
    }

    SUBCASE("Test Arrest Action"){
        SUBCASE("Test Arrest Passive Interactions"){
            SUBCASE("Interaction with a Merchant"){
                TestGame g = TestGame({"Alice", "Bob"}, {"Merchant", "Judge"});

                // have Alice perform the tax action
                CHECK_NOTHROW(g.playAction("tax"));

                // pass the turn to Bob
                g.nextTurn();

                // check that bob has no coins
                CHECK(g.getPlayers()[1]->coins() == 0);

                // have Bob arrest Alice
                int ret;
                CHECK_NOTHROW(ret = g.playAction("arrest", g.getPlayers()[0]));

                // check that the return value is -2 (not ending the turn)
                CHECK(ret == -2);

                // check that Bob has 0 actions left
                CHECK(g.getPlayers()[1]->getRemainingActions() == 0);

                // check that Bob still has no coins after arresting Alice
                CHECK(g.getPlayers()[1]->coins() == 0);

                // check that Alice now has 0 coins after being arrested
                CHECK(g.getPlayers()[0]->coins() == 0);

                // check that Alice is now the last arrested player of Bob
                CHECK(g.getPlayers()[1]->getLastArrested() == g.getPlayers()[0]);
            }
            SUBCASE("Interaction with a General"){
                TestGame g = TestGame({"Alice", "Bob"}, {"General", "Judge"});

                // have Alice perform the tax action
                CHECK_NOTHROW(g.playAction("tax"));

                // pass the turn to Bob
                g.nextTurn();

                // check that Bob has no coins
                CHECK(g.getPlayers()[1]->coins() == 0);

                // have Bob arrest Alice
                int ret;
                CHECK_NOTHROW(ret = g.playAction("arrest", g.getPlayers()[0]));

                // check that the return value is -2 (not ending the turn)
                CHECK(ret == -2);

                // check that Bob has 0 actions left
                CHECK(g.getPlayers()[1]->getRemainingActions() == 0);

                // check that Bob still has no coins after arresting Alice
                CHECK(g.getPlayers()[1]->coins() == 0);

                // check that Alice still has 2 coins after being arrested
                CHECK(g.getPlayers()[0]->coins() == 2);

                // check that Alice is now the last arrested player of Bob
                CHECK(g.getPlayers()[1]->getLastArrested() == g.getPlayers()[0]);
            }
        }
        SUBCASE("Test Arrest Player Not in Game"){
            TestGame g = TestGame({"Alice", "Bob"});

            // create a test player that is not in the game
            Player *testPlayer = new TestPlayer("Charlie");

            // make sure the test player is not in the game
            CHECK(!g.isPlayerInGame(*testPlayer));

            // make Alice attempt to arrest the test player
            CHECK_THROWS(g.playAction("arrest", testPlayer));

            // delete the test player
            delete testPlayer;
        }
        SUBCASE("Test Arrest Null Player"){
            TestGame g = TestGame({"Alice", "Bob"});

            // have Alice attempt to arrest a null player
            CHECK_THROWS(g.playAction("arrest", nullptr));
        }
        SUBCASE("Test Successful Arrest"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Judge", "Governor"});

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // have Alice perform the tax action
            CHECK_NOTHROW(g.playAction("tax"));

            // pass the turn to Bob
            g.nextTurn();

            // check that bob has no coins
            CHECK(g.getPlayers()[1]->coins() == 0);

            // have Bob arrest Alice
            int ret;
            CHECK_NOTHROW(ret = g.playAction("arrest", g.getPlayers()[0]));

            // check that the return value is -2 (not ending the turn)
            CHECK(ret == -2);

            // check that Bob has 0 actions left
            CHECK(g.getPlayers()[1]->getRemainingActions() == 0);

            // check that Bob has 1 coin after arresting Alice
            CHECK(g.getPlayers()[1]->coins() == 1);

            // check that Alice is now the last arrested player of Bob
            CHECK(g.getPlayers()[1]->getLastArrested() == g.getPlayers()[0]);
        }
        SUBCASE("Test Arrest Twice in a Row"){
            TestGame g = TestGame({"Alice", "Bob"});

            // have Alice Arrest Bob
            CHECK_NOTHROW(g.playAction("arrest", g.getPlayers()[1]));

            // pass the turn to Bob
            g.nextTurn();
            // pass the turn to Alice
            g.nextTurn();

            // have Alice attempt to arrest Bob again
            CHECK_THROWS(g.playAction("arrest", g.getPlayers()[1]));
        }
        SUBCASE("Test Arrest Two Different Players"){
            TestGame g = TestGame({"Alice", "Bob", "Charlie"});

            // have Alice arrest Bob
            CHECK_NOTHROW(g.playAction("arrest", g.getPlayers()[1]));

            // check that Bob is now the last arrested player of Alice
            CHECK(g.getPlayers()[0]->getLastArrested() == g.getPlayers()[1]);

            // pass the turn to Bob
            g.nextTurn();

            // pass the turn to Charlie
            g.nextTurn();

            // pass the turn to Alice
            g.nextTurn();

            // have Alice arrest Charlie
            CHECK_NOTHROW(g.playAction("arrest", g.getPlayers()[2]));

            // check that Charlie is now the last arrested player of Alice
            CHECK(g.getPlayers()[0]->getLastArrested() == g.getPlayers()[2]);
        }
    }

    SUBCASE("Test Sanction Action"){
        SUBCASE("Test Sanction Passive Interactions"){
            SUBCASE("Test Sanction a Judge"){
                TestGame g = TestGame({"Alice", "Bob"}, {"Baron", "Judge"});

                // have Alice perform the tax action
                CHECK_NOTHROW(g.playAction("tax"));
                // pass the turn back to Alice
                g.nextTurn();
                g.nextTurn();
                // have Alice perform the gather action
                CHECK_NOTHROW(g.playAction("gather"));

                // check that Alice has 3 coins
                CHECK(g.getPlayers()[0]->coins() == 3);

                // pass the turn back to Alice
                g.nextTurn();
                g.nextTurn();


                // have Alice attempt to sanction Bob
                CHECK_THROWS(g.playAction("sanction", g.getPlayers()[1]));

                // pass the turn back to Alice
                g.nextTurn();
                g.nextTurn();

                // have Alice perform the gather action
                CHECK_NOTHROW(g.playAction("gather"));

                // check that Alice has 4 coins
                CHECK(g.getPlayers()[0]->coins() == 4);

                // pass the turn back to Alice
                g.nextTurn();
                g.nextTurn();

                // have Alice sanction Bob
                int ret;
                CHECK_NOTHROW(ret = g.playAction("sanction", g.getPlayers()[1]));

                // check that the return value is -2 (not ending the turn)
                CHECK(ret == -2);

                // check that Alice has 0 coins after sanctioning Bob
                CHECK(g.getPlayers()[0]->coins() == 0);
            }
            SUBCASE("Test Sanction a Baron"){
                TestGame g = TestGame({"Alice", "Bob"}, {"Judge", "Baron"});

                // have Alice perform the tax action
                CHECK_NOTHROW(g.playAction("tax"));
                // pass the turn back to Alice
                g.nextTurn();
                g.nextTurn();
                // have Alice perform the gather action
                CHECK_NOTHROW(g.playAction("gather"));
                // pass the turn back to Alice
                g.nextTurn();
                g.nextTurn();

                // check that Alice has 3 coins
                CHECK(g.getPlayers()[0]->coins() == 3);

                // check that Bob has 0 coins
                CHECK(g.getPlayers()[1]->coins() == 0);

                // have Alice sanction Bob
                int ret;
                CHECK_NOTHROW(ret = g.playAction("sanction", g.getPlayers()[1]));

                // check that the return value is -2 (not ending the turn)
                CHECK(ret == -2);

                // check that Alice has 0 coins after sanctioning Bob
                CHECK(g.getPlayers()[0]->coins() == 0);

                // check that Bob has 1 coin after being sanctioned
                CHECK(g.getPlayers()[1]->coins() == 1);
            }
        }
        SUBCASE("Test Sanction Player Not in Game"){
            TestGame g = TestGame({"Alice", "Bob"});

            // have Alice perform the tax action
            CHECK_NOTHROW(g.playAction("tax"));
            // pass the turn back to Alice
            g.nextTurn();
            g.nextTurn();
            // have Alice perform the tax action again
            CHECK_NOTHROW(g.playAction("tax"));
            // pass the turn back to Alice
            g.nextTurn();
            g.nextTurn();

            // create a test player that is not in the game
            Player *testPlayer = new TestPlayer("Charlie");

            // make sure the test player is not in the game
            CHECK(!g.isPlayerInGame(*testPlayer));

            // make Alice attempt to sanction the test player
            CHECK_THROWS(g.playAction("sanction", testPlayer));

            // delete the test player
            delete testPlayer;
        }
        SUBCASE("Test Sanction Null Player"){
            TestGame g = TestGame({"Alice", "Bob"});

            // have Alice attempt to sanction a null player
            CHECK_THROWS(g.playAction("sanction", nullptr));
        }
        SUBCASE("Test Successful Sanction"){
            TestGame g = TestGame({"Alice", "Bob"});

            // have Alice perform the tax action
            CHECK_NOTHROW(g.playAction("tax"));
            // pass the turn back to Alice
            g.nextTurn();
            g.nextTurn();
            // have Alice perform the tax action
            CHECK_NOTHROW(g.playAction("tax"));
            // pass the turn back to Alice
            g.nextTurn();
            g.nextTurn();

            // check that Alice has at least 4 coins
            CHECK(g.getPlayers()[0]->coins() >= 4);

            // have Alice sanction Bob
            int ret;
            CHECK_NOTHROW(ret = g.playAction("sanction", g.getPlayers()[1]));

            // check that the return value is -2 (not ending the turn)
            CHECK(ret == -2);

            // pass the turn to Bob
            g.nextTurn();

            // check that Bob has an action left
            CHECK(g.getPlayers()[1]->getRemainingActions() == 1);

            // have Bob attempt to perform the gather action
            CHECK_THROWS(g.playAction("gather"));

            // check that Bob has an action left
            CHECK(g.getPlayers()[1]->getRemainingActions() == 1);

            // have Bob attempt to perform the tax action
            CHECK_THROWS(g.playAction("tax"));

            // check that Bob has an action left
            CHECK(g.getPlayers()[1]->getRemainingActions() == 1);
        }
    }

    SUBCASE("Test Coup Action"){
        SUBCASE("Test Coup Player Not in Game"){
            TestGame g = TestGame({"Alice", "Bob"});

            // give Alice enough coins to perform a coup
            while(g.getPlayers()[0]->coins() < 7){
                // have the player perform a tax action
                g.playAction("tax");

                // pass the turn to the next player
                g.nextTurn();
            }

            // check that the current player is Alice
            if(g.turn() != "Alice"){
                // if not, pass the turn to Alice
                g.nextTurn();
            }

            // create a test player that is not in the game
            Player *testPlayer = new TestPlayer("Charlie");

            // make sure the test player is not in the game
            CHECK(!g.isPlayerInGame(*testPlayer));

            // make Alice attempt to coup the test player
            CHECK_THROWS(g.playAction("coup", testPlayer));

            // delete the test player
            delete testPlayer;
        }
        SUBCASE("Test Coup Null Player"){
            TestGame g = TestGame({"Alice", "Bob"});

            // give Alice enough coins to perform a coup
            while(g.getPlayers()[0]->coins() < 7){
                // have the player perform a tax action
                g.playAction("tax");

                // pass the turn to the next player
                g.nextTurn();
            }

            // check that the current player is Alice
            if(g.turn() != "Alice"){
                // if not, pass the turn to Alice
                g.nextTurn();
            }

            // have Alice attempt to coup a null player
            CHECK_THROWS(g.playAction("coup", nullptr));
        }
        SUBCASE("Test Coup Player with Undo True"){
            TestGame g = TestGame({"Alice", "Bob"});

            // give Alice enough coins to perform a coup
            while(g.getPlayers()[0]->coins() < 7){
                // have the player perform a tax action
                g.playAction("tax");

                // pass the turn to the next player
                g.nextTurn();
            }

            // check that the current player is Alice
            if(g.turn() != "Alice"){
                // if not, pass the turn to Alice
                g.nextTurn();
            }

            // have Alice perform a coup on Bob, with undo as true
            CHECK_THROWS(g.playAction("coup", g.getPlayers()[1], true));

            // check that Bob is still in the game
            CHECK(g.playersCount() == 2);
            CHECK(g.getPlayerByIndex(1)->getName() == "Bob"); // Bob should still be in the game

        }
        SUBCASE("Test Coup Player with Undo False"){
            TestGame g = TestGame({"Alice", "Bob"});

            // give Alice enough coins to perform a coup
            while(g.getPlayers()[0]->coins() < 7){
                // have the player perform a tax action
                g.playAction("tax");

                // pass the turn to the next player
                g.nextTurn();
            }

            // check that the current player is Alice
            if(g.turn() != "Alice"){
                // if not, pass the turn to Alice
                g.nextTurn();
            }

            // have Alice perform a coup on Bob, with undo as false
            int ret;
            CHECK_NOTHROW(ret = g.playAction("coup", g.getPlayers()[1], false));

            // check that the return value is -2 (not ending the turn)
            CHECK(ret == -2);
            
            // check that Bob is no longer in the game
            CHECK(g.playersCount() == 1);
            CHECK(g.getPlayerByIndex(0)->getName() == "Alice"); // Alice should still be in the game
            CHECK_THROWS(g.getPlayerByIndex(1)); // should throw an exception since Bob is no longer in the game
        }
    }

    SUBCASE("Test EndTurn Action"){
        SUBCASE("Test End Turn Actions Left"){
            TestGame g = TestGame({"Alice", "Bob"});
        
            // check that it's Alice's turn
            CHECK(g.turn() == "Alice");

            // check that Alice has 1 action left
            CHECK(g.getPlayers()[0]->getRemainingActions() == 1);

            // have Alice end her turn
            int ret;
            CHECK_NOTHROW(ret = g.playAction("endTurn"));

            // check that the return value is -1 (ending the turn)
            CHECK(ret == -1);

            // check that it's Bob's turn
            CHECK(g.turn() == "Bob");

            // check that Bob has 1 actions left
            CHECK(g.getPlayers()[1]->getRemainingActions() == 1);

            // check that Alice has 0 actions left
            CHECK(g.getPlayers()[0]->getRemainingActions() == 0);
        }
        SUBCASE("Test End Turn No Actions Left"){
            TestGame g = TestGame({"Alice", "Bob"});
        
            // check that it's Alice's turn
            CHECK(g.turn() == "Alice");

            // check that Alice has 1 action left
            CHECK(g.getPlayers()[0]->getRemainingActions() == 1);

            // have Alice perform an action to use up her action
            CHECK_NOTHROW(g.playAction("gather"));

            // check that Alice has 0 actions left
            CHECK(g.getPlayers()[0]->getRemainingActions() == 0);

            // have Alice end her turn
            int ret;
            CHECK_NOTHROW(ret = g.playAction("endTurn"));

            // check that the return value is -1 (ending the turn)
            CHECK(ret == -1);

            // check that it's Bob's turn
            CHECK(g.turn() == "Bob");

            // check that Bob has 1 actions left
            CHECK(g.getPlayers()[1]->getRemainingActions() == 1);

            // check that Alice has 0 actions left
            CHECK(g.getPlayers()[0]->getRemainingActions() == 0);
        }
    }

    SUBCASE("Test Invest Action"){
        SUBCASE("Test Invest On Not Baron"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Judge", "Governor"});

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // check that Alice has 0 coins
            CHECK(g.getPlayers()[0]->coins() == 0);

            // give Alice enough coins to perform the invest action
            while(g.getPlayers()[0]->coins() < 3){
                // have the player perform a tax action
                g.playAction("tax");

                // pass the turn to the next player
                g.nextTurn();
            }
            
            // if Alice is not the current player, pass the turn to her
            if(g.turn() != "Alice"){
                g.nextTurn();
            }

            // check that Alice has at least 3 coins
            CHECK(g.getPlayers()[0]->coins() >= 3);

            // have Alice attempt to perfrom the invest action
            CHECK_THROWS(g.playAction("invest"));
        }
        SUBCASE("Test Invest On Baron"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Baron", "Governor"});

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // check that Alice has 0 coins
            CHECK(g.getPlayers()[0]->coins() == 0);

            // give Alice enough coins to perform the invest action
            while(g.getPlayers()[0]->coins() < 3){
                // have the player perform a tax action
                g.playAction("tax");

                // pass the turn to the next player
                g.nextTurn();
            }
            
            // if Alice is not the current player, pass the turn to her
            if(g.turn() != "Alice"){
                g.nextTurn();
            }

            // check that Alice has at least 3 coins
            CHECK(g.getPlayers()[0]->coins() >= 3);

            // save the amount of coins Alice has before investing
            int aliceCoins = g.getPlayers()[0]->coins();

            // have Alice attempt to perfrom the invest action
            int ret;
            CHECK_NOTHROW(ret = g.playAction("invest"));

            // check that the return value is -2 (not ending the turn)
            CHECK(ret == -2);

            // check that Alice has 3 more coins after investing
            CHECK(g.getPlayers()[0]->coins() == aliceCoins + 3);

            // check that Alice has 0 actions left
            CHECK(g.getPlayers()[0]->getRemainingActions() == 0);
        }
    }

    SUBCASE("Test SpyOn Action"){
        SUBCASE("Test Spy On as Not Spy"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Judge", "Governor"});
            
            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // have Alice attempt to perform the spyOn action
            CHECK_THROWS(g.playAction("spyOn", g.getPlayers()[1]));
        }
        SUBCASE("Test Spy On Null"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Spy", "Judge"});

            // have Alice attempt to spy on a null player
            CHECK_THROWS(g.playAction("spyOn", nullptr));
        }
        SUBCASE("Test Spy On Player Not in Game"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Spy", "Judge"});

            // create a test player that is not in the game
            Player *testPlayer = new TestPlayer("Charlie");

            // make sure the test player is not in the game
            CHECK(!g.isPlayerInGame(*testPlayer));

            // make Alice attempt to spy on the test player
            CHECK_THROWS(g.playAction("spyOn", testPlayer));

            // delete the test player
            delete testPlayer;
        }
        SUBCASE("Test Successful Spy On"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Spy", "Judge"});

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // have Alice perform the tax action
            g.playAction("tax");

            // pass the turn to Bob
            g.playAction("endTurn");

            // check that Bob has 0 coins
            CHECK(g.getPlayers()[1]->coins() == 0);

            // have Bob play the tax action
            g.playAction("tax");

            // save the amount of coins Bob 
            int bobCoins = g.getPlayers()[1]->coins();

            // pass the turn to Alice
            g.playAction("endTurn");

            // have Alice spy on Bob
            int ret;
            CHECK_NOTHROW(ret = g.playAction("spyOn", g.getPlayers()[1]));

            // ret should be equal to the amount of coins Bob has
            CHECK(ret == bobCoins);
        }
    }

    SUBCASE("Test Block Arrest Action"){
        SUBCASE("Test Block Arrest as Not Spy"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Judge", "Governor"});

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // have Alice attempt to block the arrest action
            CHECK_THROWS(g.playAction("blockArrest", g.getPlayers()[1]));
        }
        SUBCASE("Test Block Arrest Player Not in Game"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Spy", "Judge"});

            // create a test player that is not in the game
            Player *testPlayer = new TestPlayer("Charlie");

            // make sure the test player is not in the game
            CHECK(!g.isPlayerInGame(*testPlayer));

            // make Alice attempt to block the arrest of the test player
            CHECK_THROWS(g.playAction("blockArrest", testPlayer));

            // delete the test player
            delete testPlayer;
        }
        SUBCASE("Test Block Arrest Null Player"){
            TestGame g = TestGame({"Alice", "Bob"});

            // have Alice attempt to block the arrest of a null player
            CHECK_THROWS(g.playAction("blockArrest", nullptr));
        }
        SUBCASE("Test Successful Block Arrest"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Spy", "Baron"});

            // have Alice perform the block arrest action on Bob
            int ret;
            CHECK_NOTHROW(ret = g.playAction("blockArrest", g.getPlayers()[1]));

            // check that the return value is -2 (not ending the turn)
            CHECK(ret == -2);

            // check that Alice has 1 actions left (as the block arrest action does not cost an action)
            CHECK(g.getPlayers()[0]->getRemainingActions() == 1);

            // pass the turn to Bob
            g.playAction("endTurn");

            // check that Bob has 1 action left
            CHECK(g.getPlayers()[1]->getRemainingActions() == 1);

            // have Bob attempt to perform the arrest action on Alice
            CHECK_THROWS(g.playAction("arrest", g.getPlayers()[0]));
        }
    }

    SUBCASE("Test Invalid Action"){
        TestGame g = TestGame({"Alice", "Bob"});

        // check that the current player is Alice
        CHECK(g.turn() == "Alice");

        // attempt to play an invalid action
        CHECK_THROWS(g.playAction("invalidAction"));
        
        // check that the current player still has 1 action left
        CHECK(g.getPlayers()[0]->getRemainingActions() == 1);
    }

    SUBCASE("Test Combinations"){
        SUBCASE("Sanctioned Player Plays Gather"){
            TestGame g = TestGame({"Alice", "Bob"});

            // have the players perform the gather action until Bob has enough coins to sanction Alice
            while(g.getPlayers()[1]->coins() < 4){
                // have the player perform a gather action
                g.playAction("gather");

                // pass the turn to the next player
                g.nextTurn();
            }

            // check that the current player is Bob
            if(g.turn() != "Bob"){
                // if not, pass the turn to Bob
                g.nextTurn();
            }

            // have bob sanction Alice
            CHECK_NOTHROW(g.playAction("sanction", g.getPlayers()[0]));

            // have alice attempt to play the gather action
            CHECK_THROWS(g.playAction("gather"));

            // end Alice's turn
            g.playAction("endTurn");
            // pass the turn back to Alice
            g.nextTurn();

            // have Alice attempt to play the gather action
            CHECK_NOTHROW(g.playAction("gather"));
        }
        SUBCASE("Sanctioned Player Plays Tax"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Governor", "Judge"});

            // have the players perform the gather action until Bob has enough coins to sanction Alice
            while(g.getPlayers()[1]->coins() < 4){
                // have the player perform a gather action
                g.playAction("gather");

                // pass the turn to the next player
                g.nextTurn();
            }

            // check that the current player is Bob
            if(g.turn() != "Bob"){
                // if not, pass the turn to Bob
                g.nextTurn();
            }

            // have bob sanction Alice
            CHECK_NOTHROW(g.playAction("sanction", g.getPlayers()[0]));

            // have alice attempt to play the tax action
            CHECK_THROWS(g.playAction("tax"));

            // end Alice's turn
            g.playAction("endTurn");
            // pass the turn back to Alice
            g.nextTurn();

            // have Alice attempt to play the tax action
            CHECK_NOTHROW(g.playAction("tax"));
        }
        SUBCASE("Test Forced Coup With At Least 10 Coins"){
            
            SUBCASE("Test Regular Actions"){
                TestGame g = TestGame({"Alice", "Bob"}, {"Judge", "Governor"});

                // have the players perform the gather action until Alice has 10 coins
                while(g.getPlayers()[0]->coins() < 10){
                    // have the player perform a gather action
                    g.playAction("gather");

                    // pass the turn to the next player
                    g.nextTurn();
                }

                // check that the current player is Alice
                if(g.turn() != "Alice"){
                    // if not, pass the turn to her
                    g.nextTurn();
                }

                // check that Alice has at least 10 coins
                CHECK(g.getPlayers()[0]->coins() >= 10);

                // check that Alice has 1 action left
                CHECK(g.getPlayers()[0]->getRemainingActions() == 1);

                // have Alice attempt to perform any action other than coup

                // attempt gather action
                CHECK_THROWS(g.playAction("gather"));
                // check that Alice still has 1 action left
                CHECK(g.getPlayers()[0]->getRemainingActions() == 1);
                // attempt tax action
                CHECK_THROWS(g.playAction("tax"));
                // check that Alice still has 1 action left
                CHECK(g.getPlayers()[0]->getRemainingActions() == 1);
                // attempt bribe action
                CHECK_THROWS(g.playAction("bribe"));
                // check that Alice still has 1 action left
                CHECK(g.getPlayers()[0]->getRemainingActions() == 1);
                // attempt arrest action
                CHECK_THROWS(g.playAction("arrest", g.getPlayers()[1]));
                // check that Alice still has 1 action left
                CHECK(g.getPlayers()[0]->getRemainingActions() == 1);
                // attempt sanction action
                CHECK_THROWS(g.playAction("sanction", g.getPlayers()[1]));
                // check that Alice still has 1 action left
                CHECK(g.getPlayers()[0]->getRemainingActions() == 1);
                // attempt endTurn action
                CHECK_THROWS(g.playAction("endTurn"));
                // check that Alice still has 1 action left
                CHECK(g.getPlayers()[0]->getRemainingActions() == 1);

                // have Alice perform a coup on Bob
                CHECK_NOTHROW(g.playAction("coup", g.getPlayers()[1]));

                // check that Alice is the only player left in the game
                CHECK(g.playersCount() == 1);
                CHECK(g.getPlayerByIndex(0)->getName() == "Alice"); // Alice should still be in the game
            }
        }
        SUBCASE("Test Special Actions"){
            SUBCASE("Invest"){
                TestGame g = TestGame({"Alice", "Bob"}, {"Baron", "Governor"});

                // have the players perform the gather action until Alice has 10 coins
                while(g.getPlayers()[0]->coins() < 10){
                    // have the player perform a gather action
                    g.playAction("gather");

                    // pass the turn to the next player
                    g.nextTurn();
                }

                // check that the current player is Alice
                if(g.turn() != "Alice"){
                    // if not, pass the turn to her
                    g.nextTurn();
                }

                // check that Alice has at least 10 coins
                CHECK(g.getPlayers()[0]->coins() >= 10);

                // check that Alice has 1 action left
                CHECK(g.getPlayers()[0]->getRemainingActions() == 1);

                // since Alice is a Baron and she has enough money to invest, the action is valid for her
                // but it will force a coup
                CHECK_THROWS(g.playAction("invest"));
            }
            SUBCASE("SpyOn"){
                TestGame g = TestGame({"Alice", "Bob"}, {"Spy", "Governor"});

                // have the players perform the gather action until Alice has 10 coins
                while(g.getPlayers()[0]->coins() < 10){
                    // have the player perform a gather action
                    g.playAction("gather");

                    // pass the turn to the next player
                    g.nextTurn();
                }

                // check that the current player is Alice
                if(g.turn() != "Alice"){
                    // if not, pass the turn to her
                    g.nextTurn();
                }

                // check that Alice has at least 10 coins
                CHECK(g.getPlayers()[0]->coins() >= 10);

                // check that Alice has 1 action left
                CHECK(g.getPlayers()[0]->getRemainingActions() == 1);

                // since Alice is a Spy, the action is valid for her
                // but it will force a coup
                CHECK_THROWS(g.playAction("spyOn", g.getPlayers()[1]));
            }
            SUBCASE("Block Arrest"){
                TestGame g = TestGame({"Alice", "Bob"}, {"Spy", "Governor"});

                // have the players perform the gather action until Alice has 10 coins
                while(g.getPlayers()[0]->coins() < 10){
                    // have the player perform a gather action
                    g.playAction("gather");

                    // pass the turn to the next player
                    g.nextTurn();
                }

                // check that the current player is Alice
                if(g.turn() != "Alice"){
                    // if not, pass the turn to her
                    g.nextTurn();
                }

                // check that Alice has at least 10 coins
                CHECK(g.getPlayers()[0]->coins() >= 10);

                // check that Alice has 1 action left
                CHECK(g.getPlayers()[0]->getRemainingActions() == 1);

                // since Alice is a Spy, the action is valid for her
                // but it will force a coup
                CHECK_THROWS(g.playAction("blockArrest", g.getPlayers()[1]));
            }
        }
    }
}

TEST_CASE("Test Performing an Action After Game Ends"){
    TestGame g = TestGame({"Alice", "Bob"}, {"Governor", "Judge"});

    // have the players perform the gather action until Alice has enough coins to coup Bob
    while(g.getPlayers()[0]->coins() < 7){
        // have the player perform a gather action
        g.playAction("gather");

        // pass the turn to the next player
        g.nextTurn();
    }

    // check that the current player is Alice
    if(g.turn() != "Alice"){
        // if not, pass the turn to her
        g.nextTurn();
    }

    // have Alice perform a coup on Bob
    g.playAction("coup", g.getPlayers()[1]);

    // check that Bob is no longer in the game
    CHECK(g.playersCount() == 1);
    CHECK(g.getPlayerByIndex(0)->getName() == "Alice"); // Alice should still be in the game

    // have Alice attempt to perform an action after the game has ended
    CHECK_THROWS(g.playAction("gather"));

    CHECK_THROWS(g.playAction("endTurn"));
}

TEST_CASE("Test Game Undo Actions"){
    SUBCASE("Test Undo Tax Action"){
        SUBCASE("Test Valid Undo Tax Action"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Governor", "Judge"});

            // pass the turn to Bob
            g.playAction("endTurn");

            // have Bob perform the tax action
            g.playAction("tax");

            // check that Bob has 2 coins after taxing
            CHECK(g.getPlayers()[1]->coins() == 2);

            // check that Bob has 0 actions left after taxing
            CHECK(g.getPlayers()[1]->getRemainingActions() == 0);

            // have Alice undo Bob's tax action
            CHECK_NOTHROW(g.undoAction(*g.getPlayers()[0], "undoTax"));

            // check that Bob has 0 coins after undoing the tax action
            CHECK(g.getPlayers()[1]->coins() == 0);

            // check that Bob has 0 action left after undoing the tax action
            CHECK(g.getPlayers()[1]->getRemainingActions() == 0);
        }
        SUBCASE("Test Undo Own's Tax Action"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Governor", "Judge"});

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // have Alice perform the tax action
            CHECK_NOTHROW(g.playAction("tax"));

            // check that Alice has 3 coins after taxing
            CHECK(g.getPlayers()[0]->coins() == 3);

            // have Alice undo her tax action
            CHECK_THROWS(g.undoAction(*g.getPlayers()[0], "undoTax"));
        }
        SUBCASE("Test Undo Tax on Player Who's Last Action Was Not Tax"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Governor", "Judge"});

            // have Alice pass the turn to Bob
            g.playAction("endTurn");

            // have Bob perform the gather action
            g.playAction("gather");

            // have Alice attempt to undo Bob's last action
            CHECK_THROWS(g.undoAction(*g.getPlayers()[1], "undoTax"));
        }
    }
    SUBCASE("Test Undo Bribe Action"){
        SUBCASE("Test Valid Undo Bribe Action"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Baron", "Judge"});

            // have the players perform the gather action until Alice has enough coins to bribe
            while(g.getPlayers()[0]->coins() < 4){
                // have the player perform a gather action
                g.playAction("gather");

                // pass the turn to the next player
                g.nextTurn();
            }

            // check that the current player is Alice
            if(g.turn() != "Alice"){
                // if not, pass the turn to her
                g.nextTurn();
            }

            // save the amount of coins Alice has before the bribe
            int aliceCoins = g.getPlayers()[0]->coins();

            // have Alice perform the bribe action
            CHECK_NOTHROW(g.playAction("bribe"));

            // check that Alice has 4 less coins after bribing
            CHECK(g.getPlayers()[0]->coins() == aliceCoins - 4);

            // check that Alice has 2 actions now
            CHECK(g.getPlayers()[0]->getRemainingActions() == 2);

            // have Bob undo Alice's bribe action
            CHECK_NOTHROW(g.undoAction(*g.getPlayers()[1], "undoBribe"));

            // check that Alice still has 4 less coins after undoing the bribe
            CHECK(g.getPlayers()[0]->coins() == aliceCoins - 4);

            // check that Alice has 1 action left after undoing the bribe
            CHECK(g.getPlayers()[0]->getRemainingActions() == 1);
        }
        SUBCASE("Test Undo Own's Bribe Action"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Judge", "Judge"});

            // have the players perform the gather action until Alice has enough coins to bribe
            while(g.getPlayers()[0]->coins() < 4){
                // have the player perform a gather action
                g.playAction("gather");

                // pass the turn to the next player
                g.nextTurn();
            }

            // check that the current player is Alice
            if(g.turn() != "Alice"){
                // if not, pass the turn to her
                g.nextTurn();
            }

            // have Alice perform the bribe action
            CHECK_NOTHROW(g.playAction("bribe"));

            // have Alice undo her own bribe action
            CHECK_THROWS(g.undoAction(*g.getPlayers()[0], "undoBribe"));
        }
        SUBCASE("Test Undo Bribe on Player Who's Last Action Was Not Bribe"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Judge", "Judge"});

            // have the players perform the gather action until Alice has enough coins to bribe
            while(g.getPlayers()[0]->coins() < 4){
                // have the player perform a gather action
                g.playAction("gather");

                // pass the turn to the next player
                g.nextTurn();
            }

            // check that the current player is Alice
            if(g.turn() != "Alice"){
                // if not, pass the turn to her
                g.nextTurn();
            }

            // have Alice perform the gather action
            CHECK_NOTHROW(g.playAction("gather"));

            // have Bob attempt to undo Alice's bribe action
            CHECK_THROWS(g.undoAction(*g.getPlayers()[1], "undoBribe"));
        }
    }
    SUBCASE("Test Undo Coup Action"){
        // since the coup action is "undone" before the player is removed from the game,
        // the undo coup action is done in a special way

        SUBCASE("Test Valid Undo Coup Action"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Baron", "General"});
            // have the players perform the gather action until Alice has enough coins to coup
            while(g.getPlayers()[0]->coins() < 7){
                // have the player perform a gather action
                g.playAction("gather");

                // pass the turn to the next player
                g.playAction("endTurn");
            }

            // check that the current player is Alice
            if(g.turn() != "Alice"){
                // if not, pass the turn to her
                g.playAction("endTurn");
            }

            // check that alices coup is valid (which will cause her last action to be coup)
            CHECK(g.isValidCoup(g.getPlayers()[1]));

            // have Bob undo Alice's coup action
            CHECK_NOTHROW(g.undoAction(*g.getPlayers()[1], "undoCoup"));
                
            // have Alice perform the coup action on Bob with the undo flag set to true
            CHECK_THROWS(g.playAction("coup", g.getPlayers()[1], true));

            // check that Bob is still in the game
            CHECK(g.playersCount() == 2);
            CHECK(g.getPlayerByIndex(1)->getName() == "Bob"); // Bob should still be in the game
        }
        SUBCASE("Test Undo Own's Coup Action"){
            TestGame g = TestGame({"Alice", "Bob"}, {"General", "Baron"});
            // have the players perform the gather action until Alice has enough coins to coup
            while(g.getPlayers()[0]->coins() < 7){
                // have the player perform a gather action
                g.playAction("gather");

                // pass the turn to the next player
                g.playAction("endTurn");
            }

            // check that the current player is Alice
            if(g.turn() != "Alice"){
                // if not, pass the turn to her
                g.playAction("endTurn");
            }

            // check that alices coup is valid (which will cause her last action to be coup)
            CHECK(g.isValidCoup(g.getPlayers()[1]));

            // have Alice attempt undo her own coup action
            CHECK_THROWS(g.undoAction(*g.getPlayers()[0], "undoCoup"));
        }
        SUBCASE("Test Undo Coup on Player Who's Last Action Was Not Coup"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Baron", "General"});
            // have the players perform the gather action until Alice has enough coins to coup
            while(g.getPlayers()[0]->coins() < 7){
                // have the player perform a gather action
                g.playAction("gather");

                // pass the turn to the next player
                g.playAction("endTurn");
            }

            // check that the current player is Alice
            if(g.turn() != "Alice"){
                // if not, pass the turn to her
                g.playAction("endTurn");
            }

            // have Alice perform the gather action
            g.playAction("gather");

            // have Bob undo Alice's coup action
            CHECK_THROWS(g.undoAction(*g.getPlayers()[1], "undoCoup"));
        }
    }
}

TEST_CASE("Test Game Winner"){
    SUBCASE("Test Game Ended"){
        TestGame g = TestGame({"Alice", "Bob"});

        // run the game until alice has enough coins to coup bob
        while(g.getPlayers()[0]->coins() < 7){
            // have the player perform a gather action
            g.playAction("gather");
    
            // pass the turn to the next player
            g.nextTurn();
        }
    
        // check that the current player is Alice
        if(g.turn() != "Alice"){
            // if not, pass the turn to Alice
            g.nextTurn();
        }

        // have Alice perform a coup on Bob
        CHECK_NOTHROW(g.playAction("coup", g.getPlayers()[1], false));

        // check that Alice is the only player left in the game
        CHECK(g.playersCount() == 1);

        // check that winner does not throw an exception
        CHECK_NOTHROW(g.winner());

        // check that the winner is Alice
        CHECK(g.winner() == "Alice");
    }
    SUBCASE("Test Game Not Ended"){
        TestGame g = TestGame({"Alice", "Bob"});

        // run the game for a few turns 
        while(g.getPlayers()[0]->coins() < 4){
            // have the player perform a gather action
            g.playAction("gather");
    
            // pass the turn to the next player
            g.nextTurn();
        }

        // check the winner
        CHECK_THROWS(g.winner());
    }
    
}
