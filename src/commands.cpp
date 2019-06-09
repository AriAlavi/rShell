#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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

Result* OutRedir::execute() {
    int fd; // 0 = stdin, 1 = stdout, 2 = stderr
    int result;
    int stdout = dup(1); // save stdout to revert back later

    fd = open(this -> file.c_str(), O_RDWR | O_CREAT | O_TRUNC,0666); //overwrites file or creates a new one

    if (fd < 0) {
        //something went wrong...
        perror("Error");
        exit(1);
    }

    result = dup2(fd, 1); // replace stdout w/ file

    if(result < 0) {
        perror("Error");
        exit(1);
    }

    char* args[3];
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
            dup2(stdout, 1);
            close(stdout);
            return new Result(false);
        }else{
            dup2(stdout, 1);
            close(stdout);
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

Result* InRedir::execute() {
    int fd; // 0 = stdin, 1 = stdout, 2 = stderr
    int result;
    int result2;
    int fd2;
    int stdin = dup(0); // save stdin to revert back later
    int stdout = dup(1);

    fd = open(this -> file.c_str(),O_RDONLY); //opens input file

    if (fd < 0) {
        //something went wrong...
        perror("Error");
        exit(1);
    }

    if (file2 != "")
        fd2 = open(this -> file2.c_str(), O_RDWR | O_CREAT | O_TRUNC,0666); // open output file

    result = dup2(fd, STDIN_FILENO); // replace stdin w/ file
    result2 = dup2(fd2, STDOUT_FILENO);
    if(result < 0) {
        perror("Error");
        exit(1);
    }

    char* args[3];
    args[0] = (char*)this -> command.c_str();
    if (this -> args != "") {
        args[1] = (char*)this -> args.c_str();
        args[2] = NULL;
    }
    else {
        args[1] = NULL;
    }
    pid_t pid = fork();
    if(pid == -1){
        throw __throw_runtime_error;
    }
    else if(pid > 0){
        int returnval = 0;
        wait(&returnval);
        if(returnval != 0){
            dup2(stdin, STDIN_FILENO);
            close(stdin);
            dup2(stdout, STDOUT_FILENO);
            return new Result(false);
        }else{
            dup2(stdin, STDIN_FILENO);
            dup2(stdout, STDOUT_FILENO);
            close(stdin);
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

Result* DubOutRedir::execute() {
    int fd; // 0 = stdin, 1 = stdout, 2 = stderr
    int result;
    int stdout = dup(1); // save stdout to revert back later

    fd = open(this -> file.c_str(), O_RDWR | O_CREAT | O_APPEND, 0666); //appends to file or creates a new one

    if (fd < 0) {
        //something went wrong...
        perror("Error");
        exit(1);
    }

    result = dup2(fd, 1); // replace stdout w/ file

    if(result < 0) {
        perror("Error");
        exit(1);
    }

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
            dup2(stdout, 1);
            close(stdout);
            return new Result(false);
        }else{
            dup2(stdout, 1);
            close(stdout);
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

Result* PipeCommand::execute(){
    cout << "PIPE" << endl;
    return new Result(true);
}