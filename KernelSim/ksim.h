/* ksim.h
 * Name: Ethan Hinkle | hink9661@valdals.uidaho.edu
 * Date: 10/31/21
 * Class: CS 240-01 | Prof Wilder
 *
 * This is the header of homework 3, a simulator of a 5 state kernel
 * The header consists of the state class and all its functions, as well
 * as all the libraries necessary to execute
 * The class consists of a queue struct that can act as a fifo or a
 * priority queue
 * ---------------------------------------------------------------------
 */

#include <iostream>
#include <sstream>
#include <string>
#include <cstring> // for tokenizing the string
#include <cstdio> // for printf

#define STR_BUFF 64 // longest buffer for the c strings

using namespace std;

class state {
private:
    struct node {
        string id; // stores the column of the array position
        string state; // stores the row of the array position
        int io; // io device the process is waiting on
        unsigned int tick; // when process entered the run state, used for determining next process
        struct node *next; // pointer to the next value in a linked list
        struct node *prev; // pointer to the previous value in a linked list
    };
    typedef struct node *NodePtr; // creates a node pointer called NodePtr
    NodePtr head;
    NodePtr tail;
public:
    state();
    void add(string);
    int enqueue(NodePtr, string, int, unsigned int);
    void dequeue(int);
    NodePtr fetch();
    NodePtr fetchTick(unsigned int);
    int search(string);
    unsigned int minTick();
    int print(string);
    void printAll();
    void removeAll();
};