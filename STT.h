#ifndef STT_H
#define STT_H
#include <vector>
#include "Para.h"
using namespace std;
class STT {
    public:
        STT();
        void run();
    private:
        Parameter* P;
        vector<double> _Valpha,_Vv;
        vector<vector<double>> _res;
        void run_one(int,int);
        void record();
};
#endif
