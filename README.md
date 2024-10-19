Version 4 Semi-Final (hopefully)

Changes:
-fixed display

Note:
-Test the code thoroughly for final version

Running the game:
To run the games run the following commands on your linux terminal in the working directory:
g++ -o gmae game.cpp -lncurses
./game
you must have lncurses installed on your system

Playing the Game:
The game is not case sensitive i.e inputting w is the same as W

Controls:
To move up: w, up arrow key
To move down: w, up  key
To move left: w, up arrow key
To move right: w, up arrow key
to quit: esc or q
to undo move: u

rules:
 The player is blind
and can only sense their surroundings within the maze.The player must navigate through the maze, find
a hidden key, and unlock the exit door, both of which are randomly placed. However,
the player cannot see the key or the door; instead, they can sense whether they are
getting closer or further from the key using a sensing ability.  Movement will be restricted by an ”undo”
feature that allows limited backtracking. A player can gather coins, which are visible,
and the difficulty will scale across three levels.

implementation:
classes:
Grid: stores the state of the game in the grid
Game: controls the input of player and modifies the game state
ItemQueue: stores the items placed in the grid as well as the next items to spawn. for this two instances of this class are implemented in the game class. After every 30 seconds an item is removed from the grid and an item is added to the grid at a random coordinate from the nextItems queue and another item is enqueued to the nextItems
coordinateQueue: stores the coordinates and name of items picked in order which are displayed at the end
MoveStack: stores the last moves of the player. when u is pressed the player goes back to the last position if possible

features:
menu: displays necessary information about the game state
add menu.png

sensing ability:
the player cannot see the key or the gate and can only sense wheater they are getting closer to the gate or key

bombs:the player cannot see the bombs but can sense how many bombs are near them. the probability of a bomb being placed next is 0.25

coins: the coins are visible but still the player can sense how many coins are near them. the probability of a coins being placed next is 0.75

levels:the game has 3 levels
easy: 

insert easy.png

medium

insert medium.png

hard

insert hard.png

Game ending conditions:

When u step on a bomb or you run out of moves the game is over

gameOver.png

When u pass all the levels, you win the game

gameWon.png

