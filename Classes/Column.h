#ifndef __COLUMN_H__
#define __COLUMN_H__

#include "cocos2d.h"
#include "Vase.h"

class Column : public cocos2d::Layer
{
public:
	Column();
	void SpawnColumn(cocos2d::Layer *layer);

private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	Vase vase;

};

#endif // __COLUMN_H__
