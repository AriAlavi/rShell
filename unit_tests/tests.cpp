#include "../src/results.h"
#include "../src/commands.h"
#include "../src/connectors.h"
#include "../src/parser.h"
#include "../src/supVec.h"


#include <string>
#include <iostream>

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

    Result* result = new Result(false);

    head -> execute(result);

    EXPECT_EQ(probe -> getResult(), 1); 
}

TEST(Parsing, Simple) {
    string toTest = "echo hello;";

    vector<vector<string>> test = parse(toTest);

    EXPECT_EQ(test[0][0], "echo");
    EXPECT_EQ(test[0][1], "hello");
    EXPECT_EQ(test[0][2], ";");
}

TEST(Parsing, Space_Semicolon) {
    string toTest = "echo hello ;";

    vector<vector<string>> test = parse(toTest);

    EXPECT_EQ(test[0][0], "echo");
    EXPECT_EQ(test[0][1], "hello");
    EXPECT_EQ(test[0][2], ";");
}

TEST(Parsing, Two_Commands) {
    string toTest = "echo hello; echo bye;";

    vector<vector<string>> test = parse(toTest);

    ASSERT_EQ(test.size(), 2);
    EXPECT_EQ(test[0][0], "echo");
    EXPECT_EQ(test[0][1], "bye");
    EXPECT_EQ(test[0][2], ";");
    EXPECT_EQ(test[1][0], "echo");
    EXPECT_EQ(test[1][1], "hello");
    EXPECT_EQ(test[1][2], ";");
}

TEST(Parsing, No_semicolon) {
    string toTest = "echo hello";

    vector<vector<string>> test = parse(toTest);

    EXPECT_EQ(test[0][0], "echo");
    EXPECT_EQ(test[0][1], "hello");
    EXPECT_EQ(test[0][2], ";");
}

TEST(Parsing, AndTest) {
    string toTest = "echo hello && mkdir newFolder";

    vector<vector<string>> test = parse(toTest);

    ASSERT_EQ(test.size(), 2);
    EXPECT_EQ(test[0][0], "mkdir");
    EXPECT_EQ(test[0][1], "newFolder");
    EXPECT_EQ(test[0][2], ";");
    EXPECT_EQ(test[1][0], "echo");
    EXPECT_EQ(test[1][1], "hello");
    EXPECT_EQ(test[1][2], "&&");
}

TEST(Parsing, ORAndTest) {
    string toTest = "echo hello || mkdir newFolder && cd newFolder";

    vector<vector<string>> test = parse(toTest);

    ASSERT_EQ(test.size(), 3);
    EXPECT_EQ(test[0][0], "cd");
    EXPECT_EQ(test[0][1], "newFolder");
    EXPECT_EQ(test[0][2], ";");
    EXPECT_EQ(test[1][0], "mkdir");
    EXPECT_EQ(test[1][1], "newFolder");
    EXPECT_EQ(test[1][2], "&&");
    EXPECT_EQ(test[2][0], "echo");
    EXPECT_EQ(test[2][1], "hello");
    EXPECT_EQ(test[2][2], "||");
}

TEST(Parsing, QuoteTest){
    string toTest = "echo \"ping && ping\"";

    vector<vector<string>> test = parse(toTest);

    EXPECT_EQ(test[0][0], "echo");
    EXPECT_EQ(test[0][1], "ping && ping");
    EXPECT_EQ(test[0][2], ";");
}

TEST(Replace, Basic) {
    string given = "yeet the meat";
    pythonicc_replace(given, "yeet", "eat");
    EXPECT_EQ(given, "eat the meat");
}

TEST(Paren, Basic){
    Command* A = new SysCommand("echo", "A");
    Command* B = new SysCommand("echo", "B");
    Command* C = new SysCommand("echo", "C");
    Command* D = new SysCommand("echo", "D");

    ProbeConnector* LEFTPROBE = new ProbeConnector();
    ProbeConnector* RIGHTPROBE = new ProbeConnector();
    ProbeConnector* ABSOLUTETAIL = new ProbeConnector();
   

    Connector* LeftB = new PassConnector(LEFTPROBE, B);
    Connector* LeftA= new PassConnector(LeftB, A);
    Connector* LeftHead = new HeadConnector(LeftA);

    Command* LeftParen = new ParenCommand(LeftHead);

    Connector* RightC = new PassConnector(RIGHTPROBE, C);
    Connector* RightD = new PassConnector(RIGHTPROBE, D);
    Connector* RightHead = new HeadConnector(RightC);

    Command* RightParen = new ParenCommand(RightHead);
    
    Connector* RightRunner = new FailConnector(ABSOLUTETAIL, RightParen);
    Connector* LeftRunner = new AnyConnector(RightRunner, LeftParen);
    Connector* head = new HeadConnector(LeftRunner);


    head -> execute(new AbsoluteTrue());

    EXPECT_EQ(ABSOLUTETAIL -> getResult(), 1); 
    EXPECT_EQ(LEFTPROBE -> getResult(), 1); 
    EXPECT_EQ(RIGHTPROBE -> getResult(), -2); 
}


// TEST(SupVec, AtTest){
//     vector<string> smallVec1 = {"echo","hello","&&"};
//     vector<string> smallVec2 = {"echo","goodbye",";"};
//     vector<vector<string> > mainVec = {smallVec1, smallVec2};

//     SupVec testVec = SupVec();
//     testVec.changeVec(mainVec);

//     EXPECT_EQ(testVec.size(), 6);
//     EXPECT_EQ(testVec.getOuter_givenInner(4), 1);
//     EXPECT_EQ(testVec.at(4), "goodbye");

// }

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}