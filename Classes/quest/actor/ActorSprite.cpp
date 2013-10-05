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
