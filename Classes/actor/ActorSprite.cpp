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
    auto spriteFramePlistName = StringUtils::format("actor_%d.plist", m_actorDto.playerId);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(spriteFramePlistName);
    
    // Spriteを生成
    auto spriteFrameName = StringUtils::format("actor_%d_%s_%d.jpg", m_actorDto.playerId, "bottom", 1);
    if ( !Sprite::initWithSpriteFrameName(spriteFrameName) )
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

void ActorSprite::runMoveAction(MapIndex moveMapIndex)
{
    if (moveMapIndex.x == 0 && moveMapIndex.y == 0)
    {
        CCLOG("移動なし");
        return;
    }
    // キャラの向きを変更
    if (moveMapIndex.moveDictType == MOVE_DOWN)
    {
        this->runBottomAction();
    }
    else if (moveMapIndex.moveDictType == MOVE_LEFT)
    {
        this->runLeftAction();
    }
    else if (moveMapIndex.moveDictType == MOVE_RIGHT)
    {
        this->runRightAction();
    }
    else if (moveMapIndex.moveDictType == MOVE_UP)
    {
        this->runTopAction();
    }
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
    return createActorAnimate(m_actorDto.playerId, "bottom");
}

FiniteTimeAction* ActorSprite::createLeftActorAnimate()
{
    return createActorAnimate(m_actorDto.playerId, "left");
}

FiniteTimeAction* ActorSprite::createRightActorAnimate()
{
    return createActorAnimate(m_actorDto.playerId, "right");
}

FiniteTimeAction* ActorSprite::createTopActorAnimate()
{
    return createActorAnimate(m_actorDto.playerId, "top");
}

FiniteTimeAction* ActorSprite::createActorAnimate(int actorId, std::string frameName)
{
    auto pAnimation = Animation::create();
    auto startSpriteFrameName = StringUtils::format("actor_%d_%s_%d.jpg", actorId, frameName.c_str(), 2);
    auto pStartFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(startSpriteFrameName);
    pAnimation->addSpriteFrame(pStartFrame);
    for (int i = 0; i < 3; i++)
    {
        auto spriteFrameName = StringUtils::format("actor_%d_%s_%d.jpg", actorId, frameName.c_str(), (i + 1));
        auto pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
        pAnimation->addSpriteFrame(pFrame);
    }
    pAnimation->setDelayPerUnit(0.3);
    pAnimation->setLoops(-1);
    
    return Animate::create(pAnimation);
}
