#ifndef __COLUMN_H__
#define __COLUMN_H__

#include "cocos2d.h"
#include "Vase.h"

class Column
{
public:
	Column();
	void SpawnColumn(cocos2d::Layer *layer, std::deque<cocos2d::Sprite*> *columnList, std::deque<cocos2d::Sprite*> *vaseList, int *columnsOnScreen, bool gameOverInitiated);

private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	Vase vase;
};

#endif // __COLUMN_H__
