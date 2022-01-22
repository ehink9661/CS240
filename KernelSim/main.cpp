/* ksimMain.cpp
 * Name: Ethan Hinkle | hink9661@valdals.uidaho.edu
 * Date: 10/31/21
 * Class: CS 240-01 | Prof Wilder
 *
 * This is the main of homework 3, a simulator of a 5 state kernel
 * The main contains all the necessary variables and logic to execute
 * the function
 * The input is turned into a c string for tokenizing, otherwise it is
 * treated as a c++ string
 * Note: The global storage of ticks is an unsigned int, meaning when it
 * reaches 2^32, it will loop to 0. There is no logic check for this
 * so will cause errors when reached. Code is made with the assumption
 * that the sim will be reset before this point.
 * ---------------------------------------------------------------------
 */

#include "ksim.h" // allows for prototypes and struct to be read

using namespace std;

int main() {
    int tick, tickDis = 0, exit = 0, error, num, op, io;
    unsigned int tickTot = 0; // unsigned to never go negative/max value 4 bil instead of 2 bil, need total ticks to keep track of last run
    string cmd;
    char temp[STR_BUFF], *token; // used to tokenize, temp must have set size to work
    string cmds[2]; // should only ever have 2 commands max
    string ops[7] = {"exit", "add", "io-event", "query", "release", "step", "wait"}; // used for switch, no string allowed
    string ios[4] = {"0", "1", "2", "3"}; // used for switch, no string allowed
    state start, ready, running, end; // all states - blocked of the kernel sim
    state blk0, blk1, blk2, blk3; // all io devices have separate blocked states to simulate to allow for easy searching
    while(exit == 0){ // loops forever until exit condition
        cmds[1] = "all"; // as query is allowed one or two commands, cmds[1] is defaulted to all to print all items, instead of keeping last value
        error = 0;
        tick = 0;
        printf("ksim-");
        printf("%09d", tickDis);
        printf(">");
        getline(cin,cmd); // fetch input as a string
        strcpy(temp, cmd.c_str()); // turns input into c string
        num = 0;
        token = strtok(temp, " "); // get first token, up to space
        while ((token != NULL) && (num < 3)){ // check to see if there are any tokens left, or too many commands
            cmds[num] = token; // converts back into string
            num++; // increments cmd to store next command token
            token = strtok(NULL, " "); // get next token
        }
        for(int i = 0; i < 8; i++){ // finds the op to allow for switch statements
            if(ops[i] == cmds[0]){
                op = i;
                break;
            }
            op = -1; // if loop never breaks, op does not exist so -1
        }
        switch(op){
            case 0: // exit case
                if(num != 1){ // checks for correct number of arguments
                    cout << "Invalid number of operations" << endl;
                    break;
                }
                exit = 1;
                break;
            case 1: // add case
                if(num != 2){ // checks for correct number of arguments
                    cout << "Invalid number of operations" << endl;
                    break;
                }
                error += start.search(cmds[1]); // adds 1 if process exists, 0 if otherwise
                error += ready.search(cmds[1]);
                error += running.search(cmds[1]);
                error += blk0.search(cmds[1]);
                error += blk1.search(cmds[1]);
                error += blk2.search(cmds[1]);
                error += blk3.search(cmds[1]);
                error += end.search(cmds[1]);
                if(cmds[1] == "all"){
                    cout << "Process name all can not be used" <<  endl;
                }
                else if(error < 8){ // process was found somewhere
                    cout << "A process named " << cmds[1] << " already exits" << endl;
                }
                else{
                    start.add(cmds[1]);
                    tick = 32;
                }
                break;
            case 2: // io-event case
                if(num != 2){ // checks for correct number of arguments
                    cout << "Invalid number of operations" << endl;
                    break;
                }
                for(int i = 0; i < 5; i++){ // finds the io to allow for it to be assigned
                    if(ios[i] == cmds[1]){
                        io = i;
                        break;
                    }
                    io = -1; // if loop never breaks, op does not exist so -1
                }
                switch (io){
                    case 0:
                        error += ready.enqueue(blk0.fetch(), "Ready", -1, tickTot); // runs first release, checks if it was empty
                        while(ready.enqueue(blk0.fetch(), "Ready", -1, tickTot) == 0); // runs remaining releases, if any
                        break;
                    case 1:
                        error += ready.enqueue(blk1.fetch(), "Ready", -1, tickTot);
                        while(ready.enqueue(blk1.fetch(), "Ready", -1, tickTot) == 0);
                        break;
                    case 2:
                        error += ready.enqueue(blk2.fetch(), "Ready", -1, tickTot);
                        while(ready.enqueue(blk2.fetch(), "Ready", -1, tickTot) == 0);
                        break;
                    case 3:
                        error += ready.enqueue(blk3.fetch(), "Ready", -1, tickTot);
                        while(ready.enqueue(blk3.fetch(), "Ready", -1, tickTot) == 0);
                        break;
                    default:
                        cout << "Invalid io device, valid io devices are 0-3" << endl;
                        error = 2;
                }
                if(error == 1){
                    cout << "No processes were waiting on io device " << io << endl;
                }
                else{
                    tick = 1;
                }
                break;
            case 3: // query case
                if((num != 2) && (num != 1)){ // checks for correct number of arguments
                    cout << "Invalid number of operations" << endl;
                    break;
                }
                if(cmds[1] == "all"){
                    start.printAll();
                    ready.printAll();
                    running.printAll();
                    blk0.printAll();
                    blk1.printAll();
                    blk2.printAll();
                    blk3.printAll();
                    end.printAll();
                }
                else{
                    error += start.print(cmds[1]); // adds 1 when no item found, adds 0 otherwise
                    error += ready.print(cmds[1]);
                    error += running.print(cmds[1]);
                    error += blk0.print(cmds[1]);
                    error += blk1.print(cmds[1]);
                    error += blk2.print(cmds[1]);
                    error += blk3.print(cmds[1]);
                    error += end.print(cmds[1]);
                }
                if(error == 8){ // error on all checks
                    cout << "Process " << cmds[1] << " does not exist" << endl;
                }
                break;
            case 4: // release case
                if(num != 1){ // checks for correct number of arguments
                    cout << "Invalid number of operations" << endl;
                    break;
                }
                error += end.enqueue(running.fetch(), "Exit", -1, tickTot);
                if(error == 0){ // process was sent to exit
                    tick = 32;
                }
                else{ // no process sent to exit
                    cout << "No process currently in the running state" << endl;
                }
                break;
            case 5: // step case
                if(num != 1){ // checks for correct number of arguments
                    cout << "Invalid number of operations" << endl;
                    break;
                }
                end.dequeue(1);
                if(((tickTot - blk0.minTick()) > 1024)){ // checks blocked state to see if any process can be released to ready
                    ready.enqueue(blk0.fetchTick(blk0.minTick()), "Ready", -1, tickTot); // as it is a fifo queue, we can assume the head is the oldest process
                }
                if(((tickTot - blk1.minTick()) > 1024)){ // 1024 ticks must pass, so when greater than 1024 have passed release
                    ready.enqueue(blk1.fetchTick(blk1.minTick()), "Ready", -1, tickTot);
                }
                if(((tickTot - blk2.minTick()) > 1024)){
                    ready.enqueue(blk2.fetchTick(blk2.minTick()), "Ready", -1, tickTot);
                }
                if(((tickTot - blk3.minTick()) > 1024)){
                    ready.enqueue(blk3.fetchTick(blk3.minTick()), "Ready", -1, tickTot);
                }
                ready.enqueue(start.fetch(), "Ready", -1, tickTot); // new process will be first into ready
                ready.enqueue(running.fetch(), "Ready", -1, tickTot); // run process will be next
                error += running.enqueue(ready.fetchTick(ready.minTick()), "Running", -1, tickTot); // grabs the node with the lowest tick value, and therefore the longest since it ran
                if(error == 0){
                    tick = 256; // process moved from ready to running for 256 ticks
                }
                else{
                    tick = 1; // no process moved to running
                }
                break;
            case 6: // wait case
                if(num != 2){ // checks for correct number of arguments
                    cout << "Invalid number of operations" << endl;
                    break;
                }
                for(int i = 0; i < 5; i++){ // finds the io to allow for it to be assigned
                    if(ios[i] == cmds[1]){
                        io = i;
                        break;
                    }
                    io = -1; // if loop never breaks, op does not exist so -1
                }
                switch (io){
                    case 0:
                        error += blk0.enqueue(running.fetch(), "Blocked", io, tickTot);
                        break;
                    case 1:
                        error += blk1.enqueue(running.fetch(), "Blocked", io, tickTot);
                        break;
                    case 2:
                        error += blk2.enqueue(running.fetch(), "Blocked", io, tickTot);
                        break;
                    case 3:
                        error += blk3.enqueue(running.fetch(), "Blocked", io, tickTot);
                        break;
                    default:
                        cout << "Invalid io device, valid io devices are 0-3" << endl;
                        error = 2; // stops the +1 tick
                }
                if(error == 1){
                    cout << "No process running to assign io too" << endl;
                }
                else{
                    tick = 1;
                }
                break;
            default: // error case
                cout << "Invalid input" << endl;
        }
        tickTot += tick; // stores global ticks, note as it is an unsigned int, will max out at 2^32
        tickDis += tick; // stores displayed ticks resets at 10^9, so useless for checking length of time
        if(tickDis > 999999999){ // display tick is above display value width
            tickDis -= 1000000000; // resets display tick
        }
    }
    start.removeAll();
    ready.removeAll();
    running.removeAll();
    blk0.removeAll();
    blk1.removeAll();
    blk2.removeAll();
    blk3.removeAll();
    end.removeAll();
    return 0;
}
