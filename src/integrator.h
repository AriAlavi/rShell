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

struct parenLocation{
    int first;
    int last;
};

parenLocation findParens(vector <preConnector> given, int parenNumber){
    int first = -1;
    int last = -1;

    for(int i = 0; i < given.size(); i++){
        if(given.at(i).shade.parent == parenNumber){
            if(first == -1){
                first = i;
            }
            last = i;
        }
    }

    parenLocation returnval = parenLocation();
    returnval.first = first;
    returnval.last = last;
    return returnval;
}

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
    return new AnyConnector(next, com);

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


    /*
    The purpose of this function is to take in the parsed user input and
    output a vector of shading. This shaded vector indicates the location
    of parantheses with a unique integer identifier (this is the parent member).
    In addition, this function shades in the depth of the parantheses
    (this is the priority member).

    When returned, the priority should be used in order to denote which set of
    parantheses to run.

    */



    vector<int> lefts;
    vector<int> rights;
    vector<int> uniqueValues;



    vector<parenShading> returnShading;


    for(int i = 0;i < preConnectors.size(); i++){
        if(preConnectors.at(i).command == ")"){
            rights.push_back(i+1);
        } else if(preConnectors.at(i).command == "("){
            lefts.push_back(i+1);
        }
    }
    if(lefts.size() != rights.size()){
        parenShading failed = parenShading();
        failed.parent = -1;
        failed.priority = -1;
        returnShading.push_back(failed);
        return returnShading;
        }

    if(lefts.size() == 0){
        for(auto i: preConnectors){
            parenShading shade = parenShading();
            shade.parent = 0;
            shade.priority = 0;
            returnShading.push_back(shade);
        }
        return returnShading;
    }

    for(int i = 0; i < rights.at(rights.size()-1);i++){
        uniqueValues.push_back(0);
    }
    for(int i = 0; i < lefts.size(); i++){
        uniqueValues.at(lefts.at(i)) = i+1; 
    }


    stack<int> stacc;
    int currentShading = 0;
    int depth = 0;
    for(int i = 0; i < preConnectors.size(); i++){
        parenShading shade = parenShading();
        if(lefts.size() > 0 and lefts.at(0) == i){
            if(currentShading != 0){
                stacc.push(currentShading);
            }
            depth++;
            currentShading = lefts.at(0);
            
            lefts.erase(lefts.begin(), lefts.begin()+1);
        }else if(rights.size() > 0 and rights.at(0) == i){
            if(stacc.empty()){
                depth = 0;
                currentShading = 0;
            }else{
                depth--;
                currentShading = stacc.top();
                stacc.pop();

            }
            rights.erase(rights.begin(), rights.begin()+1);
        }
        shade.parent = currentShading;
        shade.priority = depth;
        returnShading.push_back(shade);
    }

    for(int i = 0; i < returnShading.size(); i++){
        returnShading.at(i).parent = uniqueValues.at(returnShading.at(i).parent);
    }

    for(int i = 0; i < returnShading.size(); i++){
        if(preConnectors.at(i).command == "("){
            returnShading.at(i) = returnShading.at(i+1);
        }
    }

    return returnShading;
}

HeadConnector* integrate(vector <preConnector> bigVec) {
    reverse(bigVec.begin(),bigVec.end());

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

        if(com1 == ")"){
            continue;
        }
        if(com1 == "("){
            ParenCommand* parenNext = bigVec.at(i-1).head;
            if(parenNext != nullptr){
                Connector* parenConn = makeConnector(bigVec.at(i).connector, parenNext, next);
                current = parenConn;
                next = current;
                continue;
            }
            continue;
        }
        if(com1 == "" and bigVec.at(i).head != nullptr){
            Connector* parenPlaceholder = makeConnector(bigVec.at(i).connector, bigVec.at(i).head, next);
            current = parenPlaceholder;
            next = current;
            continue;
        }
        else if(com1 == ""){
            continue;
        }

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
    vector<parenShading> shaders;
    shaders = constructShading(bigVec);
    if(shaders.size() == 1 and shaders.at(0).priority == -1){
        TailConnector* failTail = new TailConnector();
        HeadConnector* dummyHead = new HeadConnector(failTail);
        cout << "Error: Unmatched parentheses" << endl;
        return dummyHead;
    }


    
    if(shaders.size() != bigVec.size()){
        throw __throw_logic_error;
    }

    for(int i = 0; i < bigVec.size(); i++){
        bigVec.at(i).shade = shaders.at(i);
    }

    int parens = 0;
    int maxDepth = 0;

    vector<HeadConnector* > headVector;
    headVector.resize(parens+1, 0);

    vector<int> executionOrder;


    for(auto i : shaders){
        if(i.parent > parens){
            parens = i.parent; //How many parantheses do we have to deal with?
        }
        if(i.priority > maxDepth){
            maxDepth = i.priority; //How deep is the deepest parantheses
        }
    }

    if(parens == 0){
        return integrate(bigVec);
    }


    for(int i = 0; i <= maxDepth; i++){//In what order should the different parantheses be executed
        for(int j = 0; j < bigVec.size(); j++){
            if(shaders.at(j).priority == i){
                if(not(find(executionOrder.begin(), executionOrder.end(), shaders.at(j).parent) != executionOrder.end())){
                    int value = shaders.at(j).parent;
                    executionOrder.push_back(value);
                }
            }
            
        }
    }
    shaders.clear();// Please no more direct to shaders after this point
    while(executionOrder.size() > 0){

        int cur = executionOrder.back();
        executionOrder.pop_back();

        vector<preConnector> toExecute;
        parenLocation locations = findParens(bigVec, cur);
        if(locations.first == -1 or locations.last == -1){
            throw __throw_logic_error;
        }
        for(int i = locations.first; i <= locations.last; i++){
            bigVec.at(i).shade.parent = cur;
            toExecute.push_back(bigVec.at(i));
        }
        HeadConnector* headResult = integrate(toExecute);
        ParenCommand* result = new ParenCommand(headResult);
        
        
        // bigVec.at(locations.last).head = result;
        
        bigVec.erase(bigVec.begin() + locations.first, bigVec.begin() + locations.last+1);
        if(locations.first > 0){
            preConnector firstParen = bigVec.at(locations.first-1);
            preConnector newResult = preConnector();
            newResult.head = result;
            newResult.shade = firstParen.shade;
            newResult.connector = bigVec.at(locations.first-1).connector;
            bigVec.insert(bigVec.begin() + locations.first, newResult);
        }
        else{
            preConnector newResult = preConnector();
            newResult.head = result;
            newResult.connector = ";";
            bigVec.insert(bigVec.begin(), newResult);
        }

    }

    return integrate(bigVec);
    // TailConnector* tail = new TailConnector();
    // Connector* current = tail;
    // for(int i = bigVec.size()-1; i >= 0; i--){
    //     if(i == bigVec.size()-1 and bigVec.at(i).command == ")"){

    //     }else if(bigVec.at(i).command == "("){
    //         Connector* newConnector = makeConnector(bigVec.at(i).connector, bigVec.at(i+1).head,current);
    //         current = newConnector;
    //     }else if(bigVec.at(i).command == ")"){

    //     }else{
    //         vector<preConnector> lazy;
    //         lazy.push_back(bigVec.at(i));
    //         Connector* newConnector = integrate(lazy);
    //         current = newConnector;
    //     }

    // }
    // HeadConnector* finalConnector = new HeadConnector(current);


    // return finalConnector;

}

#endif