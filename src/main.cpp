#include "results.h"
#include "commands.h"
#include "connectors.h"
#include "parser.h"
#include "integrator.h"

#include <vector>
#include <sstream>
#include <iostream>

bool exit(string s) {
    istringstream ss(s);
    while (ss) {
        string input;
        ss >> input;
        if(input == "exit" || input == "Exit") {
            return true;
        }
    }
    return false;
}

int main(){

    string input;
    HeadConnector* head;
    
    cout << "$ ";
    getline(cin, input);

    while (not exit(input)) { //exit command is "exit" or "Exit"
        head = integrate(parse(input));
        head -> execute(new AbsoluteTrue());
        cout << "$ ";
        getline(cin, input);
    }
    

    return 1;
}
