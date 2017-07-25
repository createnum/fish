#include "MyUtilsConfig.h"
#include "Java_tools_CommonFunc.h"
#include <android/log.h>

MyUtilsConfig* MyUtilsConfig::m_inst=0;

void MyUtilsConfig::init(){
    m_mainLogPath = getSdcardPathJni("skt.xia/logs/fishMain.log");
    //m_mainLogPath = "mnt/sdcard-ext/skt.log";
    m_timerLogPath = getSdcardPathJni("skt.xia/logs/fishTimerThread.log");
    //m_timerLogPath = "mnt/sdcard-ext/timeskt.log";
}
