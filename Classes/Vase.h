#ifndef __VASE_H__
#define __VASE_H__

#include "cocos2d.h"

class Vase
{
public:
	Vase();
	void SpawnVase(cocos2d::Layer *layer, cocos2d::EventListenerTouchOneByOne *touchListener, float colHeight, float colWidth, float colY, float colScale);

private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
};

#endif // __VASE_H__
