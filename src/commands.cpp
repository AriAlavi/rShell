#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <iostream>

#include "results.h"
#include "commands.h"
#include "connectors.h"

using namespace std;





Result* SysCommand::execute(){
    char* args[3]; //Thanks for that prototype malhar ;)
    args[0] = (char*)this -> command.c_str();
    args[1] = (char*)this -> args.c_str();
    args[2] = NULL;
    pid_t pid = fork();
    if(pid == -1){
        throw __throw_runtime_error;
    }
    else if(pid > 0){
        int returnval = 0;
        wait(&returnval);
        if(returnval != 0){
            return new Result(false);
        }else{
            return new Result(true);
        }
        
    }else{
        int result = execvp(args[0], args);
        if(result == -1){
            perror("Error");
            exit(-1);
        }        
    }
}

bool TestCommand::exists(string file) {
    struct stat validate;

    if (file.find("-d")!= string::npos) {   // checks if it's a directory
        file.replace(file.find("-d"), 3, "");
        if (stat(file.c_str(), &validate) != 0) {
            return false;
        }
        return S_ISDIR(validate.st_mode);
    }
    if (file.find("-f")!= string::npos) {   // checks if it's a regular file
        file.replace(file.find("-f"), 3, "");
        if (stat(file.c_str(), &validate) != 0) {
            return false;
        }
        return S_ISREG(validate.st_mode);
    }
    if (file.find("-e")!= string::npos) {
        file.replace(file.find("-e"), 3, "");  // checks if file exists
    
    } 
    return(stat(file.c_str(), &validate) == 0);   
}


Result* TestCommand::execute() {
    if (exists(this -> args)) {
        cout << "(True)" << endl;
        return new Result(true);
    }
    cout << "(False)" << endl;
    return new Result(false);
}
