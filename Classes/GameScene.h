#pragma once
#include "cocos2d.h"
#include "Hose.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;


class GameScene:public Layer
{
public:
	virtual void onEnter();
	static Scene* createScene();
	CREATE_FUNC(GameScene);
    virtual bool init();
    
	virtual void update(float delta);


	void gameStart(Ref *pSender);
	void gameEnd(Ref *pSender);
	void gameOver(float dt);
	
	virtual bool onTouchBegan(Touch* pTouch, Event* pEvent);
	virtual void onTouchEnded(Touch* pTouch, Event* pEvent);
    
    void preloadMusic();
    
    void birdReadyAction();

private:
	Hose* hose;
	Sprite *ground1;
	Sprite *ground2;
	GameState gameState;
	Animate *birdanimate;
    float scale;
	int score;
	int hightestScore;
};