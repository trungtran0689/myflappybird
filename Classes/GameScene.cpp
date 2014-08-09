#include "GameScene.h"
#include <cmath>


Scene* GameScene::createScene()
{
	Scene *gameSceen = Scene::create();
	GameScene *gameLayer = GameScene::create();

	gameSceen->addChild(gameLayer);
	return gameSceen;
}

bool GameScene::init() {
    if(!Layer::init()) {
        return false;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    hightestScore = 0;
	score = 0;
    scale = 2.2;
    
    auto spriteCache = SpriteFrameCache::getInstance();
//	spriteCache->addSpriteFramesWithFile("ui/flappy_frame.plist");
	spriteCache->addSpriteFramesWithFile("ui/flappy_packer.plist");
	
	Size winSize =Director::getInstance()->getWinSize();
    
	Sprite *bg = Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("background.png"));
    Rect bgRect= bg->getTextureRect();
    float scaleX = winSize.width / bgRect.size.width;
    float scaleY = winSize.height / bgRect.size.height;
    
	bg->setAnchorPoint(Vec2(0,0));
	bg->setPosition(Vec2(0,0));
	bg->setScale(scaleX, scaleY);
	this->addChild(bg);

    
	ground1 = Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("ground.png"));
	ground1->setAnchorPoint(Vec2(0,0));
	ground1->setPosition(Vec2(0,0));
	ground1->setZOrder(10);
    Rect groundRect = ground1->getTextureRect();
    ground1->setScaleX(winSize.width / groundRect.size.width);
	this->addChild(ground1);

	ground2 = Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("ground.png"));
	ground2->setAnchorPoint(Vec2(0,0));
	ground2->setPosition(Vec2(winSize.width,0));
    ground2->setScaleX(winSize.width / groundRect.size.width);
	ground2->setZOrder(10);
	this->addChild(ground2);
    
    
    Sprite *flappyBird = Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("logo.png"));
	flappyBird->setPosition(Vec2(winSize.width/2,winSize.height/4*3));
    flappyBird->setScale(scale + .5);
	flappyBird->setTag(TAG_LOGO);
	this->addChild(flappyBird);
    
    
    Sprite *gameOver=Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("gameover.png"));
	gameOver->setPosition(Vec2(winSize.width/2,winSize.height/4*3));
	gameOver->setScale(scale + .5);
	gameOver->setTag(TAG_OVER);
	gameOver->setZOrder(10);
	gameOver->setVisible(false);
	this->addChild(gameOver);
    
    
    Sprite *getReady = Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("ready.png"));
    getReady->setPosition(Vec2(winSize.width/2,winSize.height/4*3));
    getReady->setScale(scale + .5);
    getReady->setVisible(false);
    getReady->setTag(TAG_READY);
    this->addChild(getReady);
    
    Sprite *start = Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("start.png"));
    MenuItemSprite *startBtn = MenuItemSprite::create(start, start, CC_CALLBACK_1(GameScene::gameStart, this));
    startBtn->setPosition(Vec2(winSize.width/4, ground1->getBoundingBox().size.height + 60));
    startBtn->setScale(scale);
    
    
    Sprite *end = Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("grade.png"));
    MenuItemSprite *endBtn = MenuItemSprite::create(end, end, CC_CALLBACK_1(GameScene::gameEnd, this));
    endBtn->setScale(scale);
    endBtn->setPosition(Vec2(winSize.width/4*3, ground1->getBoundingBox().size.height + 60));
    
    auto menu = Menu::create(startBtn, endBtn, NULL);
    menu->setPosition(Vec2(0, 0));
    menu->setTag(TAG_MENU);
    this->addChild(menu);
    
    
    Sprite *hint = Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("hint.png"));
    hint->setPosition(Vec2(winSize.width/2,winSize.height/2));
    hint->setScale(scale);
    hint->setTag(TAG_CLICK);
    hint->setVisible(false);
    this->addChild(hint);
    
    Label *score=Label::createWithBMFont("fonts/font.fnt", "0");
	score->setPosition(Vec2(winSize.width/2,winSize.height/4*3+60));
	score->setTag(TAG_SCORE);
	score->setVisible(false);
	score->setZOrder(10);
	this->addChild(score);
    
	// ÃÌº”º«¬º
    Sprite *record = Sprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->getSpriteFrameByName("scoreboard.png"));
	record->setPosition(Vec2(winSize.width/2,winSize.height/2+30));
	record->setScale(scale);
	record->setTag(TAG_RECORD);
	record->setVisible(false);
	record->setZOrder(10);
	this->addChild(record);
    
 
    hose = Hose::create();
    this->addChild(hose);
    
    this->birdReadyAction();
    
    preloadMusic();
    
    // Loop
    this->scheduleUpdate();
    
    
    gameState = Welcome;
	   
    return true;
}


void GameScene::preloadMusic()
{
    auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
	audioEngine->preloadEffect("sound/lh_die.ogg");
	audioEngine->preloadEffect("sound/lh_hit.ogg");
	audioEngine->preloadEffect("sound/lh_point.ogg");
	audioEngine->preloadEffect("sound/lh_swooshing.ogg");
	audioEngine->preloadEffect("sound/lh_wing.ogg");
}

void GameScene::onEnter()
{
    Layer::onEnter();
}


