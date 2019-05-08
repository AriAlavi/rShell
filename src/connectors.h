#ifndef __CONNECTORS_H__
#define __CONNECTORS_H__

#include "results.h"
#include "commands.h"

class Connector{
    protected:
        Connector* next;
        Command* command;
    public:
        Connector(){};
        virtual void execute(Result*) = 0;
        
};


class FailConnector:public Connector{
    public:
        FailConnector(Connector* next, Command* command){this -> next = next; this -> command = command;};
        void execute(Result*);
};

class PassConnector:public Connector{
    public:
        PassConnector(Connector* next, Command* command){this -> next = next; this -> command = command;};
        void execute(Result*);
};

class AnyConnector:public Connector{
    public:
        AnyConnector(Connector* next, Command* command){this -> next = next; this -> command = command;};
        void execute(Result*);
};

class HeadConnector:public Connector{
    public:
        HeadConnector(Connector* next){this -> next = next;};
        HeadConnector(){};
        void setNext(Connector* next){this -> next = next;};
        void execute(Result*);
};

class TailConnector:public Connector{
    public:
        TailConnector(Connector* next){this -> next = next;};
        TailConnector(){};
        void setNext(Connector* next){this -> next = next;};
        void execute(Result*){};
};

#endif