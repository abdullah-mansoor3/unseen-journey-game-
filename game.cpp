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
        playerJ =size/2;
        Node *playerNode = getNode(playerI, playerJ);
        player = playerNode;
        player->item = 'p';

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

                if(j==0){
                    currRowHead = newNode;
                    curr = newNode;
                }
                else{
                    curr->right = newNode;
                    newNode->left = curr;
                    curr = curr->right;
                }

                if(nodeAbove){
                    curr->up = nodeAbove;
                    nodeAbove->down = curr;
                    nodeAbove = nodeAbove->right;
                }
            }
            nodeAbove = currRowHead;

        }

        placeItems(size);
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
        resize(size);
    }

    Node *getNode(int i, int j){

        Node *currRowHead =  head;

        for(int I = 0; I<=i ; I++){ 

            if(!currRowHead){return nullptr;}  

            if(I==i){

                Node *curr = currRowHead;

                for(int J=0 ; J<j; J++){
                    if(curr){
                        curr=curr->right;
                    }
                    else{
                        return nullptr;
                    }
                }

                return curr;
            }

            currRowHead = currRowHead->down;
        }

        return nullptr;
    }

    int getDistance(){
        
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
    char mode; //e = easy, m = medium, h =hard

    public:
    Game(): mode('e'), grid(10), size(10), keyFound(false){
        
    }

    void printGrid(){
        printw("Mode : Easy\n");
        printw("Remaining Moves: 20       \t     Remaining Undos: 6\n");
        printw("Score : 0\t\tKey Status: False \n");
        printw("Hint : Closer\n");
        printw("k = key\tb = bomb\tg = gate\tc = coin\n");
        printw("Hint : Closer\n");


        for(int i = 0; i <= size*2 + 1; i++)
            mvprintw(LINES-size-2, i,"%c", '#');

        Node *row = grid.head;

        for(int i = 0 ; i<size;i++){
            mvprintw(LINES-size+i -1, 0, "#");
            
            Node *curr =row;

            for(int j=0; j<size;j++){
                if(curr){
                    char ch = curr->item;
                    ch = ch=='k' ? '.' : ch;  //if the item is the gate or key then hide it
                    mvprintw(LINES - size + i-1, (j+1)*2, "%c", ch);
                    curr = curr->right;
                }
            }
            mvprintw(LINES-size+i-1, size*2+1,"%c", '#');
            if(row)
                row = row->down;
        }

        for(int i = 0; i <= size*2 + 1; i++)
            mvprintw(LINES-1, i,"%c", '#');

        refresh();
        
    }

    void movePlayer(){

    }

};

int main(){
    char ch;
    initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */

    Game g;
    g.printGrid();
	refresh();
    getch();
	endwin();
    
    return 0;
}