void GameScene::gameStart( Ref *pSender )
{	
    CCLOG("PLAY");
    if(gameState == GameOver) {
        hose->removeAllChildren();
        hose->onEnter();
        Director::getInstance()->resume();
        this->birdReadyAction();
    }
    gameState = Ready;
    
    this->getChildByTag(TAG_READY)->setVisible(true);
    this->getChildByTag(TAG_CLICK)->setVisible(true);

    this->getChildByTag(TAG_LOGO)->setVisible(false);
    this->getChildByTag(TAG_MENU)->setVisible(false);
}

void GameScene::birdReadyAction() {
    Size winSize = Director::getInstance()->getWinSize();
    Sprite *bird = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird1.png"));
    bird->setPosition(Vec2(winSize.width/2-80, winSize.height/2+20));
    bird->setZOrder(20);
    bird->setScale(scale);
    bird->setTag(TAG_BIRD);
    this->addChild(bird);
    
    Animation *birdAnimation = Animation::create();
    birdAnimation->setDelayPerUnit(0.2);
    birdAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird1.png"));
    birdAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird2.png"));
    birdAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird3.png"));
    
    birdanimate = Animate::create(birdAnimation);
    
    
    ActionInterval *action = MoveBy::create(0.3, Vec2(0, -10));
	ActionInterval *action1 = action->reverse();
	ActionInterval *sequence =Sequence::create(action,action1,NULL);
    
    Spawn *spawn = Spawn::create(birdanimate, sequence, NULL);
    RepeatForever *repeatForever = RepeatForever::create(spawn);
    bird->runAction(repeatForever);
}

void GameScene::birdWelcomeAction() {
    auto object = this->getChildByTag(TAG_BIRD);
    Size winSize = Director::getInstance()->getWinSize();
    Sprite *bird;
    if(!object){
        bird = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird1.png"));
        this->addChild(bird);
    }
    
    bird->setPosition(Vec2(winSize.width/2-80, winSize.height/2+20));
    bird->setZOrder(20);
    bird->setScale(scale);
    bird->setTag(TAG_BIRD);
    
    
    Animation *birdAnimation = Animation::create();
    birdAnimation->setDelayPerUnit(0.2);
    birdAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird1.png"));
    birdAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird2.png"));
    birdAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bird3.png"));
    
    birdanimate = Animate::create(birdAnimation);
    auto down = MoveBy::create(0.3, Vec2(0,-10));
    auto up = MoveBy::create(0.3, Vec2(0,10));
    
    ActionInterval *sequence = Sequence::create(down, up, NULL);
    
    Spawn *spawn = Spawn::create(birdanimate,sequence, NULL);
    RepeatForever *repeatForever = RepeatForever::create(spawn);
    bird->runAction(repeatForever);
}

void GameScene::gameEnd(Ref *pSender)
{

	Director::getInstance()->end();
}

void GameScene::gameOver(float dt)
{

}


void GameScene::update(float dt)
{
    this->updateGround();
    
    if(gameState == Start) {
        this->hose->update();
    }
}

void GameScene::updateGround() {
    float dif = 4;
    ground1->setPositionX(ground1->getPositionX() - dif);
	ground2->setPositionX(ground1->getPositionX() + ground1->getContentSize().width);
	if(ground2->getPositionX() == 0)
	{
		ground1->setPositionX(0);
	}
}

void GameScene::birdRiseAction()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/lh_wing.ogg");
	Size winSize = Director::getInstance()->getWinSize();
	float riseHeight =45;
	float birdX =this->getChildByTag(TAG_BIRD)->getPositionX();
	float birdY= this->getChildByTag(TAG_BIRD)->getPositionY();
	float time =birdY/600;
    
	Repeat *flyAction = Repeat::create(birdanimate,90000);
	if(birdY+riseHeight>winSize.height)
	{
		birdY=winSize.height-riseHeight;
	}
    
	MoveTo *riseAction1 = MoveTo::create(0.2,Vec2(birdX,birdY+riseHeight));
	RotateTo *riseAction2 =CCRotateTo::create(0,-30);
	Spawn *riseAction = Spawn::create(riseAction1,riseAction2,NULL);
    
	MoveTo *fallAction1 = MoveTo::create(time,Vec2(birdX,60));
	Sequence *fallAction2= Sequence::create(DelayTime::create(time/6),CCRotateTo::create(0,30),NULL);
	Spawn *fallAction = Spawn::create(fallAction1,fallAction2,NULL);
    
	this->getChildByTag(TAG_BIRD)->stopAllActions();
	this->getChildByTag(TAG_BIRD)->runAction(Spawn::create(flyAction,Sequence::create(riseAction,CCDelayTime::create(0.05),fallAction,NULL),NULL));
}


bool GameScene::onTouchBegan(Touch* pTouch, Event* pEvent )
{
	return true;
}

void GameScene::onTouchEnded( Touch* pTouch, Event* pEvent )
{
    this->getChildByTag(TAG_READY)->setVisible(false);
	this->getChildByTag(TAG_CLICK)->setVisible(false);
	if (gameState==Ready)
	{
		gameState=Start;
	}
	if (gameState==GameOver)
	{
		return;
	}
    
	this->birdRiseAction();
}

