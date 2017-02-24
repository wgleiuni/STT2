#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
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
        iss>>_alpha>>_kappa>>_mu>>_nu>>_o;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        double cur;
        while (iss>>cur) _v.push_back(cur);
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_h>>_totT>>_numdt>>_error;
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_b[0]>>_b[1]>>_b[2];
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_y[0]>>_y[1]>>_y[2];
    }
    cout.precision(6);
    cout << string(35,'-') << "Parameter" << string(35,'-') << endl;
    cout << setw(16) << "\u03B1 = "+to_string(_alpha)+"  ";
    cout << setw(16) << "\u03BA = "+to_string(_kappa)+"  ";
    cout << setw(16) << "\u03BC = "+to_string(_mu)+"  ";
    cout << setw(16) << "\u03BD = "+to_string(_nu)+"  ";
    cout << setw(16) << "v = "+to_string(_v[0])+"  " << endl;
    cout << "Important component: " << string(1,'x'+_o) << endl;
    cout << "Initial conditions: ";
    cout << setw(16) << "m1 = "+to_string(_y[0])+"  ";
    cout << setw(16) << "m2 = "+to_string(_y[1])+"  ";
    cout << setw(16) << "m3 = "+to_string(_y[2])+"  " << endl;
    cout << "External field: ";
    cout << setw(16) << "b1 = "+to_string(_b[0])+"  ";
    cout << setw(16) << "b2 = "+to_string(_b[1])+"  ";
    cout << setw(16) << "b3 = "+to_string(_b[2])+"  " << endl;
    cout << string(34,'-') << "Simulation" << string(35,'-') << endl;
    cout << setw(16) << "dt = "+to_string(_h)+"  ";
    cout << setw(16) << "Total Time = "+to_string(_totT)+"  ";
    cout << setw(16) << "Record Step = "+to_string(_numdt)+"  " << endl;
    return;
}
