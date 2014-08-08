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
    flappyBird->setScale(scale);
	flappyBird->setTag(TAG_LOGO);
	this->addChild(flappyBird);
    
    
    Sprite *gameOver=Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("gameover.png"));
	gameOver->setPosition(Vec2(winSize.width/2,winSize.height/4*3));
	gameOver->setScale(scale);
	gameOver->setTag(TAG_OVER);
	gameOver->setZOrder(10);
	gameOver->setVisible(false);
	this->addChild(gameOver);
    
    
    Sprite *getReady = Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("ready.png"));
    getReady->setPosition(Vec2(winSize.width/2,winSize.height/4*3-60));
    getReady->setScale(scale);
    getReady->setVisible(false);
    getReady->setTag(TAG_READY);
    this->addChild(getReady);
    
    Sprite *start = Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("start.png"));
    start->setScale(scale);
    
    MenuItemSprite *startBtn = MenuItemSprite::create(start, start, CC_CALLBACK_1(GameScene::gameStart, this));
    
    Menu *menuStart = Menu::create(startBtn, NULL);
    menuStart->setPosition(Vec2(winSize.width/5, ground1->getBoundingBox().size.height/2 + 60));
    menuStart->setTag(TAG_START_BTN);
    menuStart->setZOrder(10);
    this->addChild(menuStart);
    
    
    Sprite *end = Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("grade.png"));
    end->setScale(scale);
    
    MenuItemSprite *endBtn = MenuItemSprite::create(end, end, CC_CALLBACK_1(GameScene::gameEnd, this));
    
    Menu *menuEnd = Menu::create(endBtn, NULL);
    menuEnd->setPosition(Vec2(winSize.width/5*3, ground1->getBoundingBox().size.height/2 + 60));
    menuEnd->setTag(TAG_END_BTN);
    menuEnd->setZOrder(10);
    this->addChild(menuEnd);
    
    
    Sprite *hint = Sprite::createWithSpriteFrame(spriteCache->getSpriteFrameByName("hint.png"));
    hint->setPosition(Vec2(winSize.width/2,winSize.height/2));
    hint->setScale(scale);
    hint->setTag(TAG_CLICK);
    this->addChild(hint);
    
    Label *score=Label::createWithBMFont("fonts/font.fnt", "0");
	score->setPosition(Vec2(winSize.width/2,winSize.height/4*3+60));
	score->setTag(TAG_SCORE);
//	score->setVisible(false);
	score->setZOrder(10);
	this->addChild(score);
    
	// ÃÌº”º«¬º
    Sprite *record = Sprite::createWithSpriteFrame(CCSpriteFrameCache::getInstance()->getSpriteFrameByName("scoreboard.png"));
	record->setPosition(Vec2(winSize.width/2,winSize.height/2+30));
	record->setScale(scale);
	record->setTag(TAG_RECORD);
//	record->setVisible(false);
	record->setZOrder(10);
	this->addChild(record);
    
//    
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
    if(gameState == GameOver) {
        hose->removeAllChildren();
        hose->onEnter();
        Director::getInstance()->resume();
        this->birdReadyAction();
    }
    gameState = Ready;
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sound/lh_swooshing.ogg");
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
    
//    ActionInterval *sequence = Sequence::create(<#cocos2d::FiniteTimeAction *action1, ...#>, NULL)
    
    Spawn *spawn = Spawn::create(birdanimate, NULL);
    RepeatForever *repeatForever = RepeatForever::create(spawn);
    bird->runAction(repeatForever);
}

void  GameScene::gameEnd(Ref *pSender)
{

	Director::getInstance()->end();
}

void GameScene::gameOver(float dt)
{

}


void GameScene::update(float dt)
{
    float dif = 4;
    ground1->setPositionX(ground1->getPositionX()-dif);
	ground2->setPositionX(ground1->getPositionX()+ground1->getContentSize().width-4);
	if(ground2->getPositionX()==0)
	{
		ground1->setPositionX(0);
	}
}

bool GameScene::onTouchBegan(Touch* pTouch, Event* pEvent )
{
	return true;
}

void GameScene::onTouchEnded( Touch* pTouch, Event* pEvent )
{

}

