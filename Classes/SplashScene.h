#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "cocos2d.h"
#include "ui/UIVideoPlayer.h"

class SplashScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(SplashScene);


private:
	void videoEventCallback(cocos2d::Ref *sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType);
	void GoToMainMenuScene(float dt);
};

#endif // __SPLASH_SCENE_H__
