#include "../src/results.h"
#include "../src/commands.h"
#include "../src/connectors.h"
#include "../src/parser.h"
#include "../src/integrator.h"

#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "gtest/gtest.h"

using namespace std;


class ProbeConnector:public Connector{
    public:
        Result* output = NULL;
        ProbeConnector(){};
        Result* execute(Result* result){
            if(this -> output){
                delete this -> output;
            }
            this -> output = result;
            return result;
        }
        int getResult(){
            if (this -> output == NULL){
                return -2;
            }
            return this -> output -> getResult();
        }
};



TEST(CompileTest, Compiles){
    EXPECT_EQ(0, 0);
}

TEST(Results, BadResult){
    Result* res = new Result(false);
    EXPECT_EQ(res->getResult(), 0);
}

TEST(Results, GoodResult){
    Result* res = new Result(true);
    EXPECT_EQ(res->getResult(), 1);
}

TEST(Results, ExitResult){
    Result* res = new ExitResult();
    EXPECT_EQ(res->getResult(), -1);
}

TEST(SysCommands, Basic){
    Command* test = new SysCommand("echo", "ping");
    Result* res = test -> execute();
    EXPECT_EQ(res -> getResult(), 1);
}

TEST(SysCommands, ErrorCommand){
    Command* test = new SysCommand("david's successful solo career", "get it?");
    Result* res = test -> execute();
    EXPECT_EQ(res -> getResult(), 0);   
}

TEST(SysCommands, TwoArgs){
    Command* test = new SysCommand("echo", "ping ping");
    Result* res = test -> execute();
    EXPECT_EQ(res -> getResult(), 1);   
}

TEST(SysCommands, ThreeArgs){
    Command* test = new SysCommand("echo", "ping ping ping");
    Result* res = test -> execute();
    EXPECT_EQ(res -> getResult(), 1);   
}

TEST(Connectors, AnyConnector){
    Command* command = new SysCommand("echo", "ping");
    ProbeConnector* probe = new ProbeConnector();
    Connector* test = new AnyConnector(probe, command);

    test -> execute(new Result(0));

    EXPECT_EQ(probe -> getResult(), 1);
}

TEST(Connectors, FailConnectorPass){
    Command* passcommand = new SysCommand("echo", "ping");
    ProbeConnector* probe = new ProbeConnector();
    Connector* test = new FailConnector(probe, passcommand);

    test -> execute(new Result(0));

    EXPECT_EQ(probe -> getResult(), 1);
}

TEST(Connectors, FailConnectorStopped){
    Command* passcommand = new SysCommand("echo", "ping");
    ProbeConnector* probe = new ProbeConnector();
    Connector* test = new FailConnector(probe, passcommand);

    Result* result = new Result(1);
    test -> execute(result);

    //result -> set_probe(10);

    EXPECT_EQ(probe -> getResult(), 1);
    //EXPECT_EQ(result -> probe(), 10);
}

TEST(Connectors, PassConnectorPassed){
    Command* passcommand = new SysCommand("echo", "ping");
    ProbeConnector* probe = new ProbeConnector();
    Connector* test = new PassConnector(probe, passcommand);

    test -> execute(new Result(1));

    EXPECT_EQ(probe -> getResult(), 1);
}

TEST(Connectors, PassConnectorStopped){
    Command* passcommand = new SysCommand("echo", "ping");
    ProbeConnector* probe = new ProbeConnector();
    Connector* test = new PassConnector(probe, passcommand);

    Result* result = new Result(false);

    test -> execute(result);

    EXPECT_EQ(probe -> getResult(), 0);
    //result -> set_probe(10);
    //EXPECT_EQ(result -> probe(), 10);
}

TEST(Connectors, HeadConnector){
    Command* passcommand = new SysCommand("echo", "ping");
    ProbeConnector* probe = new ProbeConnector();
    Connector* test = new PassConnector(probe, passcommand);
    Connector* head = new HeadConnector(test);

    Result* result = new Result(true);

    head -> execute(result);

    EXPECT_EQ(probe -> getResult(), 1); 
}

TEST(Parsing, Simple) {
    string toTest = "echo hello;";

    vector<preConnector> test = parse(toTest);

    EXPECT_EQ(test[0].command, "echo");
    EXPECT_EQ(test[0].argument, "hello");
    EXPECT_EQ(test[0].connector, ";");
}

