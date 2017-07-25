#ifndef __COMMON_FUCTION_H__
#define __COMMON_FUCTION_H__
#include "support/tinyxml2/tinyxml2.h"
#include <vector>

class CommonFunc
{
public:
    CommonFunc(void);
    ~CommonFunc(void);
    static bool loadInnerDictFile(tinyxml2::XMLDocument &doc, const char * fileName);
    static bool stringEqual(const char* first, const char* second, int cycleCount = INT_MAX);
    static bool writeToNativePlace(char *pContent, const char * pFileName);
    static int getCurrentDay();
};


#endif //__COMMON_FUCTION_H__