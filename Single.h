#ifndef SINGLE_H
#define SINGLE_H
#include "Para.h"
#include <fstream>
using namespace std;
class Single {
    public:
        Single(double,double,int,int);
        double go();
        virtual ~Single(){};
    protected:
        virtual double getforce()=0;
        virtual void set(double,double,int)=0;
        Parameter *P;
        double _alpha,_v,_t;
        vector<double> _y,_ty,_k1,_k2,_k3,_k4,_stable;
        fstream _ofile;
        int _cnt=0,_sstable;
        void RK4_onestep();
        void check_stable();
        void RK4_df(double,vector<double>&);
};

class Single_static : public Single {
    double getforce();
    void set(double,double,int);
    public:
    Single_static(double x,double y,int id,int mode) : Single(x,y,id,mode) {set(x,y,mode);}
};

class Single_pulse : public Single {
    double getforce();
    void set(double,double,int);
    double _tau;
    public:
    Single_pulse(double x,double y,int id,int mode) : Single(x,y,id,mode) {set(x,y,mode);}
};

class Single_periodic : public Single {
    double getforce();
    void set(double,double,int);
    public:
    Single_periodic(double x,double y,int id,int mode) : Single(x,y,id,mode) {set(x,y,mode);}
};
#endif
