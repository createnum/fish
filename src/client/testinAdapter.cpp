#include "testinAdapter.h"
#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "crashhelper/TestinCrashHelper.h"
#endif

void TestinAdapter::init(const char *appKey, const char *channelId){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    TestinCrashHelper::initTestinAgent(appKey, channelId);
#endif
}

void TestinAdapter::onStart(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    TestinCrashHelper::onActivityStart();
#endif
}

void TestinAdapter::onStop(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    TestinCrashHelper::onActivityStop();
#endif
}