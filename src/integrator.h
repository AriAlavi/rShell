#include <string>
#include <vector>

#include "connectors.h"
#include "commands.h"
#include "results.h"

using namespace std;

struct tailhead {TailConnector* t = NULL; HeadConnector* h = NULL;};

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
    return NULL;
}

vector <tailhead*> integrate(vector <vector<string> > bigVec) {
    TailConnector* tail = new TailConnector();
    string com1, argument;
    Connector* connector;
    Command* command;
    vector <tailhead*> vec;
    tailhead* tailhead;

    tailhead -> t = tail;
    if (bigVec.size() == 0) { /* if there is nothing to integrate */
        return vec;
    }
    com1 = bigVec[0][0];
    argument = bigVec[0][1];
    if (com1 == "exit") {
        command = new ExitCommand();
    }
    else {
        command = new SysCommand(com1,argument);
    }
    connector = makeConnector(bigVec[0][2], command, tail); /* function to return appropriate connector */

    if (bigVec.size() > 1) { 
        for (int i = 1; i < bigVec.size(); ++i) {
            com1 = bigVec[i][0];
            argument = bigVec[i][1];
            Connector* conn2;
            if (com1 == "exit") {
                conn2 = makeConnector(bigVec[i][2], (new ExitCommand()), connector);
            }
            else{
                conn2 = makeConnector(bigVec[i][2], (new SysCommand(com1, argument)), connector); /* conn2 -> next = connector */
            }
            connector = conn2;
        }

    }
    HeadConnector* head = new HeadConnector(connector);
    tailhead -> h = head;
    vec.push_back(tailhead);
    return vec;
}