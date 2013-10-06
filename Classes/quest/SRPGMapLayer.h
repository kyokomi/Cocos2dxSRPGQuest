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
#include "MapManager.h"

USING_NS_CC;

class SRPGMapLayer : public Layer
{
protected:
    enum kTag {
        kTiledMapTag    = 1,
        kCursorBaseTag  = 9000,
        kActorBaseTag   = 10000,
        kGridLineTag    = 100000,
    };
    enum zIndex {
        zTiledMapIndex   = 1,
        zCursorBaseIndex = 9000,
        zActorBaseIndex  = 10000,
        zGridLineIndex   = 100000,
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
    
    Point moveMapPoint(Point mapPosition, float updateDelta, Point pDelta);
private:

    MapManager m_mapManager;
    
//    // マップデータ（カーソル用？）
//    std::vector<std::vector<MapItem>> m_mapCursorDataArray;
//    std::list<MapIndex> m_mapMoveCursorList;
    
    Size m_baseContentSize;
    Size m_baseMapSize;
    Size m_baseTileSize;
    
    void addMapCursor(MapDataType pMapDataType, std::list<MapIndex> moveMapPointList);
    ActorMapItem* addActor(MapDataType pMapDataType, int pSeqNo, int pMapPointX, int pMapPointY, ActorSprite::ActorDto pActorDto);
    
  
//    std::list<MapIndex> createActorFindDist(MapIndex mapIndex, int dist);
//    void clearCursorMapItemArray();
//    void findDist(int x, int y, int dist, bool first);
//    bool chkMove(int mapPointX, int mapPointY, int dist);
//    void addDistCursor(int mapPointX, int mapPointY, int dist);
};


#endif /* defined(__Cocos2dxSRPGQuest__SRPGMapLayer__) */
