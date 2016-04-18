#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Column.h"
#include "Kitty.h"

class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
	void update(float) override;
	cocos2d::EventListenerTouchOneByOne *touchListener;

private:
	void SetPhysicsWorld(cocos2d::PhysicsWorld *world) {
		sceneWorld = world;
	};

	cocos2d::PhysicsBody *currentVaseBody;
	float visibleSizeWidth;
	float visibleSizeHeight;
	int *totalScore;
	cocos2d::Label *scoreLabel;
	void SpawnCol(float dt);
	void KittyJump(float dt);
	void ScheduleSpawnBg(float dt);
	void SpawnBg(float dt);
	bool onContactBegin(cocos2d::PhysicsContact &contact);
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	bool onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event);
	bool onTouchStop(cocos2d::Touch *touch, cocos2d::Event *event);
	bool onTouchCancel(cocos2d::Touch *touch, cocos2d::Event *event);
	cocos2d::Node *movingNode;

	cocos2d::PhysicsWorld *sceneWorld;
	std::deque<cocos2d::Sprite*> columnList;
	std::deque<cocos2d::Sprite*> vaseList;
	int columnsOnScreen;
	Column column;
	Kitty *kitty;
	cocos2d::Sprite *backgroundSprite;
};

#endif // __GAME_SCENE_H__
