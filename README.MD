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
    * **make clean**- Which removes all object and exe files from the project.
---
## Game Instructions 

---
## Libraries Used 
1.  wxprec.h - for GUI <br/>
2.  wx/wx.h - for GUI <br/>
3.  vector - general use throughout the project <br/>
4.  string - general use throughout the project <br/>
5.  exception - error handling <br/>
5.  stdexcept - error handling and custom errors <br/>
6.  random - random role picking for the players <br/> <br/>

**NOTE: The libraries that use wxWidgets require the download of the following apt through linux:**

```sh
sudo apt install libwxgtk3.2-dev
```
