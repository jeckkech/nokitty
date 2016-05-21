#include "Kitty.h"
#include "SimpleAudioEngine.h"
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

	kittyScale = visibleSize.height / 6 / CCSprite::create("kitty_1.png")->getContentSize().height;
	catSprite->setName("KittySpriteNode");
	spritebatch->addChild(catSprite);
	layer->addChild(spritebatch, 2);
	spritebatch->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 3 - origin.y);
	spritebatch->setScale(kittyScale);
	initialYPosition = visibleSize.height / 3 - origin.y;
	animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
	auto kittyBody = PhysicsBody::createCircle(catSprite->getContentSize().width*kittyScale / 2);
	kittyBody->setCollisionBitmask(KITTY_COLLISION_BITMASK);
	kittyBody->setContactTestBitmask(true);
	kittyBody->setMass(200);
	kittyBody->setDynamic(false);
	kittyBody->setGravityEnable(false);
	catSprite->setPhysicsBody(kittyBody);

	//catSprite->setScale(kittyScale);
	//catSprite->setPosition(Point(visibleSize.width / 2 - origin.x, visibleSize.height / 3 - origin.y));
	catSprite->runAction(RepeatForever::create(Animate::create(animation)));
	
	isInJump = false;
	isLanding = false;
}

float Kitty::getKittyWidth() {
	return catSprite->getContentSize().width * kittyScale;
}

float Kitty::getKittyHeight() {
	return catSprite->getContentSize().height * kittyScale;
}

cocos2d::Vec2 Kitty::GetPosition() {
	return spritebatch->getPosition();
}

void Kitty::Animate() {
	if (spritebatch->numberOfRunningActions() < 1) {
		spritebatch->getChildByName("KittySpriteNode")->resume();
	}
}
void Kitty::Jump() {
	if(spritebatch->numberOfRunningActions() < 1){
		spritebatch->getChildByName("KittySpriteNode")->pause();
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/kitty_jump.wav");
		auto kittyJumpAction = MoveTo::create((KITTY_JUMP_SPEED * visibleSize.height), Point(spritebatch->getPositionX(), initialYPosition + visibleSize.height / 3));
		auto kittyLandAction = MoveTo::create((KITTY_JUMP_SPEED * visibleSize.height), Point(spritebatch->getPositionX(), initialYPosition));
		
		spritebatch->runAction(Sequence::create(kittyJumpAction, kittyLandAction, nullptr));
	}
}

void Kitty::Land() {
	if (spritebatch->getPositionY() >= initialYPosition + visibleSize.height / 3) {
		spritebatch->setPositionY(spritebatch->getPosition().y - (KITTY_FALL_SPEED * visibleSize.height));
		isLanding = true;
	} else {
		isLanding = false;
	}
}