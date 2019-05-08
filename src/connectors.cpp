#include "connectors.h"

using namespace std;

void HeadConnector::execute(Result* result){
    this -> next -> execute(new AbsoluteTrue());
}

void AnyConnector::execute(Result* result){
    Result* nextResult = this -> command -> execute();
    switch(nextResult -> getResult()){
        case -1:
            break;
        default:
            this -> next -> execute(nextResult);

    }
    
}

void FailConnector::execute(Result* result){
    switch(result -> getResult()){
        case 1:
            this -> next -> execute(result);
            break;
        default:
            Result* nextResult = this -> command -> execute();
            if(nextResult -> getResult() == -1){return;}
            this -> next -> execute(nextResult);
    }
    
}

void PassConnector::execute(Result* result){
    switch(result -> getResult()){
        case 0:
            this -> next -> execute(result);
            break;
        default:
            Result* nextResult = this -> command -> execute();
            if(nextResult -> getResult() == -1){return;}
            this -> next -> execute(nextResult);
    }
    
}
