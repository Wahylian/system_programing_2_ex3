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
            // create a game with 3 players with the same name
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
        CHECK_NOTHROW(g.playAction("gather"));

        // check that Alice has 1 coin after gathering
        CHECK(g.getPlayers()[0]->coins() == 1);

        // check that Alice has 0 actions left
        CHECK(g.getPlayers()[0]->getRemainingActions() == 0);
    }

    SUBCASE("Test Tax Action"){
        SUBCASE("Test Alice Not Governor"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Judge", "Governor"});

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // check that Alice has 0 coins
            CHECK(g.getPlayers()[0]->coins() == 0);

            // Alice performs the tax action
            CHECK_NOTHROW(g.playAction("tax"));

            // check that Alice has 2 coins after taxing
            CHECK(g.getPlayers()[0]->coins() == 2);

            // check that Alice has 0 actions left
            CHECK(g.getPlayers()[0]->getRemainingActions() == 0);
        }
        SUBCASE("Test Alice is Governor"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Governor", "Judge"});

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // check that Alice has 0 coins
            CHECK(g.getPlayers()[0]->coins() == 0);

            // Alice performs the tax action
            CHECK_NOTHROW(g.playAction("tax"));

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
        CHECK_NOTHROW(g.playAction("bribe"));

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
                CHECK_NOTHROW(g.playAction("arrest", g.getPlayers()[0]));

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
                CHECK_NOTHROW(g.playAction("arrest", g.getPlayers()[0]));

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
            CHECK_NOTHROW(g.playAction("arrest", g.getPlayers()[0]));

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
                CHECK_NOTHROW(g.playAction("sanction", g.getPlayers()[1]));

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
                CHECK_NOTHROW(g.playAction("sanction", g.getPlayers()[1]));

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
            CHECK_NOTHROW(g.playAction("sanction", g.getPlayers()[1]));

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
            CHECK_NOTHROW(g.playAction("coup", g.getPlayers()[1], false));
            
            // check that Bob is no longer in the game
            CHECK(g.playersCount() == 1);
            CHECK(g.getPlayerByIndex(0)->getName() == "Alice"); // Alice should still be in the game
            CHECK_THROWS(g.getPlayerByIndex(1)); // should throw an exception since Bob is no longer in the game
        }
    }

    SUBCASE("Test EndTurn Action"){

    }

    SUBCASE("Test Invest Action"){

    }

    SUBCASE("Test SpyOn Action"){

    }

    SUBCASE("Test Block Arrest Action"){

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

    /*SUBCASE("Test Combinations"){
        SUBCASE("Sanctioned Player Plays Tax"){
            TestGame g = TestGame({"Alice", "Bob"}, {"Governor", "Judge"});

            // check that the current player is Alice
            CHECK(g.turn() == "Alice");

            // give alice enough coins to perform a sanction on Bob
            g.getPlayers()[0]->addCoins(3); 

            // check that Alice has 0 coins
            CHECK(g.getPlayers()[0]->coins() == 0);

            // Alice performs the tax action
            CHECK_NOTHROW(g.playAction("tax"));

            // check that Alice has 3 coins after taxing
            CHECK(g.getPlayers()[0]->coins() == 3);

            // check that Alice has 0 actions left
            CHECK(g.getPlayers()[0]->getRemainingActions() == 0);

            // pass the turn to Bob
            g.nextTurn();


        }
    }*/
}

TEST_CASE("Test Game Undo Actions"){
    SUBCASE("Test Undo Tax Action"){
        
    }
    SUBCASE("Test Undo Bribe Action"){

    }
    SUBCASE("Test Undo Coup Action"){

    }
}