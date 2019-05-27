#include "results.h"
#include "commands.h"
#include "connectors.h"



using namespace std;

Result* HeadConnector::execute(Result* result){
    return this -> next -> execute(result);
}

Result* AnyConnector::execute(Result* result){
    switch(result -> getResult()){
        case -1:
            return this -> next -> execute(result);
            break;
        default:
            Result* nextResult = this -> command -> execute();
            return this -> next -> execute(nextResult);

    }
    
}

Result* FailConnector::execute(Result* result){
    switch(result -> getResult()){
        case -1:
            return this -> next -> execute(result);
            break;
        case 1:
            return this -> next -> execute(result);
            break;
        default:
            Result* nextResult = this -> command -> execute();
            return this -> next -> execute(nextResult);
    }
    
}

Result* PassConnector::execute(Result* result){
    switch(result -> getResult()){
        case -1:
            return this -> next -> execute(result);
            break;
        case 0:
            return this -> next -> execute(result);
            break;
        default:
            Result* nextResult = this -> command -> execute();
            return this -> next -> execute(nextResult);
    }
    
}
