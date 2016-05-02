#ifndef __VASE_H__
#define __VASE_H__

#include "cocos2d.h"

class Vase
{
public:
	Vase();
	void SpawnVase(cocos2d::Layer *layer, float colHeight, float colWidth, float colY, float colScale, std::deque<cocos2d::Sprite*> *vaseList, bool gameOverInitiated);

private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
};

#endif // __VASE_H__
