#include "connectors.h"


using namespace std;

Result* HeadConnector::execute(Result* result){
    this -> next -> execute(new AbsoluteTrue());
}

Result* AnyConnector::execute(Result* result){
    switch(result -> getResult()){
        case -1:
            this -> next -> execute(result);
            break;
        default:
            Result* nextResult = this -> command -> execute();
            this -> next -> execute(nextResult);

    }
    
}

Result* FailConnector::execute(Result* result){
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

Result* PassConnector::execute(Result* result){
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
