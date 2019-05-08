#include "../src/results.h"
#include "../src/commands.h"

#include <string>

#include "gtest/gtest.h"

using namespace std;

TEST(CompileTest, Compiles){
    EXPECT_EQ(0, 0);
}

TEST(Results, BadResult){
    EXPECT_EQ(new Result(false)->getResult(), 0);
}

TEST(Results, GoodResult){
    EXPECT_EQ(new Result(true)->getResult(), 1);
}

TEST(Results, ExitResult){
    EXPECT_EQ(new ExitResult()->getResult(), -1);
}

TEST(SysCommands, Basic){
    Command* test = new SysCommand("echo", "yeet");
    Result* res = test -> execute();
    EXPECT_EQ(res -> getResult(), 1);
}

TEST(SysCommands, ErrorCommand){
    Command* test = new SysCommand("pleaseCrash", "thanks");
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

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}