#include "results.h"
#include "commands.h"
#include "connectors.h"
#include "parser.h"
#include "integrator.h"


#include <vector>
#include <sstream>
#include <iostream>
#include <string>
#include <stdlib.h>


int main(int argv, char** argc){
    Command* base = new SysCommand("echo", "ping");
    PipeCommand* bob = new PipeCommand("tr", "a-z A-Z", base);
    cout << bob -> execute() -> getResult();
    sleep(50000);
    throw __throw_logic_error;


    string input;
    HeadConnector* head;
    TailConnector* tail;
    Result* result;


    if(argv > 1){
        string input = "";
        for(int i = 1; i < argv; i++) {
            if(input.length() > 0){
                input += " ";
            }
            input = string(argc[i]);
        }
        head = superIntegrate(parse(input));

        head -> execute();
        return 1;
    }else{
        bool keepRunning = true;
        while (keepRunning) { //exit command is "exit" or "Exit"
            cout << "$ ";
            getline(cin, input);
            head = superIntegrate(parse(input));

            result = head -> execute();

            keepRunning = head -> keepRunning();       

        }
    }



    

    return 1;
}