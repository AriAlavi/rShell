#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <string>
#include <vector>



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
        Result* execute();
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

class OutRedir: public Command{
    private:
        string file;
    public:
        OutRedir(string command, string args, string file) {
            this -> command = command; 
            this -> args = args;
            this -> file = file;
        }
        Result* execute();
};

class DubOutRedir: public Command{
    private:
        string file;
    public:
        DubOutRedir(string command, string args, string file) {
            this -> command = command; 
            this -> args = args;
            this -> file = file;
        }
        Result* execute();
};

class InRedir: public Command{
    private:
        string file;
    public:
        InRedir(string command,string argument, string file) { //for some reason, it does not work when args are instantiated
            this -> command = command; 
            this -> args = argument;
            this -> file = file;
        }
        Result* execute();
};



#endif