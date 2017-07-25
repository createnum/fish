#ifndef __FORMATION_DICT_H__
#define __FORMATION_DICT_H__
#include <map>
#include <vector>
#include <string>
#include "cocos2d.h"

class FormationLine{
public:
    int id;
    int beginArea;
    int type;
    cocos2d::CCPoint bControlPoint;
    cocos2d::CCPoint endPoint;
    cocos2d::CCPoint eControlPoint;
    cocos2d::CCPoint bControlPoint2;
    cocos2d::CCPoint endPoint2;
    cocos2d::CCPoint eControlPoint2;
    void makeRandomBeginPosition(cocos2d::CCPoint &point) const;
};

class FormationFish{
public:
    int fishId;
    int type;
    cocos2d::CCPoint beginPoint;
    const FormationLine* line;
    float delay;
};

class FormationConfig{
public:
    int m_id;
    std::vector<FormationFish*> m_fishes;
};

class FormationDict
{
public:
    static FormationDict* getInstance();
    const FormationLine *randomBigFishLine() const;
    const FormationLine *randomSmallFishLine() const;
    const FormationConfig* randomFormationConfig() const;
    const FormationConfig* randomMiniFormationConfig() const;
    const FormationLine *getLine(int id) const;
protected:
    FormationDict(void);
    ~FormationDict(void);
    int loadInfos();
    void printMap();
    FormationConfig* findFormationConfig(int id) const;
    static FormationDict* m_dict;
    std::map<int, FormationLine*> m_linesMap;
    std::map<int, FormationConfig*> m_formationsMap;
    std::vector<FormationConfig*> m_sceneFormations;
    std::vector<FormationConfig*> m_miniFormations;
    std::vector<FormationLine*> m_bigFishLines;
    std::vector<FormationLine*> m_smallFishLines;
};
#endif 

