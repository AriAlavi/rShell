#include <string>
#include <iostream>
#include <vector>

#include "supVec.h" 

using namespace std;

SupVec::SupVec() {
    this -> asString = "";
}

void SupVec::changeVec(vector<vector<string> > newVec) {
    this -> asVector = newVec;
    // this -> updateString();
}

int SupVec::size() {
    int totSize = 0;
    for (int i = 0; i < this -> asVector.size(); ++i) {
        totSize += asVector[i].size();
    }
    return totSize;
}

int SupVec::outerSize() {
    return this -> asVector.size();
}

string SupVec::at(int index) {
    if (index < -1) {
        throw __throw_out_of_range;
    }
    if (this -> asVector.size() == 0) {
        throw __throw_out_of_range;
    }
    int outer = 0;

    while (index >= this -> asVector[outer].size()) {
        index = index - this -> asVector[outer].size();
        outer++;
    }
    return this -> asVector[outer][index];
}

int SupVec::getOuter_givenInner(int index) {
    if (index < -1) {
        throw __throw_out_of_range;
    }
    if (this -> asVector.size() == 0) {
        throw __throw_out_of_range;
    }
    int outer = 0;

    while (index >= this -> asVector[outer].size()) {
        index = index - this -> asVector[outer].size();
        outer++;
    }
    return outer;
}

vector<string> SupVec::atOuter(int index) {
    return this -> asVector[index];
}