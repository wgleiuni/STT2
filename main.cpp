#include <iostream>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
using namespace std;
class Mag {
    public:
        Mag();
        void Read();
        void go();
    private:
        void RK4_onestep();
        void RK4_df(double,vector<double>&);
        vector<double> _y,_ty,_k1,_k2,_k3,_k4,_b;
        double _alpha,_mu,_kappa,_nu,_v,_f,_gammaB,_omega,_h,_t,_totT;
        fstream ofile;
        int _cnt=0,_numdt;
};

Mag::Mag() {
    _y.resize(3);
    _ty.resize(3);
    _k1.resize(3);
    _k2.resize(3);
    _k3.resize(3);
    _k4.resize(3);
    _b.resize(3);
    Read();
    _omega=2*M_PI*_f/_gammaB;
    cout << "omega = " << _omega << endl;
    ofile.open("result.txt",fstream::out);
    return;
}

void Mag::Read() {
    ifstream infile("data.txt");
    string line;
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_alpha;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_kappa;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_mu;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_nu;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_f;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_gammaB;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_v;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_h;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_totT;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_numdt;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_b[0]>>_b[1]>>_b[2];
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_y[0]>>_y[1]>>_y[2];
    }
    cout << "alpha = " << _alpha << endl;
    cout << "kappa = " << _kappa << endl;
    cout << "mu = " << _mu << endl;
    cout << "nu = " << _nu << endl;
    cout << "f = " << _f << endl;
    cout << "gammaB = " << _gammaB << endl;
    cout << "v = " << _v << endl;
    cout << "dt = " << _h << endl;
    cout << "m0 = " << _y[0] << " " << _y[1] << " " << _y[2] << endl;
    return;
}

void Mag::RK4_onestep() {
    for (int i=0;i<3;i++) _ty[i]=_y[i];
    RK4_df(_t,_k1);
    for (int i=0;i<3;i++) _ty[i]=_y[i]+_h*_k1[i]/2.;
    RK4_df(_t+_h/2.,_k2);
    for (int i=0;i<3;i++) _ty[i]=_y[i]+_h*_k2[i]/2.;
    RK4_df(_t+_h/2.,_k3);
    for (int i=0;i<3;i++) _ty[i]=_y[i]+_h*_k3[i];
    RK4_df(_t+_h,_k4);
    for (int i=0;i<3;i++) _y[i]+=_h/6.*(_k1[i]+2.*_k2[i]+2.*_k3[i]+_k4[i]);
    _t+=_h;
    if (_cnt++%_numdt==0) ofile << _t << "\t" <<  _y[0] << "\t" << _y[1] << "\t" << _y[2] << endl;
    return;
}

void Mag::RK4_df(double t,vector<double>& k) {
    vector<double> b (3);
    double v=_v*sin(_omega*t);
    b[0]=(1.0-_mu*(_ty[0]*_b[0]+_ty[1]*_b[1]+_ty[2]*_b[2])-_nu*v)*_b[0];
    b[1]=(1.0-_mu*(_ty[0]*_b[0]+_ty[1]*_b[1]+_ty[2]*_b[2])-_nu*v)*_b[1]+_kappa*_ty[1];
    b[2]=(1.0-_mu*(_ty[0]*_b[0]+_ty[1]*_b[1]+_ty[2]*_b[2])-_nu*v)*_b[2];
    double denom=1.+_alpha*_alpha;
    k[0]=-(b[2]*_ty[1]+_alpha*b[1]*_ty[0]*_ty[1]-_alpha*b[0]*_ty[1]*_ty[1]-b[1]*_ty[2]+_alpha*b[2]*_ty[0]*_ty[2]-_alpha*b[0]*_ty[2]*_ty[2])/denom;
    k[1]=-(-b[2]*_ty[0]+_alpha*b[0]*_ty[0]*_ty[1]-_alpha*b[1]*_ty[0]*_ty[0]+b[0]*_ty[2]+_alpha*b[2]*_ty[1]*_ty[2]-_alpha*b[1]*_ty[2]*_ty[2])/denom;
    k[2]=-(b[1]*_ty[0]+_alpha*b[1]*_ty[1]*_ty[2]-_alpha*b[2]*_ty[0]*_ty[0]-b[0]*_ty[1]+_alpha*b[0]*_ty[0]*_ty[2]-_alpha*b[2]*_ty[1]*_ty[1])/denom;
    return;
}

void Mag::go() {
    double t=0.;
    while (t<_totT) {
        RK4_onestep();
        t+=_h;
    }
    return;
}

int main(int argc, char *argv[])
{
    Mag obj;
    obj.go();
    return 0;
}
