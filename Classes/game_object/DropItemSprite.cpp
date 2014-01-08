//
//  DropItemSprite.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2014/01/05.
//
//

#include "DropItemSprite.h"

using namespace cocos2d;

DropItemSprite::DropItemSprite()
{
    
}

DropItemSprite::~DropItemSprite()
{
    
}

bool DropItemSprite::initWithDropItemDto(DropItemDto pDropItemDto)
{
    m_dropItemDto = pDropItemDto;
    
//    // ActorのSpriteFrameのplistをキャッシュ
//    String* spriteFramePlistName = String::createWithFormat("actor_%d.plist", m_dropItemDto.playerId);
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(spriteFramePlistName->getCString());
    
    // Spriteを生成
//    String* spriteFrameName = String::createWithFormat("actor_%d_%s_%d.jpg", m_dropItemDto.playerId, "bottom", 1);
//    if ( !Sprite::initWithSpriteFrameName(spriteFrameName->getCString()) )
//    {
//        return false;
//    }
    // TODO: とりあえずSpriteFrameCacheはまだやらない。そんなにアイテム多くないので
    auto spriteFileName = StringUtils::format("item_%d.png", m_dropItemDto.imageResId);
    if (!Sprite::initWithFile(spriteFileName))
    {
        return false;
    }
    
    return true;
}

DropItemSprite* DropItemSprite::createWithDropItemDto(DropItemDto pDropItemDto)
{
    auto *pRet = new DropItemSprite();
    if (pRet && pRet->initWithDropItemDto(pDropItemDto))
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

void DropItemSprite::setDropMapItem(DropMapItem dropMapItem)
{
    m_dropMapItem = dropMapItem;
}

DropMapItem* DropItemSprite::getDropMapItem()
{
    return &m_dropMapItem;
}

DropItemSprite::DropItemDto* DropItemSprite::getDropItemDto()
{
    return &m_dropItemDto;
}
