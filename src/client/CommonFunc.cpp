#include "CommonFunc.h"
#include "myGame.h"
#include "platform/CCFileUtils.h"
#include "support/tinyxml2/tinyxml2.h"
#include "cocos2d.h"
#include <time.h>

using namespace tinyxml2;
CommonFunc::CommonFunc(void)
{

}

CommonFunc::~CommonFunc(void)
{
}

bool CommonFunc::loadInnerDictFile(tinyxml2::XMLDocument &doc, const char * fileName){
    unsigned char *pBytes = NULL;
    unsigned long size = 0;
    pBytes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(fileName,"rb" , &size);
    if (size <= 0 || !pBytes)
    {
        cocos2d::CCLog("load xml file:%s. error size:%d", fileName, size);
        return false;
    }
    XMLError error = doc.Parse(reinterpret_cast<char*>(pBytes), size);
    delete []pBytes;
    pBytes = 0;
    if(XML_NO_ERROR != error)
    {
        cocos2d::CCLog("load xml file:%s. error:%d", fileName, error);
        return false;
    }
    return true;
}

bool CommonFunc::stringEqual(const char* first, const char* second, int cycleCount)
{
    int count = 0;
    if (first == second)
    {
        return true;
    }
    while(*first != '\0' && *second != '\0' && *first == *second && count < cycleCount)//'0' = 0
    {
        ++first;
        ++second;
        ++count;
    }
    if (count == cycleCount || (*first == 0 && *second == 0))
    {
        return true;
    }
    return false;
}
bool CommonFunc::writeToNativePlace(char *pContent, const char * pFileName){
    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath() + pFileName;  
    FILE* file = fopen(path.c_str(), "w");  
    if (file) {  
        fputs(pContent, file);  
        fclose(file);
        return true;
    }  
    return false;  
}

int CommonFunc::getCurrentDay(){
    time_t t = time(NULL);
    int rtn = 0;
    tm* ptm = localtime(&t);
    rtn += (ptm->tm_year+1900) * 10000;
    rtn += (ptm->tm_mon +1) * 100;
    rtn += ptm->tm_mday;
    return rtn;
}