#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#include <iostream>
//Todo removes

#include "results.h"
#include "commands.h"

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
        if (result == -1){
            perror("Error");
            exit(-1);
        }        
    }
}