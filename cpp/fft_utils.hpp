#pragma once
/*  Radix‑2 FFT (256‑pt) – header‑only minimal implementation
 *  Used only to locate peak amplitude period for coherence filter.
 */
#include <complex>
#include <vector>
#include <cmath>
namespace bo {
using cd = std::complex<double>;

inline void fft(std::vector<cd>& a){
    const size_t n=a.size();
    if(n<=1) return;
    // bit‑reverse
    for(size_t i=1,j=0;i<n;i++){
        size_t bit=n>>1;
        for(;j&bit;bit>>=1) j^=bit;
        j^=bit;
        if(i<j) std::swap(a[i],a[j]);
    }
    for(size_t len=2;len<=n;len<<=1){
        double ang=-2*M_PI/len;
        cd wlen{cos(ang), sin(ang)};
        for(size_t i=0;i<n;i+=len){
            cd w{1};
            for(size_t j=0;j<len/2;j++){
                cd u=a[i+j];
                cd v=a[i+j+len/2]*w;
                a[i+j]         = u+v;
                a[i+j+len/2]   = u-v;
                w*=wlen;
            }
        }
    }
}
/** return dominant period in bars (256/peakBin)  ignoring DC */
inline int dominant_period_256(const double *price){
    std::vector<cd> a(256);
    for(size_t i=0;i<256;i++) a[i] = cd(price[i],0);
    fft(a);
    size_t kmax=1; double ampMax=0;
    for(size_t k=1;k<128;k++){ double amp=std::abs(a[k]); if(amp>ampMax){ampMax=amp; kmax=k;} }
    return int(256.0/double(kmax));
}
}