/* ksim.cpp
 * Name: Ethan Hinkle | hink9661@valdals.uidaho.edu
 * Date: 10/31/21
 * Class: CS 240-01 | Prof Wilder
 *
 * This is the function file of homework 3, a simulator of a 5 state kernel
 * This file contains the acutual functions used by the state class in the header
 * ---------------------------------------------------------------------
 */

#include "ksim.h" // allows for prototypes and struct to be read

using namespace std;

// class constructor
state::state(){
    head = NULL;
    tail = head;
}

// this function adds processes to the sim
void state::add(string p){
    NodePtr n = new node;
    n -> id = p; // sets name of the process
    n -> state = "New"; // sets current state, always new on creation
    n -> io = -1; // sets io device for process, -1 signifies no io device
    n -> tick = 0; // sets entry time for blocked queue, init to 0
    n -> next = NULL; // sets next data member to NULL
    n -> prev = NULL; // sets prev data member to NULL
    if(head == NULL){ // if the list is empty check
        head = n; // n is head
        tail = head; // head is tail
    }else{ // if the list is not empty
        tail -> next = n; // adds n to list
        n -> prev = tail; // sets n prev to current tail
        tail = tail -> next; // sets n to be the new tail
    }
}

// this function adds an already created processes to a queue, update state, io, and current time
int state::enqueue(NodePtr n, string s, int x, unsigned int y){
    if(n == NULL){ // NULL data received from fetch
        return 1;
    }
    cout << "Process " << n -> id << " moved from " << n -> state;
    if((n -> state) == "Blocked"){ // if blocked, display what io device
        cout << " (IO Device " << n -> io << ")";
    }
    cout << " to " << s << endl;
    if(head == NULL){ // if the list is empty check
        if((n -> state) == "Running"){ // only update tick if moving out of running state
            n->tick = y;
        }
        n -> state = s;
        n -> io = x;
        head = n;
        tail = head;
    }else{ // if the list is not empty
        if((n -> state) == "Running"){ // only update tick if moving out of running state
            n->tick = y;
        }
        n -> state = s;
        n -> io = x;
        tail -> next = n;
        n -> prev = tail; // sets n prev to current tail
        tail = tail -> next; // sets new addition to tail
    }
    return 0;
}

// this function deletes the front of a linked list
void state::dequeue(int i){
    NodePtr curr;
    if(head != NULL) { // only runs if list is occupied
        curr = head;
        if (i == 1) { // only triggers if display deletion is toggled
            cout << "Process " << curr->id << " is banished to the void" << endl; // deletion message
        }
        head = head->next;
        if (head != NULL){ // if new head null, ignore
            head -> prev = NULL; // sets new head prev pointer to NULL, instead of removed data
        }
        curr -> next = NULL;
        curr -> prev = NULL;
        delete curr; // deletes front of list
    }
}

// this function fetches the front of a linked list
state::NodePtr state::fetch(){
    NodePtr curr;
    if(head != NULL){ // only runs if list is occupied
        curr = head;
        head = head -> next;
        if(head != NULL) { // if the new head is null, no need to update prev pointer
            head -> prev = NULL;
        }
        curr -> next = NULL;
        curr -> prev = NULL;
        return curr; // returns front of list
    }
    return NULL; // if list is not occupied, NULL returned
}

// this function fetches an item with the inputted tick rate, priority queue
state::NodePtr state::fetchTick(unsigned int i){
    NodePtr curr;
    if(head != NULL){ // only runs if list is occupied
        curr = head;
        while(curr != NULL && curr->tick != i){ // loop until end of list or data found
            curr = curr -> next;
        }
        if(curr == NULL){ // no data found
            return NULL;
        }
        else{
            if(curr == head){
                head = head -> next;
            }
            if((curr -> next) != NULL){ // do as long as the next value is not NULL
                (curr->next) -> prev = curr -> prev; // sets the next nodes prev value to the current nodes prev value
            }
            if((curr -> prev) != NULL) { // do as long as the prev value is not NULL
                (curr->prev) -> next = curr -> next; // sets the prev nodes next value to the current nodes next value
            }
            curr -> next = NULL; // clears current pointers
            curr -> prev = NULL; // clears current pointers
        }
        return curr; // returns front of list
    }
    return NULL; // if list is not occupied, NULL returned
}

// finds if the process already exists
int state::search(string p){
    if(head == NULL){ // if the list is empty
        return 1; // process not found
    }
    else{ // list is not empty
        NodePtr curr = head; // creates value for current location equal to head
        while(curr != NULL && curr->id != p){ // loops until the node with the id is found or the list ends
            curr = curr->next; // walks curr to the next value in the list
        }
        if(curr == NULL){
            return 1; // process not found
        }
        else{ // runs if a value was found in the list
            return 0; // process found
        }
    }
}

// returns the smallest tick value in the list
unsigned int state::minTick(){
    unsigned int val;
    if(head == NULL){ // if the list is empty
        return 0; // will return a min value of 0, can be problem but the fetch function will catch error later and return NULL
    }
    else{
        NodePtr curr = head; // creates value for current location equal to head
        val = curr -> tick;
        while(curr != NULL){ // loops until the node with the id is found or the list ends
            if((curr -> tick) < val){ // been longer since last run
                val = curr -> tick;
            }
            curr = curr -> next; // walks curr to the next value in the list
        }
    }
    return val; // return the tick for when the process blocked
}

// finds and prints specific process data
int state::print(string p){
    if(head == NULL){ // if the list is empty
        return 1; // process not found
    }
    else{ // list is not empty
        NodePtr curr = head; // creates value for current location equal to head
        while(curr != NULL && curr->id != p){ // loops until the node with the id is found or the list ends
            curr = curr->next; // walks curr to the next value in the list
        }
        if(curr == NULL){
            return 1; // process not found
        }
        else{ // runs if a value was found in the list
            cout << "*********************************************" << endl;
            cout << "ID: " << curr -> id << endl;
            cout << "State: " << curr -> state << endl;
            if(curr -> state == "Blocked"){ // only show io device and tick if in blocked queue
                cout << "Waiting on io device " << curr -> io << " since tick " << curr -> tick << endl;
            }
            cout << "*********************************************" << endl;
        }
    }
    return 0; // process found and printed
}

// finds and prints all process data in a queue
void state::printAll(){
    if(head != NULL) { // the queue is not empty
        NodePtr curr = head; // creates value for current location equal to head
        while (curr != NULL) { // loops until the node with the id is found or the list ends
            cout << "*********************************************" << endl;
            cout << "ID: " << curr -> id << endl;
            cout << "State: " << curr -> state << endl;
            if(curr -> state == "Blocked"){ // only show io device and tick if in blocked queue
                cout << "Waiting on io device " << curr -> io << " since tick " << curr -> tick << endl;
            }
            cout << "*********************************************" << endl;
            curr = curr->next; // walks curr to the next value in the list
        }
    }
}

// clears queue
void state::removeAll(){
    while(head != NULL){ // while queue is not empty
        dequeue(0);
    }
    head = NULL;
    tail = head;
}
