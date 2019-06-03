#ifndef __INTEGRATOR_H__
#define __INTEGRATOR_H__


#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>

#define GetCurrentDir getcwd


 
using namespace std;

class preConnector;

struct parenLocation{
    int first;
    int last;
};


HeadConnector* integrate(vector <preConnector> bigVec);
HeadConnector* superIntegrate(vector <preConnector> bigVec);

#endif