#ifndef __COLUMN_H__
#define __COLUMN_H__

#include "cocos2d.h"
#include "Vase.h"

class Column
{
public:
	Column();
	void SpawnColumn(cocos2d::Layer *layer, cocos2d::EventListenerTouchOneByOne *touchListener);

private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	Vase vase;
};

#endif // __COLUMN_H__
