#include "regression.hpp"
#include "spread_gate.hpp"
#include "slippage_mgr.hpp"
#include "kpi_monitor.hpp"
extern "C" __declspec(dllexport) int SidewaySignal(){
    // --- placeholder logic returns 0 (no trade) until filled ---
    return 0;
}