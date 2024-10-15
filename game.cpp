#include<iostream>
#include<ncurses.h>

using namespace std;

int random(int min, int max) {
    return rand()%(max-min+1) + min;
}

int abs(int num){
    return num<0? num*-1 : num;
}

int cityBlockDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

struct GridNode{

    char item;  //p for player, b=bomb, k=key, c=coin, g=gate, .=nothing
    int row, col;
    GridNode *up;
    GridNode *down;
    GridNode *left;
    GridNode *right;

    GridNode(int Row, int Col){
        item = '.';
        up = nullptr;
        down = nullptr;
        left = nullptr;
        right = nullptr;
        row = Row;
        col = Col;
    }
};

struct Grid{
    GridNode *head;
    GridNode *player;
    GridNode *key;
    GridNode *gate;
    int size;

    void placeItems(){
        int playerI, playerJ,keyI, keyJ,gateI, gateJ,bomb1I, bomb1J, bomb2I, bomb2J;
        
        playerI =size - 1;
        playerJ =size/2;  //place the player in the middle of bottom row
        GridNode *playerNode = getNode(playerI, playerJ);
        player = playerNode;
        player->item = 'p';


        //keep getting random coordinates until we get a coordinate which does not have an item
        do{
            keyI = random(0, size-1);
            keyJ = random(0, size-1);
        }while(keyI==playerI && keyJ==playerJ);

        GridNode *keyNode = getNode(keyI, keyJ);
        keyNode->item = 'k';
        key = keyNode;

        do{
            gateI = random(0, size-1);
            gateJ = random(0, size-1);
        }while((gateI==playerI && gateJ==playerJ) ||(gateI==keyI && gateJ==keyJ));

        GridNode *gateNode  =getNode(gateI, gateJ);
        gateNode->item = 'g';
        gate = gateNode;


        do{
            bomb1I = random(0, size-1);
            bomb1J = random(0, size-1);
        }while((bomb1I==playerI && bomb1J==playerJ) ||(bomb1I==keyI && bomb1J==keyJ) || (bomb1I==gateI && bomb1J==gateJ));

        GridNode *bomb1Node  =getNode(bomb1I, bomb1J);
        bomb1Node->item = 'b';

        do{
            bomb2I = random(0, size-1);
            bomb2J = random(0, size-1);
        }while((bomb2I==playerI && bomb2J==playerJ) ||(bomb2I==keyI && bomb2J==keyJ) || (bomb2I==gateI && bomb2J==gateJ) || (bomb2I==bomb1I && bomb2J==bomb1J));

        GridNode *bomb2Node  =getNode(bomb2I, bomb2J);
        bomb2Node->item = 'b';




    }

