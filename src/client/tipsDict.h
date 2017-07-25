#ifndef __TIPS_DICT_H__
#define __TIPS_DICT_H__
#include <map>
#include <string>

class TipsDict
{
public:
    static const int KEY_ELIMINATE_REWARD=1001;
    static const int KEY_REWARD_SCORE=1002;
    static const int KEY_REMAIN_COUNT=1003;
    static const int KEY_LEVEL_CHANGE=1004;
    static const int KEY_PASS_SCORE_CHANGE=1005;
    static const int KEY_PROPERTY_REPLACE=1006;
    static const int KEY_PROPERTY_SHUFFLE=1007;
    static const int KEY_PROPERTY_BOMB=1008;
    static const int KEY_CATCH_FISH=1009;
    static const int KEY_TASK_FAILED=1010;
    static const int KEY_TASK_SUCCESS=1011;
    static const int KEY_NOT_ENGOUH_GOLD=1012;
    static const int KEY_UUCUN_SALE_LABEL=1013;
    static const int KEY_UUCUN_PAYUP_LABEL=1014;
    static const int KEY_UUCUN_PAYDOWN_LABEL=1015;
    static TipsDict* getInstance();
    const std::string& getTips(int key) const;
protected:
    TipsDict(void);
    ~TipsDict(void);
    int loadInfos();
    static TipsDict* m_dict;
    std::map<int, std::string> m_tipsMap;
};
#endif 

