#include "results.h"
#include "commands.h"
#include "connectors.h"
#include "parser.h"
#include "integrator.h"

#include <vector>
#include <sstream>
#include <iostream>

int main(){

    string input;
    executePayload result;
    HeadConnector* head;
    TailConnector* tail;    

    bool keepRunning = true;
    while (keepRunning) { //exit command is "exit" or "Exit"
        cout << "$";
        getline(cin, input);
        result = integrate(parse(input));
        head = result.head;
        tail = result.tail;

        head -> execute();

        keepRunning = tail -> keepRunning();       

    }
    

    return 1;
}
