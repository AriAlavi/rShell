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
        Result* execute(){return new ExitResult();}
        
};

class SysCommand: public Command{
    public:
        SysCommand(string command, string args){this -> command = command; 
                                                    this -> args = args;};
        Result* execute();

};

