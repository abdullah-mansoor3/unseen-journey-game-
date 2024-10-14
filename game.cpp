#include<iostream>
#include<ncurses.h>

using namespace std;

int random(int min, int max) {
    return rand()%(max-min+1) + min;
}

struct Node{

    char item;  //p for player, b=bomb, k=key, c=coin, g=gate, .=nothing

    Node *up;
    Node *down;
    Node *left;
    Node *right;

    Node(){
        item = '.';
        up = nullptr;
        down = nullptr;
        left = nullptr;
        right = nullptr;
    }
};

struct Grid{
    Node *head;
    Node *player;
    Node *key;

    void placeItems(int size){
        int playerI, playerJ,keyI, keyJ,gateI, gateJ,bomb1I, bomb1J, bomb2I, bomb2J;
        
        playerI =size - 1;
        playerJ =size/2;  //place the player in the middle of bottom row
        Node *playerNode = getNode(playerI, playerJ);
        player = playerNode;
        player->item = 'p';


        //keep getting random coordinates until we get a coordinate which does not have an item
        do{
            keyI = random(0, size-1);
            keyJ = random(0, size-1);
        }while(keyI==playerI && keyJ==playerJ);

        Node *keyNode = getNode(keyI, keyJ);
        keyNode->item = 'k';
        key = keyNode;

        do{
            gateI = random(0, size-1);
            gateJ = random(0, size-1);
        }while((gateI==playerI && gateJ==playerJ) ||(gateI==keyI && gateJ==keyJ));

        Node *gateNode  =getNode(gateI, gateJ);
        gateNode->item = 'g';


        do{
            bomb1I = random(0, size-1);
            bomb1J = random(0, size-1);
        }while((bomb1I==playerI && bomb1J==playerJ) ||(bomb1I==keyI && bomb1J==keyJ) || (bomb1I==gateI && bomb1J==gateJ));

        Node *bomb1Node  =getNode(bomb1I, bomb1J);
        bomb1Node->item = 'b';

        do{
            bomb2I = random(0, size-1);
            bomb2J = random(0, size-1);
        }while((bomb2I==playerI && bomb2J==playerJ) ||(bomb2I==keyI && bomb2J==keyJ) || (bomb2I==gateI && bomb2J==gateJ) || (bomb2I==bomb1I && bomb2J==bomb1J));

        Node *bomb2Node  =getNode(bomb2I, bomb2J);
        bomb2Node->item = 'b';




    }

    void init(int size){

        Node *currRowHead = nullptr;
        Node *curr = nullptr;
        Node *nodeAbove = nullptr;

        for(int i =  0; i <size; i++){

            for(int j = 0; j<size; j++){
                Node *newNode = new Node();
                if(!curr){ //for the very first node
                    head = newNode; 
                    currRowHead = head;
                    curr = head;
                    continue;
                }

                if(j==0){ //for first columns i.e. head of each row
                    //we dont link the first node from the left
                    //its up, down and left will be set later
                    currRowHead = newNode; 
                    curr = newNode;
                }
                else{  //for other nodes
                    curr->right = newNode; //set right of previous node to the new node
                    newNode->left = curr; 
                    curr = curr->right; //move the curr to new node
                }

                if(nodeAbove){  //only set the up node if the node above isnt nullptr
                    curr->up = nodeAbove; //set the up
                    nodeAbove->down = curr; //set the down of the node above
                    nodeAbove = nodeAbove->right; //move the node above to right
                }
            }
            nodeAbove = currRowHead; //

        }

        placeItems(size); //place key, gate, player etc
    }

    void deallocate(){
        Node *row = head;

        while(row){
            Node *curr  = row;
            row = row->down;
            while(curr){
                Node *deleted = curr;
                curr = curr->right;
                delete deleted;
            }
        }
    }

    Grid(int size){
        head = nullptr;
        player = nullptr;

        init(size);
    }

    ~Grid(){
        deallocate();
    }

    void resize(int size){
        deallocate();  
        init(size); //initialize the grid with the new size
    }

    Node *getNode(int i, int j){

        Node *currRowHead =  head;

        for(int I = 0; I<=i ; I++){ //loop through each row

            if(!currRowHead){return nullptr;} //node not found 

            if(I==i){ //only iterate the row if its the row of the required node

                Node *curr = currRowHead;

                for(int J=0 ; J<j; J++){
                    if(curr){
                        curr=curr->right;
                    }
                    else{
                        return nullptr;  //not found
                    }
                }

                return curr;
            }

            currRowHead = currRowHead->down;
        }

        return nullptr;
    }

    char moveLeft(){
        if(player->left){
            player->item = '.';//set the currrent item to .

            player = player->left;
            char item = player->item; //for returning
            player->item = 'p'; 
            return item;
        }

        return '.';


    }

    char moveRight(){
        if(player->right){
            player->item = '.';//set the currrent item to .

            player = player->right;
            char item = player->item; //for returning
            player->item = 'p'; 

            return item;
        }

        return '.' ;
    }

    char moveUp(){
        if(player->up){
            player->item = '.';//set the currrent item to .

            player = player->up;
            char item = player->item; //for returning
            player->item = 'p'; 

            return item;

        }

        return '.';
    }

    char moveDown(){
        if(player->down){
            player->item = '.';//set the currrent item to .

            player = player->down;
            char item = player->item; //for returning
            player->item = 'p'; 

            return item;
        }

        return '.';
    }

    int getKeyDistance(){ //distnace from player to key
        int distance = 0;

        return distance;
    }

    int getTotalDistance(){ //distance from player to key to gate
        int distance = getKeyDistance();

        return distance;
    }



};

class Game{
    Grid grid;
    int size;
    int playerX, playerY;
    int keyX, keyY;
    int doorX, doorY;

