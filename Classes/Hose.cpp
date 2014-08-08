#include "Hose.h"

void Hose::onEnter()
{
    Node::onEnter();
    
	for (int i=0;i<30;i++)
	{
		addHose(i);
	}
}

void Hose::update()
{
	for(auto it = hoseList.begin(); it != hoseList.end(); it++)
	{
		Sprite *hoseSprite =(Sprite*) it.operator*();
		hoseSprite->setPositionX(hoseSprite->getPositionX()-4);
		if (hoseSprite->getPositionX()<-hoseSprite->getContentSize().width*hoseSprite->getScaleX())
		{
            this->removeChild(hoseSprite);
            hoseList.erase(it);
		}
	}
}

// 添加障碍物
void Hose::addHose(int num)
{
	Size winSize = Director::getInstance()->getWinSize();

	float hoseHeight =830;
	float accrossHeight = winSize.height/8+150;
	float maxDownY=winSize.height/2;
	float minDownY =300;
	float downHeight = CCRANDOM_0_1()*(maxDownY-minDownY)+minDownY;

	int hoseX =winSize.width+200*num;

	Sprite *spriteDown = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hosedown.png"));
	// Z轴大的会覆盖Z轴小的
	spriteDown->setZOrder(1);
	spriteDown->setAnchorPoint(Vec2(0,0));
	spriteDown->setPosition(Vec2(hoseX,0));
	spriteDown->setScaleX(0.5);
	spriteDown->setScaleY(downHeight/hoseHeight);
	hoseList.pushBack(spriteDown);
	this->addChild(spriteDown);

	Sprite *spriteUp = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hoseup.png"));
	spriteUp->setZOrder(1);
	spriteUp->setAnchorPoint(Vec2(0,0));
	spriteUp->setPosition(Vec2(hoseX,downHeight+accrossHeight));
	spriteUp->setScaleX(0.5);
	spriteUp->setScaleY(0.4);
	hoseList.pushBack(spriteUp);
	this->addChild(spriteUp);
}