#pragma once
/*  KPI monitor for 98 % reliability */
#include <ctime>
namespace bo {
struct KPI{
    int exec_total{0},exec_err{0};
    int slip_total{0},slip_bad{0};
    int logic_total{0},logic_bad{0};
    time_t next_reset{0};
};
inline void reset_kpi(KPI& k){ k=KPI{}; k.next_reset=std::time(nullptr)+604800; }
inline double ratio(int good,int total){ return total? (double)good/total : 1.0; }
}