#include "../src/results.h"
#include "../src/commands.h"
#include "../src/connectors.h"

#include <string>

#include "gtest/gtest.h"

using namespace std;


class ProbeResult:public Result{
    public:
        int probe_value = 0;
        void set_probe(int probe){
            this -> probe_value = probe;
        }
        ProbeResult(){
            this -> result = false;
        }
        ProbeResult(bool result){
            this -> result = result;
        }
        int probe(){
            return this -> probe_value;
        }
};

class Output{
    public:
        ProbeResult* result;
        Output(ProbeResult* result){this -> result = result;};
};

class ProbeConnector:public Connector{
    public:
        Output* output = NULL;
        ProbeConnector(){};
        void execute(Result* result){
            if(this -> output){
                delete this -> output;
            }
            this -> output = new Output(new ProbeResult(result));
        }
        void execute(ProbeResult* result){
            if(this -> output){
                delete this -> output;
            }
            this -> output = new Output(result);
        }
        int getResult(){
            return this -> output -> result -> getResult();
        }
        int probe(){
            ProbeResult* result = this -> output -> result;
            return result -> probe;
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

    ProbeResult* result = new ProbeResult(1);
    test -> execute(result);

    result -> set_probe(10);

    EXPECT_EQ(probe -> probe(), 1);
    EXPECT_EQ(result -> probe(), 10);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}