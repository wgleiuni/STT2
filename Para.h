#ifndef PARA_H
#define PARA_H
#include <vector>
using namespace std;
class Parameter {
    public:
        static Parameter* getInstance();
    private:
        // member data
        double _alpha,_mu,_kappa,_nu,_h,_totT;
        vector<double> _y,_b,_v;
        int _numdt;
        static Parameter *_instance;

        // member method
        Parameter();
        void read();
    friend class Single;
    friend class Single_static;
    friend class Single_pulse;
    friend class Single_periodic;
    friend class STT;
};
#endif