    bool keyFound;
    int movesLeft;
    int distanceToKey;
    int previousDistance;
    int score;
    int undosLeft;
    string mode; 
    string hint;

    void revealOrderOfCollection(){


        getch(); //wait for user input to display the next screen
    }

    void displayYouWin(){
        score += undosLeft;
        clear();
        printw("You Won yayyyyyy :-) ");
        printw("Your total Score: ");
        printw((to_string(score).c_str()));
        revealOrderOfCollection();
    }

    void displayYouLose(){
        score += undosLeft;
        clear();
        if(movesLeft<=0)
            printw("You Lose. You used up all of your moves :-(");
        else
            printw("You Lose. You stepped on a bomb :-( ");
        printw((to_string(score).c_str()));
        revealOrderOfCollection();
    }

    void changeMode(){
        score += undosLeft;
        switch(mode[0]){ //swithc cannot work with strings so we use the first character only
            case 'E': //easy
                size = 10;
                movesLeft = 6; //6 extra moves
                undosLeft = 6;
                break;
            case 'M': //medium
                size = 15;
                movesLeft = 2; //2 extra moves
                undosLeft = 4;
                break;
            case 'H': //hard
                size = 20;
                movesLeft = 0; //no extra moves
                undosLeft = 1;
                break;
            default://invalid input
                mode = "Easy"; 
                changeMode(); //call the function again

        };

        grid.resize(size); //resize the grid

        previousDistance = 0;
        distanceToKey = grid.getKeyDistance();
        hint = "Closer";
        keyFound = false;

        movesLeft += grid.getTotalDistance(); //add the distance from the player to the key to the gate
    }

    void nextLevel(){ //for moving to the next level
        if(keyFound){
            switch(mode[0]){ //switch cannot work with strings so we use first character only
                case 'E': //easy
                    mode = "Medium";
                    break;
                case 'M'://medium
                    mode = "Hard";
                    break; 
                case 'H':  //hard
                    displayYouWin();
                    break;
            }

            changeMode();

            revealOrderOfCollection();
        }

    }

    bool movePlayer(int input){ //returns true if game ends

        if(movesLeft <= 0){
            displayYouLose();
            return true; //end the game
        }
        
        char itemAtNewPosition = '.';
        
        switch(input){

            case 'w':
            case 'W':
            case KEY_UP:
                itemAtNewPosition = grid.moveUp();
                break;

            case 's':
            case 'S':
            case KEY_DOWN:
                itemAtNewPosition = grid.moveDown();
                break;

            case 'a':
            case 'A':
            case KEY_LEFT:
                itemAtNewPosition = grid.moveLeft();
                break;

            case 'd':
            case 'D':
            case KEY_RIGHT:
                itemAtNewPosition = grid.moveRight();
                break;
        }

        switch(itemAtNewPosition){
            //check what item was on the new position
            case 'k'://key
                keyFound = true;
                break;
            case 'c'://coin
                score += 2;
                undosLeft += 1;
                break;
            case 'b'://bomb
                displayYouLose();
                return true; //end the game
            case 'g'://gate
                nextLevel();
                break;
        };

        return false;

    }

    public:
    Game(): mode("Easy"), grid(10), size(10), keyFound(false){
        score = 0;
        undosLeft = 0;
        previousDistance = 0;
        distanceToKey = 0;
        movesLeft = 0;
        hint = "Closer";

        changeMode();
        
    }

    bool getInput(){ //returns true if game ends
        int input = getch();

        switch(input){

            case 'q':
            case 'Q':
            case 27: //esc key
                return true; //quit the game
                break;
            default:
                return movePlayer(input);
        }

        return false;


    }

    void printGrid(){


        //menu on the top
        mvprintw(0, 0, ("Mode :  "  + mode + "\n").c_str());  //convert to c style string bcz mvprintw works like that
        mvprintw(1, 0, ("Remaining Moves: " + to_string(movesLeft) + "       \t     Remaining Undos: " + to_string(undosLeft) + "\n").c_str());
        mvprintw(2, 0, ("Score : " + to_string(score) + "\t\tKey Status: " + to_string(keyFound) + " \n").c_str());
        mvprintw(3, 0, ("Hint : " + hint + "\n").c_str());
        mvprintw(4, 0, "k = key\tb = bomb\tg = gate\tc = coin\n");


        //grid
        for(int i = 0; i <= size*2 + 1; i++) //top wall
            mvprintw(LINES-size-2, i,"%c", '#');

        Node *row = grid.head;

        for(int i = 0 ; i<size;i++){ //grid
            mvprintw(LINES-size+i -1, 0, "#"); //left wall
            
            Node *curr =row;

            for(int j=0; j<size;j++){ //the center grid area
                if(curr){
                    char ch = curr->item;
                    ch = ch=='k' || ch=='c' || ch=='b'? '.' : ch;  //if the item is the key, coin or bomb then hide it
                    mvprintw(LINES - size + i-1, (j+1)*2, "%c", ch);
                    curr = curr->right;
                }
            }
            mvprintw(LINES-size+i-1, size*2+1,"%c", '#'); //right wall
            if(row)
                row = row->down;
        }

        for(int i = 0; i <= size*2 + 1; i++) //bottom wall
            mvprintw(LINES-1, i,"%c", '#');

        refresh();
        
    }    

};

int main(){
    char ch;
    initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */
    curs_set(0);  //hide the cursor
    set_escdelay(0);         // Disable ESC key delay (optional for ESC)

    Game g;
    bool endGame = false;

    while(!endGame){
        endGame = g.getInput();
        g.printGrid();
        refresh();
    }

    getch(); //wait for user input before exiting
	endwin();
    
    return 0;
}