TEST(Parsing, Space_Semicolon) {
    string toTest = "echo hello ;";

    vector<preConnector> test = parse(toTest);

    EXPECT_EQ(test[0].command, "echo");
    EXPECT_EQ(test[0].argument, "hello");
    EXPECT_EQ(test[0].connector, ";");
}

TEST(Parsing, Two_Commands) {
    string toTest = "echo hello; echo bye;";

    vector<preConnector> test = parse(toTest);

    ASSERT_EQ(test.size(), 2);
    EXPECT_EQ(test[0].command, "echo");
    EXPECT_EQ(test[0].argument, "hello");
    EXPECT_EQ(test[0].connector, ";");
    EXPECT_EQ(test[1].command, "echo");
    EXPECT_EQ(test[1].argument, "bye");
    EXPECT_EQ(test[1].connector, ";");
}

TEST(Parsing, No_semicolon) {
    string toTest = "echo hello";

    vector<preConnector> test = parse(toTest);

    EXPECT_EQ(test[0].command, "echo");
    EXPECT_EQ(test[0].argument, "hello");
    EXPECT_EQ(test[0].connector, ";");
}

TEST(Parsing, AndTest) {
    string toTest = "echo hello && mkdir newFolder";

    vector<preConnector> test = parse(toTest);

    ASSERT_EQ(test.size(), 2);
    EXPECT_EQ(test[0].command, "echo");
    EXPECT_EQ(test[0].argument, "hello");
    EXPECT_EQ(test[0].connector, "&&");
    EXPECT_EQ(test[1].command, "mkdir");
    EXPECT_EQ(test[1].argument, "newFolder");
    EXPECT_EQ(test[1].connector, ";");
}

TEST(Parsing, ORAndTest) {
    string toTest = "echo hello || mkdir newFolder && cd newFolder";

    vector<preConnector> test = parse(toTest);

    ASSERT_EQ(test.size(), 3);
    EXPECT_EQ(test[0].command, "echo");
    EXPECT_EQ(test[0].argument, "hello");
    EXPECT_EQ(test[0].connector, "||");
    EXPECT_EQ(test[1].command, "mkdir");
    EXPECT_EQ(test[1].argument, "newFolder");
    EXPECT_EQ(test[1].connector, "&&");
    EXPECT_EQ(test[2].command, "cd");
    EXPECT_EQ(test[2].argument, "newFolder");
    EXPECT_EQ(test[2].connector, ";");
}

TEST(Parsing, QuoteTest){
    string toTest = "echo \"ping && ping\"";

    vector<preConnector> test = parse(toTest);

    EXPECT_EQ(test[0].command, "echo");
    EXPECT_EQ(test[0].argument, "ping && ping");
    EXPECT_EQ(test[0].connector, ";");
}

TEST(testCommand, literal_exists) {
    TestCommand* totest = new TestCommand("test", "-e rshell");
    EXPECT_EQ(1, totest -> execute() -> getResult());

}

TEST(testCommand, literal_dir) {
    TestCommand* totest = new TestCommand("test", "-d rshell");
    EXPECT_EQ(0, totest -> execute() -> getResult());

}

TEST(testCommand, literal_is_regular) {
    TestCommand* totest = new TestCommand("test", "-f rshell");
    EXPECT_EQ(1, totest -> execute() -> getResult());

}

TEST(testCommand, literal_no_flag) {
    TestCommand* totest = new TestCommand("test", "rshell");
    EXPECT_EQ(1, totest -> execute() -> getResult());

}

TEST(testCommand, literal_dir2) {
    TestCommand* totest = new TestCommand("test", "-d src");
    EXPECT_EQ(1, totest -> execute() -> getResult());

}

TEST(testCommand, symb_exists) {
    vector<preConnector> testVec = parse("[-e rshell]");
    HeadConnector* totest = integrate(testVec);
    EXPECT_EQ(1, totest -> execute() -> getResult());

}

TEST(testCommand, symb_dir) {
    vector<preConnector> testVec = parse("[-d rshell]");
    HeadConnector* totest = integrate(testVec);
    EXPECT_EQ(0, totest -> execute() -> getResult());

}

TEST(testCommand, symb_regular_file) {
    vector<preConnector> testVec = parse("[-f rshell]");
    HeadConnector* totest = integrate(testVec);
    EXPECT_EQ(1, totest -> execute() -> getResult());

}

