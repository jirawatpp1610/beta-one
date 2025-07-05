#pragma once
/* Dynamic spread acceptance – EMA & Welford variance */
#include <cmath>
namespace bo {
struct SpreadGate{
    double ema{0}; double var{0}; int count{0};
    void update(double s){
        const double k=2.0/101.0;       // EMA‑100 tick
        if(count==0) ema=s; else ema=ema + k*(s-ema);
        // Welford
        count++;
        double diff=s-ema;
        var = var + diff*diff/count;    // crude for demo
    }
    bool acceptable(double s){ return s <= ema + std::sqrt(var); }
};
}