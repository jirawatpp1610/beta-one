#pragma once
#include <deque>
#include <algorithm>
namespace bo {
struct SlippageMgr{
    std::deque<double> hist;
    size_t maxLen{20};
    void push(double v){ hist.push_back(v); if(hist.size()>maxLen) hist.pop_front(); }
    double devMax() const{
        if(hist.empty()) return 3; // default 3 point
        std::deque<double> sorted(hist); std::sort(sorted.begin(),sorted.end());
        return sorted[sorted.size()/2]+1; // median+1
    }
};
}