TEST(testCommand, symb_dir2) {
    vector<preConnector> testVec = parse("[-d src]");
    HeadConnector* totest = integrate(testVec);
    EXPECT_EQ(1, totest -> execute() -> getResult());

}

TEST(testCommand, symb_no_flag) {
    vector<preConnector> testVec = parse("[src]");
    HeadConnector* totest = integrate(testVec);
    EXPECT_EQ(1, totest -> execute() -> getResult());

}

TEST(testCommand, symb_space) {
    vector<preConnector> testVec = parse("[ src ]");
    HeadConnector* totest = integrate(testVec);
    EXPECT_EQ(1, totest -> execute() -> getResult());

}


TEST(Replace, Basic) {
    string given = "The red bus";
    pythonicc_replace(given, "red", "blue");
    EXPECT_EQ(given, "The blue bus");
}


// TEST(redir_out, single_bracket_exists) {
//     OutRedir* test = new OutRedir("echo", "hello", "toTest.txt");
//     test -> execute();

//     TestCommand* exists = new TestCommand("test", "-f toTest.txt");

//     EXPECT_EQ(1,exists->execute() -> getResult());

// }

// TEST(redir_out, single_bracket_simple) {
//     OutRedir* test = new OutRedir("echo", "hello", "toTest.txt");
//     test -> execute();

//     ifstream myfile;
//     string curr;
//     myfile.open ("toTest.txt");

//     myfile >> curr;
//     EXPECT_EQ(curr, "hello");
//     myfile.close();

// }

// TEST(redir_out, single_bracket_overwrite) {
//     OutRedir* test = new OutRedir("echo", "The Wall is the best PF album", "toTest.txt");
//     test -> execute();

//     OutRedir* test2 = new OutRedir("echo", "The Division Bell is the best PF album", "toTest.txt");
//     test2 -> execute();

//     ifstream myfile;
//     string curr;
//     myfile.open ("toTest.txt");

//     getline(myfile,curr);
//     EXPECT_EQ(curr, "The Division Bell is the best PF album");
//     myfile.close();

// }

// TEST(redir_out, double_bracket_exists) {
//     DubOutRedir* test = new DubOutRedir("echo", "hello", "toTest2.txt");
//     test -> execute();

//     TestCommand* exists = new TestCommand("test", "-f toTest2.txt");

//     EXPECT_EQ(1,exists->execute() -> getResult());

// }

// TEST(redir_out, double_bracket_simple) {
//     DubOutRedir* test = new DubOutRedir("echo", "goodbye", "toTest2.txt");
//     test -> execute();

//     ifstream myfile;
//     string curr;
//     myfile.open ("toTest2.txt");

//     myfile >> curr;
//     EXPECT_EQ(curr, "hello");
//     myfile >> curr;
//     EXPECT_EQ(curr, "goodbye");
//     myfile.close();

// }

// TEST(redir_out, double_bracket_append) {
//     OutRedir* test = new OutRedir("echo", "Overhead", "toTest.txt");
//     test -> execute();

//     DubOutRedir* test2 = new DubOutRedir("echo", "the albatross", "toTest.txt");
//     test2 -> execute();

//     DubOutRedir* test3 = new DubOutRedir("echo", "hangs motionless", "toTest.txt");
//     test3 -> execute();

//     DubOutRedir* test4 = new DubOutRedir("echo", "upon the air", "toTest.txt");
//     test4 -> execute();


//     ifstream myfile;
//     string curr;
//     myfile.open ("toTest.txt");

//     getline(myfile,curr);
//     EXPECT_EQ(curr, "Overhead");
//     curr.clear();
//     getline(myfile,curr);
//     EXPECT_EQ(curr, "the albatross");
//     curr.clear();
//     getline(myfile,curr);
//     EXPECT_EQ(curr, "hangs motionless");
//     curr.clear();
//     getline(myfile,curr);
//     EXPECT_EQ(curr, "upon the air");

//     myfile.close();

// }

// TEST(redir_in, executes) {
//     OutRedir* helper = new OutRedir("echo", "test", "file1.txt");
//     helper -> execute();
//     InRedir* test = new InRedir("cat", "","file1.txt");

//     EXPECT_EQ(1, test -> execute() -> getResult());

// }

// TEST(redir_in, cat) {

