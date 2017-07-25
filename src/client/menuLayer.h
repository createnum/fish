#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"

class MenuLayer:public cocos2d::CCLayer
{
public:
    ~MenuLayer(void);
    CREATE_FUNC(MenuLayer);
    virtual bool init();
};
#endif

