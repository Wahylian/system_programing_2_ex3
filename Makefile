# rido1607@gmail.com
CXX=clang++
CXXFLAGS=-Wall -Werror -std=c++2a 
WX_CXXFLAGS= $(shell wx-config --cxxflags)
WX_LIBS= $(shell wx-config --libs)

# object files for the player classes
PLAYER_OBJ = PlayerFolder/Player.o PlayerFolder/Baron.o PlayerFolder/General.o \
			PlayerFolder/Governor.o PlayerFolder/Judge.o PlayerFolder/Merchant.o PlayerFolder/Spy.o

# object files for the custom exceptions
CUST_EXCEPT_OBJ = CustomExceptions/CustomPlayerExceptions.o CustomExceptions/CustomGameExceptions.o

# object files used for the game to run
GAME_OBJ = Game.o $(CUST_EXCEPT_OBJ) $(PLAYER_OBJ)

# object files for the test files
TEST_OBJ = Tests/testFile.o Tests/testPlayer.o Tests/testBaron.o Tests/testGeneral.o \
			Tests/testGovernor.o Tests/testJudge.o Tests/testMerchant.o Tests/testSpy.o \
			Tests/testGame.o


# object files for the GUI
GUI_OBJ = gui.o $(GAME_OBJ)

# valgrind flags, taken from course site, folder 02-classes-constructors-destructors: the makefile in the valgrind folder
VALGRIND_FLAGS=-v --leak-check=full --show-leak-kinds=all --error-exitcode=99 

# compile the gui file with the wxWidgets flags
gui.o: gui.cpp
	$(CXX) $(CXXFLAGS) $(WX_CXXFLAGS) -c gui.cpp -o gui.o

# turns the specified cpp file into an object file with the same name
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# creates the main executable (gui)
Main: $(GUI_OBJ)
	$(CXX) $(CXXFLAGS) $(WX_CXXFLAGS) -o Main $(GUI_OBJ) $(WX_LIBS)

valgrind: Main
	valgrind $(VALGRIND_FLAGS) ./Main

# creates a test executable for the test files
test: $(TEST_OBJ) $(GAME_OBJ)
	$(CXX) $(CXXFLAGS) -o test $(TEST_OBJ) $(GAME_OBJ) 

# checks for memory leaks during the tests
valgrind-test: test
	valgrind $(VALGRIND_FLAGS) ./test

clean:
	rm -f *.o PlayerFolder/*.o CustomExceptions/*.o Tests/*.o Main test 

.PHONY: clean 