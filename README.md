# CS 100 Programming Project
> ### Spring 2019
> ### Malhar Nerurkar (862004989) Arian Alavi (862030914) Hussain Merchant (862027193)

# Introduction
Our program will consist of a command shell that
will be able to read in bash commands, typically located within PATH directories, and execute them using the fork, execvp, and waitpid syscalls. We will be using a composite pattern for our classes.

Inputs will be tokenized by spaces and put into an array of strings. From there, they will be broken up by given connectors. The broken up data is put into a command class, storing its command and arguments. Then, the connector is put into a connector class with reference to it's command and the connector that will come next.

We will be using a linked-list of connectors, of which there are three diffrent types. The different types dictate control flow to all the following commands (passed through an argument).

We will iterate through the connectors and we will pass the result (error or not error) into the execute function of the next connector, based on that input the connector will react. The connector's execute function will then run the execute function of the command, with its args.

In assignment 3, we added the test command to the rshell we have developed as well as its symbolic equivalent [ ]. The square brackets [ ] are actually interpreted as the test command in bash. This command returns 0 (TRUE) if the test succeeds and 1 (FALSE) if the test fails. This command is very useful for writing conditions that can be combined with the && and || connectors to write more complex bash command structures.

# Diagrams
![UML](images/CS100-Assignment_2-UML.jpg)
# Classes

**Command Classes**:
The command classes are classes which execute the given user's commands. They will store the command, and its arguments. When execute is ran it will execute them using a combination of execvp and fork. 

* The system call command subclass will work as stated above.

* The exit command subclass will terminate the program.

**Connector Classes**:
The connector classes will be the classes which determine if the command will actually be ran. The connector also has a pointer to its next connector and will call its execute with the result of its current execution passed in as an argument. 

* Fail connector executes its command if it is passed in a false result.

* Pass connector executes its command if it is passed in a true result.

* The any connector will always executs its code.

If a connector doesn't execute its command, it will pass in the given result into the next result.

**Result Class**:
    A class to store a boolean. It's purpose is to allow for the code to be more easily expanded later on.



# Prototypes/Research
* **waitpid():** Used to pause the calling process until the appropriate system information about a child process is received. More specifically, it waits until the child process has ended or been terminated. The *pid* value determines which child process the system should wait for.

* **execvp():** Searchs a path for a desired program to execute. If a match is found, the program is loaded into memory, effectively replacing the current running program.
    * The name of the program that we are searching for
    * Its arguments (in the form of a null-terminated array of character pointers)
* **fork():** Creates a new child process that runs alongside the parent process. The fork() call returns an integer: 
    * ***Negative value***: represents parent process; indicates that the child process was not created successfully
    * ***Zero***: represents the child process
    * ***Positive Value***: represents the parent process

    The total number of processes is equal to 2<sup>n</sup> where n = # of fork() syscalls.

## ***waitpid()*, *execvp()*, and *fork()* can also be used concurrently:**
* ***execvp()*** : used to run a program (a bash command in our case)

* ***fork()*** : used to continue the program after the first process has been terminated (and to allow for additional processes)
* ***waitpid()*** :  used to ensure that the processes run one-by-one by waiting until the child process has ended before calling the parent process 

**see prototypes for example of this*

## **Connectors:**
**We will be implementing three connectors to separate processes (commands) in our program:**
*  " ; " : will be used to separate multiple processes which be then execute in the given order

* " && " : will be used to run a subsequent process only if the process before it was successful

* " | | " : will be used to run a subsequent process only if the process before it was unsuccessful 


# Development and Testing Roadmap
## Malhar will be in charge of:
* [Parser (1)](https://github.com/cs100/spring-2019-assignment-echo-ping-ping-ping/issues/1)
* [Integration of Connector classes (5)](https://github.com/cs100/spring-2019-assignment-echo-ping-ping-ping/issues/2)
* [Integration of Command classes (6)](https://github.com/cs100/spring-2019-assignment-echo-ping-ping-ping/issues/3)
* [Integration tests (7)](https://github.com/cs100/spring-2019-assignment-echo-ping-ping-ping/issues/4)

## Arian will be in charge of: 
* [Result class (1)](https://github.com/cs100/spring-2019-assignment-echo-ping-ping-ping/issues/5)
* [Command classes (2)](https://github.com/cs100/spring-2019-assignment-echo-ping-ping-ping/issues/6)
* [Connector classes (3)](https://github.com/cs100/spring-2019-assignment-echo-ping-ping-ping/issues/7)
* [Unit tests (4)](https://github.com/cs100/spring-2019-assignment-echo-ping-ping-ping/issues/8)


