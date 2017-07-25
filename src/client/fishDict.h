#ifndef __FISH_DICT_H__
#define __FISH_DICT_H__
#include <map>
#include <vector>
#include <string>

class FishConfig{
public:
    int id;
    std::string name;
    int exp;
    int gold;
    std::string png_path;
    std::string plist_path;
    std::string normal_name;
    int normal_count;
    std::string dead_name;
    int dead_count;
    bool isSmall;
    bool isThunder;
    float speed;
    float accelerateSpeed;
    float decelerateSpeed;
    int maxNum;
    int probability;
    int showDeadTips;
    std::string deadSound;
};

class FishDict
{
public:
    static FishDict* getInstance();
    const FishConfig* getFishConfig(int id) const;
    int getRandomFishId() const;
protected:
    FishDict(void);
    ~FishDict(void);
    int loadInfos();
    static FishDict* m_dict;
    std::map<int, FishConfig*> m_fishesMap;
    std::vector<int> m_probabilityFishIds;
};
#endif 

