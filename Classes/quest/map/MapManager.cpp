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
    
    m_mapMovePointList.clear();
}

/**
 * キャラクター移動範囲検索.
 */
std::list<MapIndex> MapManager::createActorFindDist(MapIndex mapIndex, int dist)
{
    // カーソル情報を初期化
    clearCursor();
    
    // 検索開始(再帰呼び出し)
    findDist(mapIndex.x, mapIndex.y, dist, true);
    
    // cursorListを作成
    return m_mapMoveCursorList;
}

void MapManager::clearCursor()
{
    clearMapItemArray(&m_mapCursorDataArray);
    m_mapMoveCursorList.clear();
}

/**
 * キャラクター移動範囲検索.
 */
std::list<MapIndex> MapManager::createMovePointList(MapIndex* moveFromMapIndex, MapItem* moveToMapItem)
{
    // 経路情報を初期化
    m_mapMovePointList.clear();
    
    // 移動先を取得
    MapItem* moveFromMapItem = getMapItem(moveFromMapIndex);
    
    // 経路探索(再帰呼び出し)
    findMovePointList(moveFromMapItem->mapIndex.x, moveFromMapItem->mapIndex.y, moveFromMapItem->moveDist, moveToMapItem);
    
    // 目的地を最終到達点として最後に追加
    m_mapMovePointList.push_back(*moveFromMapIndex);
    
    // movePointListを返却
    return m_mapMovePointList;
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

bool MapManager::chkMovePoint(int mapPointX, int mapPointY, int dist, MapDataType ignoreMapDataType)
{
    MapIndex mapIndex = {mapPointX, mapPointY};
    MapItem* mapItem = getMapItem(&mapIndex);
    if (mapItem && mapItem->moveDist == dist && mapItem->mapDataType != ignoreMapDataType)
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
        MapIndex mapIndex;
        mapIndex.x = mapPointX;
        mapIndex.y = mapPointY;
        
		// リストに入れたやつだけあとで描画する
        MapItem cursorItem;
        cursorItem.mapDataType = MapDataType::MOVE_DIST;
        cursorItem.moveDist  = dist;
        cursorItem.attackDist = 0;
        cursorItem.mapIndex = mapIndex;
        m_mapCursorDataArray[mapPointX][mapPointY] = cursorItem;
        
        // 描画用のリスト
        if (mapItem.mapDataType == MapDataType::NONE)
        {
            m_mapMoveCursorList.push_back(mapIndex);
        }
    }
}

void MapManager::addActor(ActorMapItem* actorMapItem)
{
    m_mapObjectDataArray[actorMapItem->mapIndex.x][actorMapItem->mapIndex.y] = *actorMapItem;
}

void MapManager::moveActor(ActorMapItem* pActorMapItem, MapIndex* pMoveMapIndex)
{
    MapIndex beforeMapIndex = pActorMapItem->mapIndex;
    pActorMapItem->mapIndex = *pMoveMapIndex;
    m_mapObjectDataArray[pMoveMapIndex->x][pMoveMapIndex->y] = *pActorMapItem;
    
    ActorMapItem mapItem;
    mapItem.mapDataType = MapDataType::NONE;
    mapItem.mapIndex = beforeMapIndex;
    m_mapObjectDataArray[beforeMapIndex.x][beforeMapIndex.y] = mapItem;
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

ActorMapItem* MapManager::getActorMapItemById(int seqNo)
{
    int xCount = m_mapObjectDataArray.size();
    for (int x = 0; x < xCount; x++)
    {
        int yCount = m_mapObjectDataArray[x].size();
        for (int y = 0; y < yCount; y++)
        {
            if (m_mapObjectDataArray[x][y].mapDataType == MapDataType::PLAYER &&
                m_mapObjectDataArray[x][y].seqNo == seqNo)
            {
                return &(m_mapObjectDataArray[x][y]);
            }
        }
    }

    return NULL;
}

/**
 * 移動ルート情報を作成.
 * 移動先のカーソルから移動元のMapItemまでの最短経路を検索する。
 * movePointListに追加していきます.
 *
 */
void MapManager::findMovePointList(int moveX, int moveY, int moveDist, MapItem* moveToMapItem)
{
    // 自軍キャラでなければ通過不可能とする除外Type
    MapDataType ignoreDataType = MapDataType::NONE;
    if (moveToMapItem->mapDataType == MapDataType::PLAYER)
    {
        ignoreDataType = MapDataType::ENEMY;
    }
    else if (moveToMapItem->mapDataType == MapDataType::ENEMY)
    {
        ignoreDataType = MapDataType::PLAYER;
    }
    else
    {
        // プレイヤーと敵以外は移動しない
        return;
    }
    
    // タップ位置から自キャラがいるところまでの最短ルートを探す
    MapIndex moveMapIndex;
    moveMapIndex.x = moveX;
    moveMapIndex.y = moveY;
    MapItem* mapItem = getMapItem(&moveMapIndex);
    if (mapItem && mapItem != moveToMapItem)
    {
        // タップした位置のdistの次はどこか探す
        moveDist++;
        
        // 下か
        if (moveY > m_top && chkMovePoint(moveX, moveY - 1, moveDist, ignoreDataType)) {
            findMovePointList(moveX, moveY - 1, moveDist, moveToMapItem);
            MapIndex movePointIndex = {moveX, moveY - 1, MoveDirectionType::MOVE_DOWN};
            m_mapMovePointList.push_back(movePointIndex);
        }
        // 上か？
        else if (moveY < (m_bottom -1) && chkMovePoint(moveX, moveY + 1, moveDist, ignoreDataType)) {
            findMovePointList(moveX, moveY + 1, moveDist, moveToMapItem);
            MapIndex movePointIndex = {moveX, moveY + 1, MoveDirectionType::MOVE_UP};
            m_mapMovePointList.push_back(movePointIndex);
        }
        // 右か?
        else if (moveX > m_left && chkMovePoint(moveX - 1, moveY, moveDist, ignoreDataType)) {
            findMovePointList(moveX - 1, moveY, moveDist, moveToMapItem);
            MapIndex movePointIndex = {moveX - 1, moveY, MoveDirectionType::MOVE_RIGHT};
            m_mapMovePointList.push_back(movePointIndex);
        }
        // 左にいけるか?
        else if (moveX < (m_right - 1) && chkMovePoint(moveX + 1, moveY, moveDist, ignoreDataType)) {
            findMovePointList(moveX + 1, moveY, moveDist, moveToMapItem);
            MapIndex movePointIndex = {moveX + 1, moveY, MoveDirectionType::MOVE_LEFT};
            m_mapMovePointList.push_back(movePointIndex);
        }
    }
}

