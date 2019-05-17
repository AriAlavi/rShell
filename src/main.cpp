#include "results.h"
#include "commands.h"
#include "connectors.h"
#include "parser.h"
#include "integrator.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <string>

int main(int argv, char** argc){

    string input;
    executePayload result;
    HeadConnector* head;
    TailConnector* tail;

    if(argv > 1){
        string input = "";
        for(int i = 1; i < argv; i++) {
            if(input.length() > 0){
                input += " ";
            }
            input = string(argc[i]);
        }
        result = integrate(parse(input));
        head = result.head;
        tail = result.tail;

        head -> execute();
        return 1;
    }else{
        bool keepRunning = true;
        while (keepRunning) { //exit command is "exit" or "Exit"
            cout << "$ ";
            getline(cin, input);
            result = integrate(parse(input));
            head = result.head;
            tail = result.tail;

            head -> execute();

            keepRunning = tail -> keepRunning();       

        }
    }



    

    return 1;
}