    void init(){

        GridNode *currRowHead = nullptr;
        GridNode *curr = nullptr;
        GridNode *nodeAbove = nullptr;

        for(int i =  0; i <size; i++){

            for(int j = 0; j<size; j++){
                GridNode *newNode = new GridNode(i, j);
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

        placeItems(); //place key, gate, player etc
    }

    void deallocate(){
        GridNode *row = head;

        while(row){
            GridNode *curr  = row;
            row = row->down;
            while(curr){
                GridNode *deleted = curr;
                curr = curr->right;
                delete deleted;
            }
        }
    }

    Grid(int size){
        head = nullptr;
        player = nullptr;
        this->size = size;
        init();
    }

    Grid(){
        size = 0;
        head = nullptr;
        player = nullptr;
        key = nullptr;
        gate = nullptr;
    }

    ~Grid(){
        deallocate();
    }

    void resize(int size){
        deallocate();  
        this->size = size;
        init(); //initialize the grid with the new size
    }

    GridNode *getNode(int i, int j){

        GridNode *currRowHead =  head;

        for(int I = 0; I<=i ; I++){ //loop through each row

            if(!currRowHead){return nullptr;} //node not found 

            if(I==i){ //only iterate the row if its the row of the required node

                GridNode *curr = currRowHead;

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
        return cityBlockDistance(player->row, player->col, key->row, key->col);
    }

    int getGateDistance(){
        return cityBlockDistance(player->row, player->col, gate->row, gate->col);
    }

    int getTotalDistance(){ //distance from player to key to gate
        return getKeyDistance() + cityBlockDistance(key->row, key->col, gate->row, gate->col);
    }

    void operator = (Grid g){
        deallocate();
        head = g.head;
    }



};

struct QueueNode{
    int x,y;
    char item;

    QueueNode *previous;
    QueueNode *next;

    QueueNode(){
        
        x = -1;
        y = -1;

        if(random(0,3) == 0){
            item = 'b';
        }
        else{
            item = 'c';
        }

        previous = nullptr;
        next = nullptr;

    }
};

class ItemQueue{
    QueueNode *head, *tail;

    int playerX, playerY, keyX, keyY, doorX, doorY; //these are the values to avoid when setting the coordinates
    int gridSize;

    bool coordinatesOccupied(int row, int col) {
        if ((row == playerX && col == playerY) ||(row == keyX && col == keyY) || (row == doorX && col == doorY)) {
            return true;
        }

        // Check if it overlaps with any item in the grid queue
        QueueNode *current = head;
        while (current) {
            if (current->x == row && current->y == col) {
                return true;
            }
            current = current->next;
        }

        return false;
    }

    public:
    

    ItemQueue(){
        init();
    }

    ~ItemQueue(){
        deallocate();
    }

    void init(int PlayerX = -1,int PlayerY = -1,int DoorX = -1,int DoorY = -1,int KeyX = -1,int KeyY = -1, int GridSize = 0){
        head = nullptr;
        tail = nullptr;
        playerX = PlayerX;
        playerY = PlayerY;
        keyX = KeyX;
        keyY = KeyY;
        doorX = DoorX;
        doorY = DoorY;
        gridSize = GridSize;
    }

    void deallocate(){
        while(head){
            QueueNode *dequeued = dequeue();
            if(dequeued)
                delete dequeued;
            else
                return;
        }
    }

    //this enqueue is called for the nextItemsQueue
    void enqueue(){
        QueueNode *newNode = new QueueNode;

        if(tail){
            tail->next = newNode;
            newNode->previous = tail;
            tail = newNode;
            return;
        }

        head = newNode;
        tail = head;
    }

    //this is called for the gridItemsQueue and the dequeued node from the nextItemsQueue is passed as an argument
    void enqueue(QueueNode *Node){
        if(!Node){
            return;
        }
        //when head is not nullptr

        int row = 0;
        int col  = 0;

        //keep generating random coordinates until we get a coordinate where the index is not occupied
        do{
            row = random(0, gridSize);
            col = random(0, gridSize);
        }while( coordinatesOccupied(row, col) );

        Node->x = row;
        Node->y = col;

        if(!head){
            head = Node;
            tail = Node;
            return;
        }

        tail->next = Node;
        Node->previous = nullptr;
        tail = Node;
    }

    QueueNode *dequeue(){ //dont forget to deallocate the returned node
        if(!head){
            return nullptr;
        }

        QueueNode *dequeued = head;
        head = head -> next;
        if(head){
            head->previous = nullptr;
        }
        else{
            tail = nullptr;
        }
        if (dequeued){
            dequeued->next = nullptr; 
            dequeued->previous = nullptr; 
        }
        return dequeued;
    }

    string getItemsInQueue(){
        string s = "[ ";
        QueueNode *curr = head;
        while(curr){
            s = s + curr->item + " ,";
            curr = curr->next;
        }
        s += " ]";
        return s;
    }

};

class Game{

    Grid grid;
    int size;

    bool keyFound;

    int movesLeft;
    int undosLeft;

    int distanceToKey;
    int distanceToGate;
    int previousDistance;

    int score;

    int bombsNearby;
    int coinsNearby;

    string mode; 
    string hint;

    time_t startTime; //for measuring the time to drop bombs and coins
    time_t currentTime; 

    ItemQueue itemsInGrid;
    ItemQueue nextItems;

    void placeItemInGrid(int col, int row, char item){
        GridNode *Node = grid.getNode(row, col);

        if(Node && Node->item == '.'){ //if the node is not a nullptr and the index is not occupied
            Node->item = item;
        }
    }

    void initializeItemQueues(){
        GridNode *player = grid.player;
        GridNode *gate = grid.gate;
        GridNode *key = grid.key;

        itemsInGrid.deallocate();
        itemsInGrid.init(player->row, player->col, gate->row, gate->col, key->row, key->col, size);

        nextItems.deallocate();
        nextItems.init();

        //first enter 5 items in the next items queue
        //they get initialized with random items
        nextItems.enqueue();
        nextItems.enqueue();
        nextItems.enqueue();
        nextItems.enqueue();
        nextItems.enqueue();

        //then dequeue one node at a time and enqueue it in the grid queue
        //this way they get placed in the grid
        QueueNode *nodeToBeAddedToGrid = nextItems.dequeue();
        itemsInGrid.enqueue(nodeToBeAddedToGrid);
        nodeToBeAddedToGrid = nextItems.dequeue();
        itemsInGrid.enqueue(nodeToBeAddedToGrid);
        nodeToBeAddedToGrid = nextItems.dequeue();
        itemsInGrid.enqueue(nodeToBeAddedToGrid);
        nodeToBeAddedToGrid = nextItems.dequeue();
        itemsInGrid.enqueue(nodeToBeAddedToGrid);
        nodeToBeAddedToGrid = nextItems.dequeue();
        itemsInGrid.enqueue(nodeToBeAddedToGrid);

        //i added node first in the next items queue bcz thats when the nodes are created
        //when enqueuing to the grid queue, we take the dequeued node from the next items queue and enqueue it. no new node is created

        //then add 5 items again to fill up the next items queue
        nextItems.enqueue();
        nextItems.enqueue();
        nextItems.enqueue();
        nextItems.enqueue();
        nextItems.enqueue();

    }

    void updateItems(){

        //remove the items whose time is up i.e the item added first
        QueueNode *dequeued = itemsInGrid.dequeue();
        if(dequeued)
            delete dequeued;

        //dequeue an item from the next items queue and add it to the grid
        itemsInGrid.enqueue(nextItems.dequeue());

        //add an item to the next items queue to fill up the space
        nextItems.enqueue();
        
    }

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

    void calculateNearbyItems(){
        bombsNearby = 0;
        coinsNearby = 0;

        //calculate the number of bombs and coins nearby
        GridNode *player = grid.player;

        //calculate bombs

        //up
        if(player->up){
            if( player->up->item == 'b' ){
                bombsNearby += 1;
            }
            else if(player->up->item == 'c'){
                coinsNearby += 1;
            }
        }

        //down
        if(player->down){
            if( player->down->item == 'b' ){
                bombsNearby += 1;
            }
            else if(player->down->item == 'c'){
                coinsNearby += 1;
            }
        }

        //left
        if(player->left){
            if( player->left->item == 'b' ){
                bombsNearby += 1;
            }
            else if(player->left->item == 'c'){
                coinsNearby += 1;
            }
        }

        //right
        if(player->right){
            if( player->right->item == 'b' ){
                bombsNearby += 1;
            }
            else if(player->right->item == 'c'){
                coinsNearby += 1;
            }
        }

        //top left
        if(player->up){
            if(player->up->left){
                if( player->up->left->item == 'b' ){
                    bombsNearby += 1;
                }
                else if(player->up->left->item == 'c'){
                    coinsNearby += 1;
                }
            }
        }

        //top right
        if(player->up){
            if(player->up->left){
                if( player->up->right->item == 'b' ){
                    bombsNearby += 1;
                }
                else if(player->up->right->item == 'c'){
                    coinsNearby += 1;
                }
            }
        }

        //bottom left
        
        if(player->up){
            if(player->up->left){
                if( player->down->left->item == 'b' ){
                    bombsNearby += 1;
                }
                else if(player->down->left->item == 'c'){
                    coinsNearby += 1;
                }
            }
        }

        //bottom right
        
        if(player->up){
            if(player->up->left){
                if( player->down->right->item == 'b' ){
                    bombsNearby += 1;
                }
                else if(player->down->right->item == 'c'){
                    coinsNearby += 1;
                }
            }
        }
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
        distanceToGate = 0;
        bombsNearby = 0;
        coinsNearby = 0;
        hint = "Getting closer ";
        keyFound = false;

        movesLeft += grid.getTotalDistance(); //add the distance from the player to the key to the gate

        startTime = time(nullptr); //get the starting time 

        initializeItemQueues();
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

    void updateHint(){
        if(keyFound){
            previousDistance = distanceToGate;
            distanceToGate = grid.getGateDistance();
            if(distanceToGate<previousDistance){
                hint = "Getting closer ";
            }
            else{
                hint = "Getting further";
            }
        }
        else{
            previousDistance = distanceToKey;
            distanceToKey = grid.getKeyDistance();
            if(distanceToKey<previousDistance){
                hint = "Getting closer ";
            }
            else{
                hint = "Getting further";
            }
        }


    }

    bool movePlayer(int input){ //returns true if game ends

        if(movesLeft <= 0){
            displayYouLose();
            return true; //end the game
        }

        updateHint();
        
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
                updateHint();
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

        calculateNearbyItems();

        return false;

    }


    public:
    Game(): mode("Easy"), grid(10), size(10), keyFound(false){
        score = 0;
        undosLeft = 0;
        previousDistance = 0;
        distanceToKey = 0;
        movesLeft = 0;
        hint = "Getting closer ";
        bombsNearby = 0;
        coinsNearby = 0;

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

        currentTime = time(nullptr);
        if(difftime(startTime, currentTime)>=30.0){
            updateItems();
            startTime = time(nullptr);
        }

        return false;


    }

    void printGrid(){

        string itemsInLine = nextItems.getItemsInQueue();
        //menu on the top
        mvprintw(0, 0, ("Mode :  "  + mode + "\tRemaining Moves: " + to_string(movesLeft) + "\tRemaining Undos: " + to_string(undosLeft)).c_str());  //convert to c style string bcz mvprintw works like that
        mvprintw(1, 0, ("Score : " + to_string(score) + "\tKey Status: " + to_string(keyFound) + "\tHint : " + hint).c_str());
        mvprintw(2, 0, "k = key\tb = bomb\tg = gate\tc = coin");
        mvprintw(3, 0, ("Bombs nearby: " + to_string(bombsNearby) + "\tCoins nearby: " + to_string(coinsNearby)).c_str());
        mvprintw(4, 0, ("Space for messages like you found key, you picked a coin, you cant enter gate without key, you cant move back wihtout undoing, max undos reached"));
        mvprintw(5, 0, ("Next drop: " + itemsInLine).c_str());
        mvprintw(5, 0, "Arrow keys or wasd to move.\tq or ESC to quit\tu to undo");


        //grid
        for(int i = 0; i <= size*2 + 1; i++) //top wall
            mvprintw(LINES-size-2, i,"%c", '#');

        GridNode *row = grid.head;

        for(int i = 0 ; i<size;i++){ //grid
            mvprintw(LINES-size+i -1, 0, "#"); //left wall
            
            GridNode *curr =row;

            for(int j=0; j<size;j++){ //the center grid area
                if(curr){
                    char ch = curr->item;
                    ch = ch=='g' || ch=='k' || ch=='b'? '.' : ch;  //if the item is the key, gate or bomb then hide it
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