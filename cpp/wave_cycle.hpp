#pragma once
/*  Wave‑cycle computation: Hilbert ±3 tap, phase, amplitude, velocity
 *  Used by Trend‑module.  Header‑only for simplicity.
 */
#include <array>
#include <cmath>

namespace bo {
struct WaveMetrics{
    double phase60{0}, amp60{0}, vel60{0}, phase140{0};
};

// 7‑tap Hilbert FIR coefficients (Ehlers)
constexpr std::array<double,7> H = {0.0962, 0, 0.5769, 0, -0.5769, 0, -0.0962};

inline double hilbert(const double *price){
    return H[0]*price[0] + H[2]*price[2] + H[4]*price[4] + H[6]*price[6];
}

inline double deg(double rad){ return rad*57.295779513; }

/** Compute metrics for last N=60/140 bars (close price array newest first) */
inline WaveMetrics compute_wave(const double *close60,const double *close140){
    // Hilbert on latest bar (index 0 = newest, 3 = current centre)
    double imag60 = hilbert(&close60[0]);
    double imag140= hilbert(&close140[0]);
    double phase60 = std::atan2(imag60, close60[3]);
    double phase140= std::atan2(imag140,close140[3]);
    // amplitude
    double amp60 = std::hypot(close60[3], imag60);
    // simple phase velocity: diff of phase in deg between bar0 & bar1
    double imagPrev = hilbert(&close60[1]);
    double phasePrev = std::atan2(imagPrev, close60[4]);
    double vel = deg(phase60 - phasePrev);
    if(vel<0) vel = -vel;
    return { deg(phase60), amp60, vel, deg(phase140) };
}
}