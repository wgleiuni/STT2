#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Para.h"
using namespace std;
// Allocating and initializing static data member
Parameter* Parameter::_instance=NULL;

Parameter* Parameter::getInstance() {
    if (!_instance) _instance=new Parameter;
    return _instance;
}

Parameter::Parameter() {
    _y.resize(3);
    _b.resize(3);
    read();
}

void Parameter::read() {
    ifstream infile("data.txt");
    string line;
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_alpha>>_kappa>>_mu>>_nu;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        double cur;
        while (iss>>cur) _v.push_back(cur);
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_h>>_totT>>_numdt;
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
    cout << "v = " << _v[0] << endl;
    cout << "dt = " << _h << endl;
    cout << "m0 = " << _y[0] << " " << _y[1] << " " << _y[2] << endl;
    return;
}
