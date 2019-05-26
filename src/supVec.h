#ifndef __SUPVEC_H__
#define __SUPVEC_H_


class SupVec{
    private:
        string asString;
        vector<vector<string> > asVector;
    public:
        SupVec();
        void changeVec(vector<vector<string> >); //Change Vector to what is passed in
        void updateString(); //Updates the asString format
        int size(); //Get size of all inner vectors combined
        int outerSize();//Get size of outer vector
        string at(int); //Treats vector as single vector and get what is at this index
        int getOuter_givenInner(int); //Given inner vector, get outer vector
        vector<string> atOuter(int); //Get vector at outer



};

#endif