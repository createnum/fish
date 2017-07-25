#ifndef __TESTIN_ADAPTER_H__
#define __TESTIN_ADAPTER_H__
class TestinAdapter{
public:
    static void init(const char *appKey, const char *channelId);
    static void onStart();
    static void onStop();
};
#endif