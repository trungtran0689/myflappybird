#pragma once
#include "cocos2d.h"
#include "Resource.h"
USING_NS_CC;

class Hose:public Node
{
public:
	CREATE_FUNC(Hose);
	virtual void onEnter();
	void update();
	void addHose(int num);
    void clearHoses();
    
private:
    Vector<Sprite *> hoseList;
};