#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <memory>
#include "Single.h"
#include "STT.h"
using namespace std;
STT::STT() {
    P=Parameter::getInstance();
    double begin,end;
    int num;
    ifstream infile("run.txt");
    string line;
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>begin>>end>>num;
        double step=(end-begin)/(num-1);
        for (int i=0;i<num;i++) _Valpha.push_back(begin+i*step);
    }
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>begin>>end>>num;
        double step=(end-begin)/(num-1);
        for (int i=0;i<num;i++) _Vv.push_back(begin+i*step);
    }
    _res=vector<vector<double>> (_Valpha.size(),vector<double> (_Vv.size(),0.0));
}

void STT::run_one(int x,int y) {
    unique_ptr<Single> p;
    int id=x*_Valpha.size()+y;
    if (P->_v.size()==1) {
        p=unique_ptr<Single_static> (new Single_static(_Valpha[x],_Vv[y],id));
    }
    else if (P->_v.size()==2) {
        p=unique_ptr<Single_pulse> (new Single_pulse(_Valpha[x],_Vv[y],id));
    }
    else if (P->_v.size()==3) {
        p=unique_ptr<Single_periodic> (new Single_periodic(_Valpha[x],_Vv[y],id));
    }
    _res[x][y]=p->go();
    return;
}

void STT::run() {
    queue<pair<int,int>> q;
    for (int i=0;i<_Valpha.size();i++)
        for (int j=0;j<_Vv.size();j++)
            q.push({i,j});
    vector<thread> workers (10);
    int tot=q.size();
    while (!q.empty()) {
        for (int i=0;i<10;i++) {
            if (!q.empty()) {
                workers[i]=thread([=]{run_one(q.front().first,q.front().second);});
                q.pop();
            }
            else break;
        }
        for (int i=0;i<10;i++)
            if (workers[i].joinable()) workers[i].join();
        double cur=50*(tot-q.size())/((double)tot);
        string scur="["+string((int)cur,'=')+">"+string(50-(int)cur,' ')+"]"+to_string((int)cur*2);
        cout << scur << "%\r";
        cout.flush();
    }
    record();
    return;
}

void STT::record() {
    fstream ofile;
    ofile.open("result.txt",fstream::out);
    for (int i=0;i<_Valpha.size();i++)
        for (int j=0;j<_Vv.size();j++)
            ofile << _Valpha[i] << "\t" << _Vv[j] << "\t" << _res[i][j] << "\n";
}
