#include "commands.h"
#include "results.h"

#include <vector>

using namespace std;

class Command{
    protected:
        vector<char> command;
        vector<char> args;
    public:
        Command(){};
        virtual Result* execute();
        
};

class ExitCommand: private Command{
    public:
        ExitCommand(){};
        Result* execute(){return new ExitResult();}
        
};

class SysCommand: public Command{
    public:
        SysCommand(vector<char> command, vector<char> args){this -> command = command; 
                                                            this -> args = args;};
        Result* execute();

}