//     OutRedir* test = new OutRedir("echo", "Hello,", "toTest.txt");
//     test -> execute();

//     DubOutRedir* test2 = new DubOutRedir("echo", "this", "toTest.txt");
//     test2 -> execute();

//     DubOutRedir* test3 = new DubOutRedir("echo", "is a", "toTest.txt");
//     test3 -> execute();

//     DubOutRedir* test4 = new DubOutRedir("echo", "test", "toTest.txt");
//     test4 -> execute();

//     int fd = open("cat.txt", O_RDWR | O_CREAT | O_TRUNC,0666);
//     int result;
//     int stdout = dup(1);

//     result = dup2(fd,1);

//     InRedir* newIn = new InRedir("cat", "", "toTest.txt");
//     newIn -> execute();

//     dup2(stdout, 1);
//     close(stdout);
    
//     ifstream myfile;
//     string curr;
//     myfile.open ("cat.txt");

//     getline(myfile,curr);
//     EXPECT_EQ(curr, "Hello,");
//     curr.clear();
//     getline(myfile,curr);
//     EXPECT_EQ(curr, "this");
//     curr.clear();
//     getline(myfile,curr);
//     EXPECT_EQ(curr, "is a");
//     curr.clear();
//     getline(myfile,curr);
//     EXPECT_EQ(curr, "test");

//     myfile.close();
// }

// TEST(redir_in, sort) {

//     OutRedir* test = new OutRedir("echo", "Zebra", "toTest.txt");
//     test -> execute();

//     DubOutRedir* test2 = new DubOutRedir("echo", "Kangaroo", "toTest.txt");
//     test2 -> execute();

//     DubOutRedir* test3 = new DubOutRedir("echo", "Tortoise", "toTest.txt");
//     test3 -> execute();

//     DubOutRedir* test4 = new DubOutRedir("echo", "Antelope", "toTest.txt");
//     test4 -> execute();

//     int fd = open("sorted.txt", O_RDWR | O_CREAT | O_TRUNC,0666);
//     int result;
//     int stdout = dup(1);

//     result = dup2(fd,1);

//     InRedir* newIn = new InRedir("sort", "", "toTest.txt");
//     newIn -> execute();

//     dup2(stdout, 1);
//     close(stdout);
    
//     ifstream myfile;
//     string curr;
//     myfile.open ("sorted.txt");

//     getline(myfile,curr);
//     EXPECT_EQ(curr, "Antelope");
//     curr.clear();
//     getline(myfile,curr);
//     EXPECT_EQ(curr, "Kangaroo");
//     curr.clear();
//     getline(myfile,curr);
//     EXPECT_EQ(curr, "Tortoise");
//     curr.clear();
//     getline(myfile,curr);
//     EXPECT_EQ(curr, "Zebra");

//     myfile.close();
// }

// TEST (redir_in, wc_test) {
//     OutRedir* test = new OutRedir("echo", "Zebra", "toTest.txt");
//     test -> execute();

//     DubOutRedir* test2 = new DubOutRedir("echo", "Kangaroo", "toTest.txt");
//     test2 -> execute();

//     DubOutRedir* test3 = new DubOutRedir("echo", "Tortoise", "toTest.txt");
//     test3 -> execute();

//     DubOutRedir* test4 = new DubOutRedir("echo", "Antelope", "toTest.txt");
//     test4 -> execute();

//     int fd = open("wordcount.txt", O_RDWR | O_CREAT | O_TRUNC,0666);
//     int result;
//     int stdout = dup(1);

//     result = dup2(fd,1);

//     InRedir* newIn = new InRedir("wc", "", "toTest.txt");
//     newIn -> execute();

//     dup2(stdout, 1);
//     close(stdout);
    
//     ifstream myfile;
//     int curr;
//     myfile.open ("wordcount.txt");

//     myfile >> curr;
//     myfile >> curr;

//     EXPECT_EQ(curr, 4); //wc outputs (# of lines, # of words, # of bytes)

//     myfile.close();
// }

// TEST(Integrator, findString){
//     string base = "just another include in the in the codebase";
//     string sub = "in";
//     vector<int> found = findStrings(base, sub);
//     EXPECT_EQ(found[0], 13);
//     EXPECT_EQ(found[1], 21);
//     EXPECT_EQ(found[2], 28);
// }


int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}