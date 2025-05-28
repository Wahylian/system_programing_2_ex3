# Assignment 3 System Programing 2 

#### By: Ido Ron
#### ID: 215312729 

---
## Project Features
1.  **[Main](gui.cpp)** that creates a GUI for the game and lets 2 to 6 players play Coup. <br/>
2. **[Test Folder](Tests)** which allows you to run tests on all classes and functions in the project. <br/>
3. **[Makefile](Makefile)** allowing to run the following commands: <br/>
    * **make Main**- Which creates the Main EXE file and allows to play the game through the GUI. <br/> Since the Main file uses a GUI there will be Memory Leaks which the Lecturer gave permission to have. <br/>
    * **make valgrind**- Which runs Main with valgrind to check for **Memory Leaks**. <be/>
    * **make test**- Which creates the test EXE file and allows for the testing of the game. <br/>
    * **make valgrind-test**- Which runs test with valgrind to check for **Memory Leaks**. <br/>
    * **make clean**- Which removes all object and exe files from the project. <br/>


#### **NOTE:** From personal testing, the GUI will not run on the terminal inside Visual Studio Code. Please run the GUI directly through the Terminal App on Linux
---
## Game Instructions 

#### Start by Running the Main Executable. 

#### Pick the number of Participants in the Game (2-6 players).

#### Pick the names of each of the Players.

#### On a given player's turn they will be able to see the following:
* The other players still in the game.
* Their **Name**.
* Their **Role**.
* The amount of **Coins** they have.
* The amount of **Actions** they have remaining in their turn.

#### On their turn, a Player can Chose one of the following Actions:
* **Gather**- Gives them one coin, costs an action.
* **Tax**- Gives them 2 coins, costs an action.
* **Bribe**- Gives them an extra Action, costs 4 coins.
* **Arrest**- Allows them to Arrest another player in the game and take a coin from them (The same player cannot be arrested twice in a row by the same player). Costs an Action.
* **Sanction**- Allows them to sanction another player and **Block** them from using the **Gather** and the **Tax**  actions for their next turn. Costs 3 coins and an Action
* **Coup**- Allows the player to Coup another player and remove them from the game. 
Costs 7 coins and an Actions and is a **Required Action** if they have at least 10 coins in their possession.


#### There are 6 Possible Roles a Player can Have, Each Granting the Player Access to Different Features:
* **Governor**-<br/> 
Tax Action will give 3 coins **instead** of 2. <br/>
Gains access to the **Undo Tax** Action- which lets them **Undo the Tax Action** of other players. Costs nothing and is done **in real time** after another player attempts to use the Tax Action
* **Spy**-<br/>
Gains Access to the **SpyOn** Action- which lets them spy on other players and see how many coins they have. Costs nothing. <br/>
Gains Access to the **Block Arrest** Action- which lets them **Block the Arrest Action** of the other players. Costs nothing.
* **Baron**-<br/>
Gains Access to the *Invest** Action- which lets them **invest 3 coins and gain 6 in return**. <br/>
If they are **Sanctioned**, they **gain 1 coin** as compensation.
* **General**-<br/>
Gains Access to the **Undo Coup** Action- which lets them **Undo a Coup from Occuring** both on themselves and on others. Costs 5 coins and is done **in real time** after another player attempts to throw a Coup. <br/>
If they are **Arrested**, they receive back the coin that was taken from them.
* **Judge**-<br/>
Gains Access to the **Undo Bribe** Action- which lets them **Undo the Bribe Action** of other players. Costs nothing and is done **in real time** after another player attempts to use the Bribe Action.<br/>
If they are **Sanctioned**, the player attempting to sanction them **must pay 4 coins instead of 3**.
* **Merchant**-<br/>
If they begin their turn with **3 Coins**, they **Gain an Additional Coin for Free**.<br/>
If they are **Arrested**, they **Must Pay 2 Coins to the Bank instead of Paying the Arresting Player**.


#### The Game Ends When All Players Except One Were Coup'd.
---
## Libraries Used 
1.  wxprec.h - for GUI <br/>
2.  wx/wx.h - for GUI <br/>
3.  vector - general use throughout the project <br/>
4.  string - general use throughout the project <br/>
5.  exception - error handling <br/>
5.  stdexcept - error handling and custom errors <br/>
6.  random - random role picking for the players <br/> <br/>

**NOTE:** To use wxWidgets install the following package on Linux:

```sh
sudo apt install libwxgtk3.2-dev
```
