#ifndef __PARSERS_H__
#define __PARSERS_H__

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>


using namespace std;

// ; = 1
// || = 2
// && = 3


class paren{
    public:
        paren* prev = nullptr;
        paren* next = nullptr;
};

struct parenShading{
    int parent;
    int priority;
};

struct preConnector{
    string command = "";
    string argument = "";
    string connector = "";
    parenShading shade;
    paren* parentheses = nullptr;
};

string pythonicc_replace(string & original, const string arg1, const string arg2);
vector <preConnector> parse(string s);

#endif