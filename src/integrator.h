#include <string>
#include <vector>

#include "connectors.h"
#include "commands.h"
#include "results.h"

using namespace std;

Connector* makeConnector(string type, Command* com, Connector* next) {
    if (type == ";") {
        return new AnyConnector(next,com);
    }
    if (type == "&&") {
        return new PassConnector(next,com);
    }
    if (type == "||") {
        return new FailConnector(next,com);
    }
    throw __throw_logic_error;
    return NULL;
}

struct executePayload{
    HeadConnector* head;
    TailConnector* tail;
};


executePayload integrate(vector <vector<string> > bigVec) {
    TailConnector* tail = new TailConnector();
    string com1, argument;
    Command* command;

    if (bigVec.size() == 0) { /* if there is nothing to integrate */
        HeadConnector* head = new HeadConnector(tail);
        executePayload empty = executePayload();
        empty.head = head;
        empty.tail = tail;
        return empty;
    }

    Connector* next = tail;
    Connector* current;
    string connector;

    for (int i = 0; i < bigVec.size(); ++i) {
        com1 = bigVec.at(i).at(0);
        argument = bigVec.at(i).at(1);
        if(i == bigVec.size()-1){
            connector = ";";
        }else{
            connector = bigVec.at(i+1).at(2);
        }
        
        if (com1 == "exit") {
            current = makeConnector(connector, (new ExitCommand()), next);
        }
        else{
            current = makeConnector(connector, (new SysCommand(com1, argument)), next); /* conn2 -> next = connector */
        }
        next = current;
    }

    HeadConnector* head = new HeadConnector(current);
    executePayload result = executePayload();
    result.head = head;
    result.tail = tail;
    return result;
}