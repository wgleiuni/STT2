#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include "Single.h"
#include "Para.h"
using namespace std;
Single::Single(double alpha,double v,int id) {
    P=Parameter::getInstance();
    _y=P->_y;
    _ty.resize(3);
    _k1.resize(3);
    _k2.resize(3);
    _k3.resize(3);
    _k4.resize(3);
    _alpha=alpha;
    _v=v;
    _t=0;
    if (P->_numdt!=-1) {
        string name="result"+to_string(id)+".txt";
        _ofile.open(name,fstream::out);
    }
    _ofile << alpha << " " << v << endl;
}

double Single_static::getforce() {
    return _v;
}

double Single_pulse::getforce() {
    return _v*pow(fabs(sin(M_PI*_t/P->_v[1])),P->_v[1]/_t);
}

double Single_periodic::getforce() {
    return _v*sin(2*M_PI*P->_v[1]/P->_v[2]*_t);
}

void Single::RK4_onestep() {
    for (int i=0;i<3;i++) _ty[i]=_y[i];
    RK4_df(_t,_k1);
    for (int i=0;i<3;i++) _ty[i]=_y[i]+P->_h*_k1[i]/2.;
    RK4_df(_t+P->_h/2.,_k2);
    for (int i=0;i<3;i++) _ty[i]=_y[i]+P->_h*_k2[i]/2.;
    RK4_df(_t+P->_h/2.,_k3);
    for (int i=0;i<3;i++) _ty[i]=_y[i]+P->_h*_k3[i];
    RK4_df(_t+P->_h,_k4);
    for (int i=0;i<3;i++) _y[i]+=P->_h/6.*(_k1[i]+2.*_k2[i]+2.*_k3[i]+_k4[i]);
    if (P->_numdt!=-1 && _cnt%P->_numdt==0) _ofile << _t << "\t" << _y[0] << "\t" << _y[1] << "\t" << _y[2] << endl;
    return;
}

void Single::RK4_df(double t,vector<double>& k) {
    vector<double> b (3);
    double v=getforce();
    b[0]=(1.0-P->_mu*(_ty[0]*P->_b[0]+_ty[1]*P->_b[1]+_ty[2]*P->_b[2])-P->_nu*v)*P->_b[0];
    b[1]=(1.0-P->_mu*(_ty[0]*P->_b[0]+_ty[1]*P->_b[1]+_ty[2]*P->_b[2])-P->_nu*v)*P->_b[1]+P->_kappa*_ty[1];
    b[2]=(1.0-P->_mu*(_ty[0]*P->_b[0]+_ty[1]*P->_b[1]+_ty[2]*P->_b[2])-P->_nu*v)*P->_b[2];
    double denom=1.+_alpha*_alpha;
    k[0]=-(b[2]*_ty[1]+_alpha*b[1]*_ty[0]*_ty[1]-_alpha*b[0]*_ty[1]*_ty[1]-b[1]*_ty[2]+_alpha*b[2]*_ty[0]*_ty[2]-_alpha*b[0]*_ty[2]*_ty[2])/denom;
    k[1]=-(-b[2]*_ty[0]+_alpha*b[0]*_ty[0]*_ty[1]-_alpha*b[1]*_ty[0]*_ty[0]+b[0]*_ty[2]+_alpha*b[2]*_ty[1]*_ty[2]-_alpha*b[1]*_ty[2]*_ty[2])/denom;
    k[2]=-(b[1]*_ty[0]+_alpha*b[1]*_ty[1]*_ty[2]-_alpha*b[2]*_ty[0]*_ty[0]-b[0]*_ty[1]+_alpha*b[0]*_ty[0]*_ty[2]-_alpha*b[2]*_ty[1]*_ty[1])/denom;
    return;
}

double Single::go() {
    while (_t<P->_totT) {
        RK4_onestep();
        _t+=P->_h;
    }
    return _y[1];
}
