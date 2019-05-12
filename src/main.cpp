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
    HeadConnector* head;
    

    head = integrate(parse(input));

    bool keepRunning = true;
    while (keepRunning) { //exit command is "exit" or "Exit"
        cout << "$";
        getline(cin, input);
        head = integrate(parse(input));
        head -> execute(new AbsoluteTrue());       

    }
    

    return 1;
}
