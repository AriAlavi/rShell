#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <string>
#include <vector>
#include "results.h"

using namespace std;

class Command{
    public:
        string command;
        string args;
        Command(){};
        virtual Result* execute() = 0;
        
};

class ExitCommand: public Command{
    public:
        ExitCommand(){};
        Result* execute(){return new ExitResult();}
        
};

class SysCommand: public Command{
    public:
        SysCommand(string command, string args){this -> command = command; 
                                                    this -> args = args;};
        Result* execute();

};

class TestCommand: public Command{
    public:
        TestCommand(string command, string args) {
            this -> command = command; 
            this -> args = args;
        }
        bool exists(string);
        Result* execute();

};

#endif