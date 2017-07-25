#ifndef __TASK_DICT_H__
#define __TASK_DICT_H__
#include <vector>
#include <string>

class TaskInfo{
public:
    int id;
    std::string taskName;
    std::vector<int> fishIds;
    int evenType;
    int taskTime;
    int needCount;
    int rewardGold;
    int rewardExp;
};

class TaskDict
{
public:
    static TaskDict* getInstance();
    const TaskInfo* firstTaskInfo() const;
    const TaskInfo* nextTaskInfo(int id) const;
    const TaskInfo* getTaskInfo(int id) const;
protected:
    TaskDict(void);
    ~TaskDict(void);
    int loadInfos();
    static TaskDict* m_dict;
    std::vector<TaskInfo*> m_taskInfos;
};
#endif 

