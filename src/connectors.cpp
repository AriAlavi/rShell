#include "connectors.h"


using namespace std;

void HeadConnector::execute(Result* result){
    this -> next -> execute(new AbsoluteTrue());
}

void AnyConnector::execute(Result* result){
    switch(result -> getResult()){
        case -1:
            this -> next -> execute(result);
            break;
        default:
            Result* nextResult = this -> command -> execute();
            this -> next -> execute(nextResult);

    }
    
}

void FailConnector::execute(Result* result){
    switch(result -> getResult()){
        case -1:
            this -> next -> execute(result);
            break;
        case 1:
            this -> next -> execute(result);
            break;
        default:
            Result* nextResult = this -> command -> execute();
            this -> next -> execute(nextResult);
    }
    
}

void PassConnector::execute(Result* result){
    switch(result -> getResult()){
        case -1:
            this -> next -> execute(result);
            break;
        case 0:
            this -> next -> execute(result);
            break;
        default:
            Result* nextResult = this -> command -> execute();
            this -> next -> execute(nextResult);
    }
    
}
