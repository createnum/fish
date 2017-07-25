#ifndef __REWARD_DICT_H__
#define __REWARD_DICT_H__
#include <map>
#include <vector>

class RewardConfig{
public:
    int orderId;
    int waitTime;
    int rewardGold;
};

class RewardDict
{
public:
    static RewardDict* getInstance();
    const RewardConfig* nextRewardConfig(int id) const;
protected:
    RewardDict(void);
    ~RewardDict(void);
    int loadInfos();
    static RewardDict* m_dict;
    std::vector<RewardConfig*> m_rewards;
};
#endif 

