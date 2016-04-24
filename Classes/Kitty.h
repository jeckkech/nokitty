#ifndef __KITTY_H__
#define __KITTY_H__

#include "cocos2d.h"

class Kitty
{
public:
	Kitty(cocos2d::Layer *layer);
	void Jump();
	void Land();
	void Animate();
	bool isLanding;
	bool isInJump;
	cocos2d::Vec2 GetPosition();
private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::SpriteBatchNode* spritebatch;
	cocos2d::Sprite* catSprite;
	float initialYPosition;
	cocos2d::Animation* animation;
};

#endif // __KITTY_H__
