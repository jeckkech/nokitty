#ifndef __KITTY_H__
#define __KITTY_H__

#include "cocos2d.h"

class Kitty
{
public:
	Kitty(cocos2d::Layer *layer);
	void Jump();
	bool isInJump;
private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::SpriteBatchNode* spritebatch;
	float initialYPosition;
};

#endif // __KITTY_H__
