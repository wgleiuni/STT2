#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <memory>
#include <mutex>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
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
    if (getline(infile,line)) {
        istringstream iss(line);
        iss>>_mode>>_nthread;
    }
    _res=vector<vector<double>> (_Valpha.size(),vector<double> (_Vv.size(),0.0));
    _cnt=0;
    _tot=_Valpha.size()*_Vv.size();
    _ofile.open("Runtime_result.txt",fstream::out);
}

void STT::run_one(int x,int y) {
    unique_ptr<Single> p;
    int id=x*_Vv.size()+y;
    if (P->_v.size()==1) {
        p=unique_ptr<Single_static> (new Single_static(_Valpha[x],_Vv[y],id,_mode));
    }
    else if (P->_v.size()==2) {
        cout << "pulse" << endl;
        p=unique_ptr<Single_pulse> (new Single_pulse(_Valpha[x],_Vv[y],id,_mode));
    }
    else if (P->_v.size()==3) {
        p=unique_ptr<Single_periodic> (new Single_periodic(_Valpha[x],_Vv[y],id,_mode));
    }
    _res[x][y]=p->go();
//    single_rec(x,y);
    return;
}

void STT::single_rec(int i,int j) {
    _m.lock();
    _ofile << _Valpha[i] << "\t" << _Vv[j] << "\t" << _res[i][j] << "\n";

    double cur=50*(++_cnt)/((double)_tot);
    string scur="["+string((int)cur,'=')+">"+string(50-(int)cur,' ')+"]";
    cout.precision(4);
    cout << scur << 2*cur << "%\r";
    cout.flush();

    _m.unlock();
}

void STT::run() {
    boost::asio::io_service ioService;
    boost::thread_group threadpool;
    boost::asio::io_service::work work(ioService);
    for (int i=0;i<_nthread;i++) {
        threadpool.create_thread(boost::bind(&boost::asio::io_service::run,&ioService));
    }
    for (auto i:_Valpha) {
        for (auto j:_Vv) {
            cout << i << " " << j << endl;
            ioService.post(boost::bind(&STT::run_one,this,i,j));
        }
    }
    ioService.stop();
    threadpool.join_all();
//    ioService.stop();
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
