#include "../src/results.h"
#include "../src/commands.h"
#include "../src/connectors.h"

#include <string>

#include "gtest/gtest.h"

using namespace std;


class ProbeConnector:public Connector{
    public:
        Result* output = NULL;
        ProbeConnector(){};
        void execute(Result* result){
            if(this -> output){
                delete this -> output;
            }
            this -> output = result;
        }
        int getResult(){
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

TEST(Connectors, ExitConnector){
    Command* passcommand = new SysCommand("echo", "ping");
    TailConnector* probe = new TailConnector();
    Connector* test3 = new PassConnector(probe, passcommand);
    Connector* test2 = new PassConnector(test3, passcommand);
    Connector* test1 = new PassConnector(test2, passcommand);
    Connector* test0 = new PassConnector(test1, passcommand);
    Connector* head = new HeadConnector(test1);

    head -> execute(new ExitResult());

    EXPECT_EQ(probe -> keepRunning(), false);

}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}