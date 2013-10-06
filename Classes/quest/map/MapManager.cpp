//
//  MapManager.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/10/06.
//
//

#include "MapManager.h"

void MapManager::init(int top, int bottom, int left, int right)
{
    m_top = top;
    m_bottom = bottom;
    m_left = left;
    m_right = right;
    
    clearMapItemArray(&m_mapObjectDataArray);
}

/**
 * キャラクター移動範囲検索.
 */
std::list<MapIndex> MapManager::createActorFindDist(MapIndex mapIndex, int dist)
{
    // カーソル情報を初期化
    clearMapItemArray(&m_mapCursorDataArray);
    
    // 検索開始(再帰呼び出し)
    findDist(mapIndex.x, mapIndex.y, dist, true);
    
    // cursorListを作成
    return m_mapMoveCursorList;
}

/**
 * 移動範囲検索.
 */
void MapManager::findDist(int x, int y, int dist, bool first)
{
    // 初期位置は移動対象外とする制御
    if (!first)
    {
        // 移動可能範囲に追加
        addDistCursor(x, y, dist);
    }
    if (dist == 0)
    {
        return;
    }
    
    // 上にいけるか?
    if (y > m_top && chkMove(x, y - 1, dist))
    {
        findDist(x, y - 1, dist - 1, false);
    }
    // 下にいけるか?
    if (y < (m_bottom - 1) && chkMove(x, y + 1, dist))
    {
        findDist(x, y + 1, dist - 1, false);
    }
    // 左にいけるか?
    if (x > m_left && chkMove(x - 1, y, dist))
    {
        findDist(x - 1, y, dist - 1, false);
    }
    // 右にいけるか?
    if (x < (m_right - 1) && chkMove(x + 1, y, dist))
    {
        findDist(x + 1, y, dist - 1, false);
    }
}

bool MapManager::chkMove(int mapPointX, int mapPointY, int dist)
{
    MapIndex mapIndex = {mapPointX, mapPointY};
    MapItem* mapItem = getMapItem(&mapIndex);
    if (mapItem->mapDataType == MapDataType::NONE ||
        (mapItem->mapDataType == MapDataType::MOVE_DIST && mapItem->moveDist < dist))
    {
        return true;
    }
    return false;
}

/**
 * 移動カーソル追加.
 */
void MapManager::addDistCursor(int mapPointX, int mapPointY, int dist)
{
    // 未設定 or 移動オブジェクトで移動力が上の場合
    MapItem mapItem = m_mapCursorDataArray[mapPointX][mapPointY];
    if (mapItem.mapDataType == MapDataType::NONE ||
        (mapItem.mapDataType == MapDataType::MOVE_DIST && mapItem.moveDist < dist))
    {        
		// リストに入れたやつだけあとで描画する
        MapItem cursorItem;
        cursorItem.mapDataType = MapDataType::MOVE_DIST;
        cursorItem.mapPointX = mapPointX;
        cursorItem.mapPointY = mapPointY;
        cursorItem.moveDist  = dist;
        cursorItem.attackDist = 0;
        m_mapCursorDataArray[mapPointX][mapPointY] = cursorItem;
        
        // 描画用のリスト
        if (mapItem.mapDataType == MapDataType::NONE)
        {
            MapIndex mapIndex;
            mapIndex.x = mapPointX;
            mapIndex.y = mapPointY;
            m_mapMoveCursorList.push_back(mapIndex);
        }
    }
}

void MapManager::addActor(ActorMapItem* actorMapItem)
{
    m_mapObjectDataArray[actorMapItem->mapIndex.x][actorMapItem->mapIndex.y] = *actorMapItem;
}

MapItem* MapManager::getMapItem(MapIndex* pMapIndex)
{
    if (m_mapCursorDataArray[pMapIndex->x][pMapIndex->y].mapDataType == MapDataType::NONE)
    {
        return getActorMapItem(pMapIndex);
    }
    return &(m_mapCursorDataArray[pMapIndex->x][pMapIndex->y]);
}

ActorMapItem* MapManager::getActorMapItem(MapIndex* pMapIndex)
{
    return &(m_mapObjectDataArray[pMapIndex->x][pMapIndex->y]);
}

//void MapManager::clearMapItemArray(std::vector<std::vector<MapItem>> *pMapItemArray)
//{
//    m_mapCursorDataArray.clear();
//    pMapItemArray->clear();
//    
//    for (int x = 0; x < m_right; x++)
//    {
//        std::vector<MapItem> mapItemArray;
//        
//        for (int y = 0; y < m_bottom; y++)
//        {
//            MapItem mapItem;
//            mapItem.mapDataType = MapDataType::NONE;
//            mapItemArray.push_back(mapItem);
//        }
//        pMapItemArray->push_back(mapItemArray);
//    }
//}


