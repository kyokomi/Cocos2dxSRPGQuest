//
//  ItemWindowLayer.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2014/01/07.
//
//

#ifndef __Cocos2dxSRPGQuest__ItemWindowLayer__
#define __Cocos2dxSRPGQuest__ItemWindowLayer__

#include "cocos2d.h"
#include "DropItemSprite.h"

class ItemWindowLayer : public cocos2d::LayerColor
{
protected:
    enum tag {
        ItemTableLayerTag  = 10000,
        ItemDetailLayerTag = 20000,
        ItemNameTag        = 21000,
        ItemDetailTag      = 22000,
    };
public:
    ItemWindowLayer();
    
    virtual bool initWithContentSize(cocos2d::Size contentSize);
    static ItemWindowLayer* createWithContentSize(cocos2d::Size contentSize);
    
    DropItemSprite::DropItemDto findItem(int itemListIndex);
    void addItemList(DropItemSprite::DropItemDto* pDropItemDto);
    void reloadItemList();
    
    void setItemDetail(int itemListIndex);
    void setItemDetail(DropItemSprite::DropItemDto* pDropItemDto);
    
    static MenuItemSprite* createMenuItemSprite(Color3B color, const ccMenuCallback& callback);
    
    typedef std::function<void(Object*, DropItemSprite::DropItemDto)> ItemWindowMenuCallback;
    void setItemUseMenuCallback(const ItemWindowMenuCallback& itemUseMenuCallback);
    void setItemDropMenuCallback(const ItemWindowMenuCallback& itemDropMenuCallback);
private:
    int m_showItemDetailIdx;
    std::list<DropItemSprite::DropItemDto> m_itemDtoList;
    ItemWindowMenuCallback m_itemUseMenuCallback;
    ItemWindowMenuCallback m_itemDropMenuCallback;
};

#endif /* defined(__Cocos2dxSRPGQuest__ItemWindowLayer__) */
