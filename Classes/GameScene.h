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
	virtual void onExit();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
	void update(float) override;
	cocos2d::EventListenerTouchOneByOne *touchListener;
	cocos2d::EventListenerPhysicsContact *contactListener;
	cocos2d::EventListenerTouchOneByOne *startListener;

private:
	void SetPhysicsWorld(cocos2d::PhysicsWorld *world) {
		sceneWorld = world;
	};

	cocos2d::PhysicsBody *currentVaseBody;
	float visibleSizeWidth;
	float visibleSizeHeight;
	bool gameOverInitiated;
	int totalScore;
	cocos2d::Label *scoreLabel;
	void SpawnCol(float dt);
	void KittyJump(float dt);
	int impulsePower;
	float spawnFrequency = 0.004;
	
	void EndGame(cocos2d::Ref * sender); 
	void ScheduleSpawnBg(float dt);
	void SpawnBg(float dt);
	void BeginGame();
	void initializeManView();
	bool onLabelTouch(cocos2d::Touch *touch, cocos2d::Event *event);
	void RestartGame(cocos2d::Ref *sender);
	void MainMenuPrompt(cocos2d::Ref *sender);
	void CleanupSceneSequence(cocos2d::Ref *sender);
	void GoToMainMenu(cocos2d::Ref *sender);
	bool onContactBegin(cocos2d::PhysicsContact &contact);
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	bool onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event);
	bool onTouchStop(cocos2d::Touch *touch, cocos2d::Event *event);
	bool onTouchCancel(cocos2d::Touch *touch, cocos2d::Event *event);
	cocos2d::Node *movingNode;

	cocos2d::PhysicsWorld *sceneWorld;
	std::deque<cocos2d::Sprite*> columnList;
	std::deque<cocos2d::Sprite*> vaseList;
	cocos2d::Sprite *backgroundSprite1;
	cocos2d::Sprite *backgroundSprite2;
	cocos2d::Sprite *backgroundSprite3;
	int columnsOnScreen;
	Column column;
	Kitty *kitty;
	cocos2d::Sprite *backgroundSprite;

};

#endif // __GAME_SCENE_H__
