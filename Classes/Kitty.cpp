#include "Kitty.h"
#include "Definitions.h"

USING_NS_CC;

Kitty::Kitty(cocos2d::Layer *layer) {
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	spritebatch = SpriteBatchNode::create("kitty.png");
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("kitty.plist");

	auto catSprite = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("kitty_1.png"));

	
	Vector<SpriteFrame*> animFrames(2);


	for (int i = 1; i <= 2; i++) {
		char str[100] = { 0 };
		sprintf(str, "kitty_%i.png", i);
		auto spr = Sprite::create(str);
		auto frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	float kittyScale = (visibleSize.height / 3) / catSprite->getContentSize().height;
	spritebatch->addChild(catSprite);
	layer->addChild(spritebatch, 2);
	spritebatch->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 3 - origin.y);
	spritebatch->setScale(kittyScale);
	initialYPosition = spritebatch->getPosition().y;
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
	//animation->setDelayPerUnit(0.3f);
	auto kittyBody = PhysicsBody::createCircle(catSprite->getContentSize().width*kittyScale / 2);
	kittyBody->setCollisionBitmask(KITTY_COLLISION_BITMASK);
	kittyBody->setContactTestBitmask(true);
	kittyBody->setMass(200);
	kittyBody->setDynamic(false);
	kittyBody->setGravityEnable(false);
	catSprite->setPhysicsBody(kittyBody);

	catSprite->setScale(kittyScale);

	catSprite->runAction(RepeatForever::create(Animate::create(animation)));
	
	isInJump = false;
}

void Kitty::Jump() {
	if (true == isInJump) {
		if (spritebatch->getPositionY() >= initialYPosition + visibleSize.height / 3) {
			isInJump = false;
		}
		else {
			spritebatch->setPositionY(spritebatch->getPosition().y + (KITTY_JUMP_SPEED * visibleSize.height));
			isInJump = true;
		}
	}
	else {
		if (spritebatch->getPositionY() <= initialYPosition) {
			isInJump = true;
		}
		else {
			spritebatch->setPositionY(spritebatch->getPosition().y - (KITTY_FALL_SPEED * visibleSize.height));
			isInJump = false;
		}
	}


	//spritebatch->setPosition(spritebatch->getPosition().x, spritebatch->getPosition().y+5);
	//spritebatch->
}