//
//  MapManager.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/10/06.
//
//

#ifndef __Cocos2dxSRPGQuest__MapManager__
#define __Cocos2dxSRPGQuest__MapManager__

#include <iostream>
#include <vector>
#include <list>

#include "SRPGMapMacro.h"

class MapManager
{
private:
    // マップカーソル一時データ
    std::vector<std::vector<MapItem>> m_mapCursorDataArray;
    // マップオブジェクトデータ（キャラ、障害物）
    std::vector<std::vector<ActorMapItem>> m_mapObjectDataArray;
    // マップ移動カーソルリスト
    std::list<MapIndex> m_mapMoveCursorList;
    // マップ移動経路リスト
    std::list<MapIndex> m_mapMovePointList;
    
    int m_top;
    int m_bottom;
    int m_left;
    int m_right;
    
    //void clearMapItemArray(std::vector<std::vector<MapItem>> *pMapItemArray);
    //void clearActorMapItemArray(std::vector<std::vector<ActorMapItem>> *pActorMapItemArray);
  
    template <typename TYPE>
    void clearMapItemArray(std::vector<std::vector<TYPE>> *pMapItemArray)
    {
        pMapItemArray->clear();
        for (int x = 0; x < m_right; x++)
        {
            std::vector<TYPE> mapItemArray;
            for (int y = 0; y < m_bottom; y++)
            {
                TYPE mapItem;
                mapItem.mapDataType = MapDataType::NONE;
                mapItemArray.push_back(mapItem);
            }
            pMapItemArray->push_back(mapItemArray);
        }
    }
    void findDist(int x, int y, int dist, bool first);
    void findMovePointList(int moveX, int moveY, int moveDist, MapItem* moveToMapItem);
    bool chkMove(int mapPointX, int mapPointY, int dist);
    bool chkMovePoint(int mapPointX, int mapPointY, int dist, MapDataType ignoreMapDataType);
    void addDistCursor(int mapPointX, int mapPointY, int dist);
    
public:
    void init(int top, int bottom, int left, int right);
    
    std::list<MapIndex> createActorFindDist(MapIndex mapIndex, int dist);
//    std::list<MapIndex> createMovePointList(MapIndex mapIndex, int dist, MapItem* moveToMapItem);
    std::list<MapIndex> createMovePointList(MapIndex* moveFromMapIndex, MapItem* moveToMapItem);
    
    void addActor(ActorMapItem* pActorMapItem);
    
    ActorMapItem* getActorMapItem(MapIndex* pMapIndex);
    MapItem* getMapItem(MapIndex* pMapIndex);
    ActorMapItem* getActorMapItemById(int seqNo);

//    void createMovePointList(int moveX, int moveY, int moveDist, MapItem* moveToMapItem);
//    void createMovePointList(MapItem* moveToMapItem, MapIndex* moveFromMapIndex);
};

#endif /* defined(__Cocos2dxSRPGQuest__MapManager__) */
