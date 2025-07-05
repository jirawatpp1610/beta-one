#pragma once
/*  Simple MT5 calendar wrapper – high‑impact USD/XAU filter */
#include <Trade\SymbolInfo.mqh>
#include <Calendar.mqh>
namespace bo {
inline bool high_impact_now(){
#if defined(MQL5)
    MqlCalendarEvent ev;
    ulong events[32]; uint total=CalendarEventByFilter(events,32,CALENDAR_EVENT_IMPORTANCE_HIGH,TimeCurrent()-7200,TimeCurrent()+7200);
    for(uint i=0;i<total;i++) if(CalendarEventById(events[i],ev)) if(ev.currency==_SYMBOL || ev.currency=="USD") return true;
#endif
    return false;
}
}