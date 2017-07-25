#ifndef __LEVEL_DICT_H__
#define __LEVEL_DICT_H__
#include <map>
#include <string>

class LevelConfig{
public:
    int level;
    int upgradeExp;
    std::string showName;
    int bombCount;
    int rewardGold;
    std::string cannonAdd;
};

class LevelDict
{
public:
    static LevelDict* getInstance();
    const LevelConfig* getLevelConfig(int level) const;
protected:
    LevelDict(void);
    ~LevelDict(void);
    int loadInfos();
    static LevelDict* m_dict;
    std::map<int, LevelConfig*> m_fishesMap;
};
#endif 

