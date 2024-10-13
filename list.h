#ifndef Node_H
#define Node_H

struct Node{

    char item;  //k = key, b = bomb, 
    Node *left;
    Node *right;
    Node *up;
    Node *down;

    Node(){}
};

#endif