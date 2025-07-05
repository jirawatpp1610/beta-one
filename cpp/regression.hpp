#pragma once
/*  Simple OLS regression + residual Z‑score for Mean‑Revert module */
#include <vector>
#include <numeric>
#include <cmath>
namespace bo {
struct RegStats{ double slope{0}, intercept{0}, sigma{1}; };
inline RegStats ols(const double *y,int N){
    double sumX=0,sumY=0,sumXY=0,sumXX=0;
    for(int i=0;i<N;i++){ sumX+=i; sumY+=y[i]; sumXY+=i*y[i]; sumXX+=i*i; }
    double beta = (N*sumXY - sumX*sumY)/(N*sumXX - sumX*sumX + 1e-9);
    double alpha= (sumY - beta*sumX)/N;
    double rss=0; for(int i=0;i<N;i++){ double res=y[i]-(alpha+beta*i); rss+=res*res; }
    double sigma = std::sqrt(rss/N);
    return {beta,alpha,sigma>0?sigma:1};
}
inline double z_residual(const double *y,int N){ auto r=ols(y,N); double res=y[0]-(r.intercept+r.slope*0); return res/r.sigma; }
}