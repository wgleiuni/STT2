#ifndef STT_H
#define STT_H
#include <vector>
#include <fstream>
#include <mutex>
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
        void single_rec(int,int);
        int _mode,_nthread,_cnt,_tot;
        mutex _m;
        fstream _ofile;
};
#endif
