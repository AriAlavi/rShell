#ifndef __CONNECTORS_H__
#define __CONNECTORS_H__

#include "results.h"
#include "commands.h"


class Connector{
    
    protected:
        string type = "Abstract";
        Connector* next;
        Command* command;
    public:
        Connector(){};
        virtual void execute(Result*) = 0;
        
};


class FailConnector:public Connector{
    public:
        string type = "Fail";
        FailConnector(Connector* next, Command* command){this -> next = next; this -> command = command;};
        void execute(Result*);
};

class PassConnector:public Connector{
    public:
        string type = "Pass";
        PassConnector(Connector* next, Command* command){this -> next = next; this -> command = command;};
        void execute(Result*);
};

class AnyConnector:public Connector{
    public:
        string type = "Any";
        AnyConnector(Connector* next, Command* command){this -> next = next; this -> command = command;};
        void execute(Result*);
};

class HeadConnector:public Connector{
    public:
        string type = "Head";
        HeadConnector(Connector* next){this -> next = next;};
        HeadConnector(){};
        void setNext(Connector* next){this -> next = next;};
        void execute(Result* res);
        void execute(){this -> execute(new AbsoluteTrue());};
};

class TailConnector:public Connector{
    private:__RESULTS_H__
        bool noExit = true;
    public:
        string type = "Next";
        TailConnector(Connector* next){this -> next = next;};
        TailConnector(){};
        void setNext(Connector* next){this -> next = next;};
        void execute(Result* res){
            if(res -> getResult() == -1){
                this -> noExit = false;
            }
            //cout << res -> getResult() << endl;}; //Uncomment this if want to test
        }
        bool keepRunning(){
            return this -> noExit;
        }
};

#endif