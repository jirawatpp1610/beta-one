//+------------------------------------------------------------------+
//|                                Beta One (bo) Router EA          |
//|                Dual‑Regime + KPI + News + Risk Guard            |
//+------------------------------------------------------------------+
#property copyright "Collaborative Development Project"
#property version   "1.00"
#property strict

#include <Trade\Trade.mqh>
CTrade  trade;                 // MT5 trade wrapper

// ---- DLL imports --------------------------------------------------
#import "trend_core.dll"
   int  TrendSignal();         // 1=long, ‑1=short, 0=noTrade
#import
#import "sideway_core.dll"
   int  SidewaySignal();       // same contract
#import

// ---- Global variables & constants --------------------------------
#define MAGIC_TREND   (10001)
#define MAGIC_SIDEWAY (10002)
#define LOSS_LIMIT    (2)          // loss‑streak guard
input bool  NewsFilter   = true;
input bool  ShowWave     = true;
input bool  ShowFFT      = false;
input double RiskTrend   = 1.0;    // percent
input double RiskSideway = 0.5;    // percent

// KPI struct lives in DLL‑shared memory (simplified proxy)
struct KPI {int exec_ok; int exec_err; int slip_bad; int slip_total;};
KPI kpi;  // updated from DLL (simulated)

// loss streak counter saved via GlobalVariable
int  LossCount(){ return(int)GlobalVariableGet("bo_losscount"); }
void SetLossCount(int v){ GlobalVariableSet("bo_losscount", (double)v); }

// pause timer GV: store until timestamp
bool PauseActive(){ return(TimeCurrent() < GlobalVariableGet("bo_pause")); }

//+------------------------------------------------------------------+
//| Expert initialization                                            |
//+------------------------------------------------------------------+
int OnInit()
{
   Print("[bo] EA initialized, version 1.00");
   return(INIT_SUCCEEDED);
}
//+------------------------------------------------------------------+
//| Manage News + Spread gates (calls into headers via DLL)          |
//+------------------------------------------------------------------+
#include <stdlib.mqh>
#import "kernel32.dll"   // dummy for Sleep in debugging
  void Sleep(int);
#import

bool NewsBlocked();      // declared later via include
bool SpreadAllowed();    // declared later via include
bool LossGuard();        // checks loss streak inside OnTick

//+------------------------------------------------------------------+
//| Main tick function                                               |
//+------------------------------------------------------------------+
void OnTick()
{
   static datetime lastBarTime = 0;
   MqlRates current;
   if(!CopyRates(_Symbol,_Period,0,1,current)) return;
   if(current.time==lastBarTime) return;   // only at bar open
   lastBarTime = current.time;

   if(PauseActive()) { Print("[bo] In PAUSE mode (loss streak)" ); return; }
   if(NewsFilter && NewsBlocked()) { Print("[bo] High impact news — flat"); return; }
   if(!SpreadAllowed()) { Print("[bo] Spread too wide — skip"); return; }

   // 1) Detect regime (inside DLL via functions) – quick proxy
   int trendSig   = TrendSignal();    // returns 1/‑1/0
   int sideSig    = SidewaySignal();  // returns 1/‑1/0

   // 2) Decide which to use according to regime flags (simplified)
   int finalSig = 0; int magic = 0; double riskPrc = 0;
   if(trendSig!=0 && sideSig==0){ finalSig=trendSig;  magic=MAGIC_TREND;   riskPrc = RiskTrend;   }
   else if(sideSig!=0 && trendSig==0){ finalSig=sideSig; magic=MAGIC_SIDEWAY; riskPrc = RiskSideway; }
   else { Print("[bo] Undefined or conflicting regime — no trade"); return; }

   // 3) Risk calc (1 % of equity)
   double lot = NormalizeDouble(AccountInfoDouble(ACCOUNT_BALANCE) * riskPrc / 100.0 / (100.0 /*stop in points*/ * _Point),2);
   if(lot<=0){ Print("[bo] Lot calc zero"); return; }

   // 4) Send order with slippage guard via trade
   trade.SetDeviationInPoints(5); // placeholder, real dev from DLL
   bool sent=false;
   if(finalSig>0)
      sent = trade.Buy(lot,_Symbol,0,0,0,"bo‑long");
   else if(finalSig<0)
      sent = trade.Sell(lot,_Symbol,0,0,0,"bo‑short");

   // 5) Update KPI & loss guard
   if(!sent) Sleep(100);   // wait (debug only)
}
//+------------------------------------------------------------------+