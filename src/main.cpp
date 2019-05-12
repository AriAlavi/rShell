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
    TailConnector* tail;
    vector<tailhead*> vec;
    
    cout << "$ ";
    getline(cin, input);
    vec = integrate(parse(input));
    tail = vec[0] -> t;
    head = vec[0] -> h;

    while (tail -> keepRunning()) { //exit command is "exit" or "Exit"
        head -> execute(new AbsoluteTrue());
        cout << "$ ";
        getline(cin, input);
        vec = integrate(parse(input));
        tail = vec[0] -> t;
        head = vec[0] -> h;
    }
    

    return 1;
}
