//
//  SRPGMapLayer.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/10/05.
//
//

#ifndef __Cocos2dxSRPGQuest__SRPGMapLayer__
#define __Cocos2dxSRPGQuest__SRPGMapLayer__

#include "cocos2d.h"
#include "SRPGMapMacro.h"
#include "ActorSprite.h"

USING_NS_CC;

class SRPGMapLayer : public Layer
{
protected:
    enum kTag {
        kTiledMapTag = 1,
        kGridLineTag = 2,
        kActorBaseTag = 10000,
    };
    enum zIndex {
        zTiledMapIndex = 1,
        zGridLineIndex = 2,
        zActorBaseIndex = 10000,
    };
public:
    SRPGMapLayer();
    ~SRPGMapLayer();
    
    virtual bool init();
    
    CREATE_FUNC(SRPGMapLayer);
    
    void showGrid();
    void hideGrid();
    
    Point indexToPoint(int mapIndex_x, int mapIndex_y);
    Point indexToPoint(MapIndex mapIndex);
    MapIndex pointToIndex(Point point);
    
private:
    Size m_baseContentSize;
    Size m_baseMapSize;
    Size m_baseTileSize;
    
    void addActor(MapDataType pMapDataType, int pSeqNo, int pMapPointX, int pMapPointY, ActorSprite::ActorDto pActorDto);
};


#endif /* defined(__Cocos2dxSRPGQuest__SRPGMapLayer__) */
