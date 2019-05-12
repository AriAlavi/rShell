#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "commands.h"
#include "connectors.h"
#include "results.h"

using namespace std;

// ; = 1
// || = 2
// && = 3

int isConnector(string s){
     vector<string> POSSIBLE_CONNECTORS{ ";", "||", "&&"} ;
     //If adding new connectors, please add to the END, and do not change the order
    if(s.back() == ';'){
        return 1;
    }

    for(int k = 0; k < POSSIBLE_CONNECTORS.size(); k++){
        if(s == POSSIBLE_CONNECTORS.at(k)){
            return k + 1;
        }
    }
    return 0;
    
}

string appendString(string baseString, string newString){
    if(baseString == ""){
        return newString;
    }else{
        return baseString + " " + newString;
    }
}

vector <vector<string> > parse(string s) {
    istringstream ss(s);
    vector<string> tempList;
    vector <vector<string> > bigVec;
    if (s.length() == 0) { /* no string exists :( */
        return bigVec;
    }
    while (ss) {
        string input;
        ss >> input;
        tempList.push_back(input);
    }


    string command = "";
    string args = "";
    string connector = "";

    bool comment = false;

    for(int i = 0; i< tempList.size(); i++){
        int connector_result = 0;
        string currentPhrase = tempList.at(i);
        if(currentPhrase == ""){
            continue;
        }

        if(currentPhrase.front() == '"'){
            currentPhrase.erase(0, 1);
            comment = true;
        }else{ //If not a quote...
            if(comment==false){connector_result=isConnector(currentPhrase);}//...check for connectors
            if(comment == false and currentPhrase.front() == '#'){connector = ';';break;}//... and if comment, treat as semicolon and stop parsing
        }


        if(currentPhrase.back() == '"'){//Check if end of quote
            comment = false;
            currentPhrase.pop_back();
        }

        
        


        

        if(command == ""){ //If vector is empty, then you have found a command, put it in
            command = currentPhrase;
        }
        else if(connector_result > 0){ //If you find a connector...
            if(connector_result == 1){ //...and it is a semicolon...
                if(currentPhrase.size() == 1){ //...that is freestanding
                    connector = currentPhrase; //...just add it to the connector index
                }
                else{ //...that is stuck to the end of an argument
                    args = appendString(args, currentPhrase.substr(0, currentPhrase.size()-1));
                    connector = currentPhrase.back(); //...rip it off
                }
            }else{//...and it is not a semicolon...
                connector = currentPhrase;//...it must be free standing
            }
            vector<string> thisResult; //...then you have finished the arguments, so return it
            thisResult.push_back(command);
            thisResult.push_back(args);
            thisResult.push_back(connector);
            bigVec.push_back(thisResult);
            command = "";
            args = "";
            connector = "";

        }else{ //If what you find is not a command nor a connector, it must be an argument, so add it to args
            args = appendString(args, currentPhrase);
        }
    }//If after looping through...

    if(connector == "" and command != ""){//...you have not found a command
        connector = ";";//...treat it as a semicolon
        vector<string> thisResult;
        thisResult.push_back(command);
        thisResult.push_back(args);
        thisResult.push_back(connector); 
        bigVec.push_back(thisResult);//...and make sure it is in the list
    }

    reverse(bigVec.begin(),bigVec.end());
    return bigVec;
}
