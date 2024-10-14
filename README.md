Version 2

Changes:
-added comments 
-moved the player in the grid-added conditions for when the player steps on a bomb, key, gate etc
-changes difficulty level when player enters the gate
-added winning condition
-changed the menu to update when the variables update
-bug fix infinite recursion in Grid::resize

Future changes:
- Write Grid::getTotalDistance Grid::getKeyDistance
-implement undoing moves
-implement random coin and bomb drops
-reveal the coordinates and order of coins, keys, bombs when player exits the door or loses
-display the initial state of the game when lose
