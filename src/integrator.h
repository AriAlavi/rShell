#ifndef __INTEGRATOR_H__
#define __INTEGRATOR_H__


#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>

#define GetCurrentDir getcwd


 
using namespace std;

struct parenShading{
    int parent;
    int priority;
};


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

vector<int> findChar(string givenStr, char findChar){
    vector<int> results;
    for(int i = 0; i < givenStr.size(); i++){
        if(givenStr[i] == findChar){
            results.push_back(i);
        }
    }
    return results;
}

vector<parenShading> constructShading(vector<preConnector> preConnectors){
    // Incoming givenStr is reversed


    // vector<int> lefts = findChar(givenStr, ')');
    // vector<int> rights = findChar(givenStr, '(');

    reverse(preConnectors.begin(),preConnectors.end());

    vector<int> lefts;
    int leftIndex = 0;
    vector<int> rights;
    int rightIndex = 0;

    vector<parenShading> returnShading;


    for(int i = 0;i < preConnectors.size(); i++){
        if(preConnectors.at(i).command == ")"){
            rights.push_back(i+1);
        } else if(preConnectors.at(i).command == "("){
            lefts.push_back(i+1);
        }
    }
    if(lefts.size() != rights.size()){
        throw __throw_logic_error;
        }
    stack<int> stacc;
    int currentShading = 0;
    for(int i = 0; i < preConnectors.size(); i++){
        parenShading shade = parenShading();
        if(lefts.size() > 0 and lefts.at(0) == i){
            if(currentShading != 0){
                stacc.push(currentShading);
            }
            
            currentShading = lefts.at(0);
            
            lefts.erase(lefts.begin(), lefts.begin()+1);
        }else if(rights.size() > 0 and rights.at(0) == i){
            if(stacc.empty()){
                currentShading = 0;
            }else{
                currentShading = stacc.top();
                stacc.pop();

            }
            rights.erase(rights.begin(), rights.begin()+1);
        }
        shade.parent = currentShading;
        returnShading.push_back(shade);
    }

    reverse(returnShading.begin(),returnShading.end());
    return returnShading;
}

HeadConnector* integrate(vector <preConnector> bigVec) {
    TailConnector* tail = new TailConnector();
    string com1, argument;
    Command* command;
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    if (bigVec.size() == 0) { /* if there is nothing to integrate */
        HeadConnector* head = new HeadConnector(tail);
        return head;
    }

    Connector* next = tail;
    Connector* current;
    string connector;

    for (int i = 0; i < bigVec.size(); ++i) {
        com1 = bigVec.at(i).command;
        argument = bigVec.at(i).argument;

        if(i == bigVec.size()-1){
            connector = ";";
        }else{
            connector = bigVec.at(i+1).connector;
        }

        if (com1 == "ls" && argument == "") {
            argument = cwd;
        }
        
        if (com1 == "exit") {
            current = makeConnector(connector, (new ExitCommand()), next);
        }
        else if (com1 == "test") {
            current = makeConnector(connector, (new TestCommand(com1, argument)), next);
        }
        else if (com1 != "" and com1.at(0) == '[') { // [argument]
            if (argument == "") {
                argument = com1;
                argument.erase(0,1);
                argument.pop_back();
                com1 = "test";
                current = makeConnector(connector, (new TestCommand(com1, argument)), next);
            }
            else if (com1 != "[") { // [-flag argument]
                argument = com1 + " " + argument;
                argument.erase(0,1);
                argument.pop_back();
                com1 = "test";
                current = makeConnector(connector, (new TestCommand(com1, argument)), next);
            }
            else if (com1 != ""){
                com1.replace(0,1,"test"); // [ argument ]
                argument.pop_back();
                argument.pop_back();
                current = makeConnector(connector, (new TestCommand(com1, argument)), next);
            }

        }

        else{
            current = makeConnector(connector, (new SysCommand(com1, argument)), next); /* conn2 -> next = connector */
        }
        next = current;
    }

    HeadConnector* head = new HeadConnector(current);
    return head;
}



HeadConnector* superIntegrate(vector <preConnector> bigVec){
    constructShading(bigVec);
    //return integrate(bigVec);

}

#endif