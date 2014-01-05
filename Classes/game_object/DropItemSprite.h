//
//  DropItemSprite.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2014/01/05.
//
//

#ifndef __Cocos2dxSRPGQuest__DropItemSprite__
#define __Cocos2dxSRPGQuest__DropItemSprite__

#include "cocos2d.h"
#include "MapManager.h"

USING_NS_CC;

class DropItemSprite : public cocos2d::Sprite
{
public:
    typedef struct _DropItemDto {
        /** アイテムを一意に識別するID. */
        int itemId;
        
        /** アイテムイメージのID. */
        int imageResId;
        
        /** アイテム名. */
        std::string name;
        
    } DropItemDto;
    
    // コンストラクタ、デストラクタ
    DropItemSprite();
    ~DropItemSprite();
    
    // 初期化とか
    virtual bool initWithDropItemDto(DropItemDto dropItemDto);
    static DropItemSprite* createWithDropItemDto(DropItemDto dropItemDto);
    
    void setDropMapItem(DropMapItem dropMapItem);
    DropMapItem* getDropMapItem();
    DropItemDto* getDropItemDto();
    
private:
    DropItemDto m_dropItemDto;
    DropMapItem m_dropMapItem;
    
};

#endif /* defined(__Cocos2dxSRPGQuest__DropItemSprite__) */
