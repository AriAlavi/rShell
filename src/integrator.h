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
    string outputfile = "";
    string outputfile_app = "";
    string inputfile = "";
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
        if (argument.find(">>") != string::npos) {
            size_t pos = argument.find(">>");
            outputfile_app = argument.substr(pos);
            outputfile_app.erase(outputfile_app.begin(), outputfile_app.begin()+3);
            argument.replace(pos, argument.size()-1, "");
            argument.pop_back();
        }
        else if (argument.find(">") != string::npos) {
            size_t pos = argument.find(">");
            outputfile = argument.substr(pos);
            outputfile.erase(outputfile.begin(), outputfile.begin()+2);
            argument.replace(pos, argument.size()-1, "");
            argument.pop_back();

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
            if (outputfile != "") {
                current = makeConnector(connector, (new OutRedir(com1, argument, outputfile)), next);
            }
            else if (outputfile_app != "") {
                current = makeConnector(connector, (new DubOutRedir(com1, argument, outputfile_app)), next);
            }
            else{
                current = makeConnector(connector, (new SysCommand(com1, argument)), next); /* conn2 -> next = connector */
            }
        }
        next = current;
    }

    HeadConnector* head = new HeadConnector(current);
    return head;
}



HeadConnector* superIntegrate(vector <preConnector> bigVec){
    vector<parenShading> shaders;
    shaders = constructShading(bigVec); //Get the location of all parans
    if(shaders.size() == 1 and shaders.at(0).priority == -1){ //Check for unmatched parans
        TailConnector* failTail = new TailConnector();
        HeadConnector* dummyHead = new HeadConnector(failTail);
        cout << "Error: Unmatched parentheses" << endl;
        return dummyHead;
    }


    
    if(shaders.size() != bigVec.size()){ //Shaders should be linked 1:1 with the input bigVec...
        throw __throw_logic_error;
    }

    for(int i = 0; i < bigVec.size(); i++){ //...therefore, match them with each other. 
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
        return integrate(bigVec); //If we have no parans, the integrating is done is just one round
    }


    for(int i = 0; i <= maxDepth; i++){//In what order should the different parantheses be executed? 
        for(int j = 0; j < bigVec.size(); j++){
            if(shaders.at(j).priority == i){
                if(not(find(executionOrder.begin(), executionOrder.end(), shaders.at(j).parent) != executionOrder.end())){
                    int value = shaders.at(j).parent;
                    executionOrder.push_back(value); //Create a vector in which the last element is the unique parent value of a group of shaders
                }
            }
            
        }
    }
    shaders.clear();// Please no more direct reference shaders after this point, they are now connected to bigVec
    while(executionOrder.size() > 0){//Execution order.size should equal the number of paran groups

        int cur = executionOrder.back(); //The the deepest, unevualated paran group's unique identifier
        executionOrder.pop_back();

        vector<preConnector> toExecute;
        parenLocation locations = findParens(bigVec, cur); //Using the unique identifier, find which range of commands correspond to the current paran group (from the unique identifier) 
        if(locations.first == -1 or locations.last == -1){
            throw __throw_logic_error;//This should be impossible
        }
        for(int i = locations.first; i <= locations.last; i++){ //Given the corresponding range of commands, now fetch them into a single execution vector
            bigVec.at(i).shade.parent = cur;
            toExecute.push_back(bigVec.at(i));
        }
        HeadConnector* headResult = integrate(toExecute); //Feed that integrator in a vector and then...
        ParenCommand* result = new ParenCommand(headResult);//Wrap it in a parancommand parantheses so it can be attached to more connectors further on
        
        bigVec.erase(bigVec.begin() + locations.first, bigVec.begin() + locations.last+1);//Remove the just integrated command
        preConnector newResult = preConnector();
        newResult.head = result;//and then feed the paran into the head of a new preConnector to be passed into the integrator at a future time
        if(locations.first > 0){//If it was not at the start of the user input...          
            newResult.shade = bigVec.at(locations.first-1).shade;//... then it needs the data from the previous command...
            newResult.connector = bigVec.at(locations.first-1).connector;//... because it needs to know what type of connector it came from for the integrator
            bigVec.insert(bigVec.begin() + locations.first, newResult);//... and add it back tot he list...
        }//... . The point of all this was to replace the following list ["(", "echo A", "echo B", ")"] with ["PARAN_COMMAND"], which then can be passed into the integrator, still holding pointers to echo B and echo A.
        else{//If the paran was the first thing the user inputted...
            newResult.connector = ";";//... then it should always be executed, so it needs a semicolon connector...
            bigVec.insert(bigVec.begin(), newResult);//... and it should be inputted at the start of the list since it was the first input from the user.
        }

    }

    return integrate(bigVec);//idk why we need this, but it works 100% of the time because of it. Maybe in the case of "(echo A && echo B) || (echo C && echo D)", since there is no zero depth unique identifier, it needs one more integrate to link them all together. 
}

#endif