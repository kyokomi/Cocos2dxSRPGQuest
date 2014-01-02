//
//  ActorSprite.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/10/05.
//
//

#include "ActorSprite.h"

using namespace cocos2d;

ActorSprite::ActorSprite()
{
    
}

ActorSprite::~ActorSprite()
{
    
}

bool ActorSprite::initWithActorDto(ActorDto pActorDto)
{
    m_actorDto = pActorDto;
    
    // ActorのSpriteFrameのplistをキャッシュ
    String* spriteFramePlistName = String::createWithFormat("actor%d.plist", m_actorDto.playerId);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(spriteFramePlistName->getCString());
    
    // Spriteを生成
    String* spriteFrameName = String::createWithFormat("actor_%d_%s_%d.jpg", m_actorDto.playerId, "bottom", 1);
    if ( !Sprite::initWithSpriteFrameName(spriteFrameName->getCString()) )
    {
        return false;
    }
    
    return true;
}

ActorSprite* ActorSprite::createWithActorDto(ActorDto pActorDto)
{
    auto *pRet = new ActorSprite();
    if (pRet && pRet->initWithActorDto(pActorDto))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

void ActorSprite::setActorMapItem(ActorMapItem actorMapItem)
{
    m_actorMapItem = actorMapItem;
}

ActorMapItem* ActorSprite::getActorMapItem()
{
    return &m_actorMapItem;
}

ActorSprite::ActorDto* ActorSprite::getActorDto()
{
    return &m_actorDto;
}

void ActorSprite::runBottomAction()
{
    if (this->isRunning() && this->getActionByTag(1))
    {
        return;
    }
    this->stopAllActions();
    auto action = createBottomActorAnimate();
    action->setTag(1);
    this->runAction(action);
}
void ActorSprite::runLeftAction()
{
    if (this->isRunning() && this->getActionByTag(2))
    {
        return;
    }
    this->stopAllActions();
    auto action = createLeftActorAnimate();
    action->setTag(2);
    this->runAction(action);
}
void ActorSprite::runRightAction()
{
    if (this->isRunning() && this->getActionByTag(3))
    {
        return;
    }
    this->stopAllActions();
    auto action = createRightActorAnimate();
    action->setTag(3);
    this->runAction(action);
}
void ActorSprite::runTopAction()
{
    if (this->isRunning() && this->getActionByTag(4))
    {
        return;
    }
    this->stopAllActions();
    auto action = createTopActorAnimate();
    action->setTag(4);
    this->runAction(action);
}

FiniteTimeAction* ActorSprite::createBottomActorAnimate()
{
    return createActorAnimate("bottom");
}

FiniteTimeAction* ActorSprite::createLeftActorAnimate()
{
    return createActorAnimate("left");
}

FiniteTimeAction* ActorSprite::createRightActorAnimate()
{
    return createActorAnimate("right");
}

FiniteTimeAction* ActorSprite::createTopActorAnimate()
{
    return createActorAnimate("top");
}

FiniteTimeAction* ActorSprite::createActorAnimate(std::string frameName)
{
    cocos2d::Animation* animation = cocos2d::Animation::create();
    String* pszStartSpriteFrameName = String::createWithFormat("actor_4_%s_%d.jpg", frameName.c_str(), 2);
    SpriteFrame *pStartFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pszStartSpriteFrameName->getCString());
    animation->addSpriteFrame(pStartFrame);
    for (int i = 0; i < 3; i++)
    {
        String* pszSpriteFrameName = String::createWithFormat("actor_4_%s_%d.jpg", frameName.c_str(), (i + 1));
        SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pszSpriteFrameName->getCString());
        animation->addSpriteFrame(pFrame);
    }
    animation->setDelayPerUnit(0.5);
    animation->setLoops(-1);
    
    return Animate::create(animation);
}
