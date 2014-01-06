//
//  MapManager.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/10/06.
//
//

#include "MapManager.h"
#include <iostream>
#include <string>

void MapManager::init(int top, int bottom, int left, int right)
{
    m_top = top;
    m_bottom = bottom;
    m_left = left;
    m_right = right;
    
    clearCursor();
    clearMapItemArray(&m_mapObjectDataArray);
    clearMapItemArray(&m_mapDropItemDataArray);

    m_mapMovePointList.clear();
}

/**
 * キャラクター移動範囲検索.
 * @param mapIndex 移動するキャラクターの座標
 * @param dist 移動可能な距離
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
    
    // TODO: 1件も取れない時に目的地だけ返してしまう
    
    // 目的地を最終到達点として最後に追加（ただしカーソル情報なので移動の向きをここで指定する）
    MapIndex moveEndMapIndex = *moveFromMapIndex;
    
    MoveDirectionType moveDict = checkMoveDirectionType(moveEndMapIndex, m_mapMovePointList.back());
    moveEndMapIndex.moveDictType = moveDict;
    
    m_mapMovePointList.push_back(moveEndMapIndex);
    
    // movePointListを返却
    return m_mapMovePointList;
}

MoveDirectionType MapManager::checkMoveDirectionType(MapIndex fromMapIndex, MapIndex toMapIndex)
{
    int moveX = fromMapIndex.x - toMapIndex.x;
    int moveY = fromMapIndex.y - toMapIndex.y;
    
    if (moveX == 1 && moveY == 0)
    {
        return MoveDirectionType::MOVE_RIGHT;
    }
    else if (moveX == -1 && moveY == 0)
    {
        return MoveDirectionType::MOVE_LEFT;
    }
    else if (moveX == 0 && moveY == 1)
    {
        return MoveDirectionType::MOVE_UP;
    }
    else if (moveX == 0 && moveY == -1)
    {
        return MoveDirectionType::MOVE_DOWN;
    }
    return MoveDirectionType::MOVE_NONE;
}

/**
 * カーソル情報を元に最短距離のMapItemを返します。
 * createActorFindDist後に呼ばないと何も意味がありません.
 */
MapItem MapManager::searchTargetMapItem(std::list<MapIndex> searchMapIndexList)
{
    MapItem targetMoveDistMapItem = {
        MapDataType::NONE,
        {0, 0, MoveDirectionType::MOVE_NONE},
        0,
        0
    };
    
    for (MapIndex mapIndex : searchMapIndexList)
    {
        auto pMapItem = getMapItem(&mapIndex);
        if (pMapItem->mapDataType == MapDataType::MOVE_DIST)
        {
            if (targetMoveDistMapItem.moveDist < pMapItem->moveDist)
            {
                targetMoveDistMapItem = *pMapItem;
            }
        }
    }
    return targetMoveDistMapItem;
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
    MapIndex mapIndex = {mapPointX, mapPointY, MoveDirectionType::MOVE_NONE};
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
    MapIndex mapIndex = {mapPointX, mapPointY, MoveDirectionType::MOVE_NONE};
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
        mapIndex.moveDictType = MoveDirectionType::MOVE_NONE;
        
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

/**
 * プレイヤー追加
 */
void MapManager::addActor(ActorMapItem* actorMapItem)
{
    m_mapObjectDataArray[actorMapItem->mapIndex.x][actorMapItem->mapIndex.y] = *actorMapItem;
    
    DEBUG_LOG_MAP_ITEM_LAYER();
}

/**
 * プレイヤー移動
 */
void MapManager::moveActor(ActorMapItem* pActorMapItem, MapIndex* pMoveMapIndex)
{
    MapIndex beforeMapIndex = pActorMapItem->mapIndex;
    pActorMapItem->mapIndex = *pMoveMapIndex;
    m_mapObjectDataArray[pMoveMapIndex->x][pMoveMapIndex->y] = *pActorMapItem;
    
    ActorMapItem mapItem;
    mapItem.mapDataType = MapDataType::NONE;
    mapItem.mapIndex = beforeMapIndex;
    mapItem.moveDist = 0;
    m_mapObjectDataArray[beforeMapIndex.x][beforeMapIndex.y] = mapItem;
    
    DEBUG_LOG_MAP_ITEM_LAYER();
}

/**
 * ドロップアイテムの追加
 */
void MapManager::addDropItem(DropMapItem* pDropMapItem)
{
    m_mapDropItemDataArray[pDropMapItem->mapIndex.x][pDropMapItem->mapIndex.y] = *pDropMapItem;
    
    DEBUG_LOG_MAP_ITEM_LAYER();
}

/**
 * 障害物の追加
 */
void MapManager::addObstacle(MapIndex* pMapIndex)
{
    // TODO: とりあえずactorと同じにする。。。大丈夫か？
    ActorMapItem mapItem = createNoneMapItem<ActorMapItem>(pMapIndex->x, pMapIndex->y);
    mapItem.mapDataType = MapDataType::OBSTACLE;
    mapItem.seqNo = 0;
    mapItem.moveDone = false;
    mapItem.attackDone = false;
    
    m_mapObjectDataArray[pMapIndex->x][pMapIndex->y] = mapItem;
}

/**
 * マップアイテムの削除
 */
void MapManager::removeMapItem(MapItem* pRemoveMapItem)
{
    if (m_mapCursorDataArray[pRemoveMapItem->mapIndex.x][pRemoveMapItem->mapIndex.y].mapDataType == pRemoveMapItem->mapDataType)
    {
        MapItem noneMapItem = createNoneMapItem<MapItem>(pRemoveMapItem->mapIndex.x, pRemoveMapItem->mapIndex.y);
        m_mapCursorDataArray[pRemoveMapItem->mapIndex.x][pRemoveMapItem->mapIndex.y] = noneMapItem;
    }
    else if (m_mapObjectDataArray[pRemoveMapItem->mapIndex.x][pRemoveMapItem->mapIndex.y].mapDataType == pRemoveMapItem->mapDataType)
    {
        ActorMapItem noneMapItem = createNoneMapItem<ActorMapItem>(pRemoveMapItem->mapIndex.x, pRemoveMapItem->mapIndex.y);
        noneMapItem.seqNo = 0;
        noneMapItem.moveDone = false;
        noneMapItem.attackDone = false;
        m_mapObjectDataArray[pRemoveMapItem->mapIndex.x][pRemoveMapItem->mapIndex.y] = noneMapItem;
    }
    else if (m_mapDropItemDataArray[pRemoveMapItem->mapIndex.x][pRemoveMapItem->mapIndex.y].mapDataType == pRemoveMapItem->mapDataType)
    {
        DropMapItem noneMapItem = createNoneMapItem<DropMapItem>(pRemoveMapItem->mapIndex.x, pRemoveMapItem->mapIndex.y);
        noneMapItem.seqNo = 0;
        noneMapItem.itemId = 0;
        m_mapDropItemDataArray[pRemoveMapItem->mapIndex.x][pRemoveMapItem->mapIndex.y] = noneMapItem;
    }
    
    DEBUG_LOG_MAP_ITEM_LAYER();
}

/**
 * 指定座標のMapItemを取得します。
 * カーソル、オブジェクト、ドロップアイテムの順番に探します。
 * @param pMapIndex 座標
 */
MapItem* MapManager::getMapItem(MapIndex* pMapIndex)
{
    if (m_mapCursorDataArray[pMapIndex->x][pMapIndex->y].mapDataType == MapDataType::NONE)
    {
        auto pActorMapItem = getActorMapItem(pMapIndex);
        if (pActorMapItem->mapDataType == MapDataType::NONE)
        {
           return &(m_mapDropItemDataArray[pMapIndex->x][pMapIndex->y]);
        }
        return pActorMapItem;
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

DropMapItem* MapManager::getDropMapItem(MapIndex* pMapIndex)
{
    return &(m_mapDropItemDataArray[pMapIndex->x][pMapIndex->y]);
}

/**
 * 移動ルート情報を作成.
 * 移動先のカーソルから移動元のMapItemまでの最短経路を検索する。
 * movePointListに追加していきます.
 * @param moveX 移動先のx座標
 * @param moveY 移動先のy座標
 * @param moveDist 索敵する移動距離
 * @param moveToMapItem 移動元
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
        
        // 上か
        if (moveY > m_top && chkMovePoint(moveX, moveY - 1, moveDist, ignoreDataType))
        {
            findMovePointList(moveX, moveY - 1, moveDist, moveToMapItem);
            MapIndex movePointIndex = {moveX, moveY - 1, MoveDirectionType::MOVE_UP};
            m_mapMovePointList.push_back(movePointIndex);
        }
        // 下か？
        else if (moveY < (m_bottom -1) && chkMovePoint(moveX, moveY + 1, moveDist, ignoreDataType))
        {
            findMovePointList(moveX, moveY + 1, moveDist, moveToMapItem);
            MapIndex movePointIndex = {moveX, moveY + 1, MoveDirectionType::MOVE_DOWN};
            m_mapMovePointList.push_back(movePointIndex);
        }
        // 左か?
        else if (moveX > m_left && chkMovePoint(moveX - 1, moveY, moveDist, ignoreDataType))
        {
            findMovePointList(moveX - 1, moveY, moveDist, moveToMapItem);
            MapIndex movePointIndex = {moveX - 1, moveY, MoveDirectionType::MOVE_RIGHT};
            m_mapMovePointList.push_back(movePointIndex);
        }
        // 右にいけるか?
        else if (moveX < (m_right - 1) && chkMovePoint(moveX + 1, moveY, moveDist, ignoreDataType))
        {
            findMovePointList(moveX + 1, moveY, moveDist, moveToMapItem);
            MapIndex movePointIndex = {moveX + 1, moveY, MoveDirectionType::MOVE_LEFT};
            m_mapMovePointList.push_back(movePointIndex);
        }
    }
}

std::list<ActorMapItem> MapManager::findEnemyMapItem()
{
    std::list<ActorMapItem> enemyMapItem;
    //enemyMapItem.clear();
    
    int xCount = m_mapObjectDataArray.size();
    for (int x = 0; x < xCount; x++)
    {
        int yCount = m_mapObjectDataArray[x].size();
        for (int y = 0; y < yCount; y++)
        {
            if (m_mapObjectDataArray[x][y].mapDataType == MapDataType::ENEMY)
            {
                enemyMapItem.push_back(m_mapObjectDataArray[x][y]);
            }
        }
    }
    return enemyMapItem;
}

#pragma mark
#pragma mark DEBUG関連

void MapManager::DEBUG_LOG_MAP_ITEM_LAYER()
{
    std::string buffer;
	for (int y = m_bottom - 1; y >= 0; y--)
    {
        buffer = "";
		for (int x = 0; x < m_right; x++)
        {
            std::string outPutStr = "-";
            std::string dropItemLayerStr = logOutString(m_mapDropItemDataArray[x][y]);
			std::string objectLayerStr = logOutString(m_mapObjectDataArray[x][y]);
			std::string cursorLayerStr = logOutString(m_mapCursorDataArray[x][y]);
			if (dropItemLayerStr != "")
            {
                outPutStr = dropItemLayerStr;
            }
            else if (objectLayerStr != "")
            {
				outPutStr = objectLayerStr;
			}
            else if (cursorLayerStr != "")
            {
				outPutStr = cursorLayerStr;
            }
            buffer += outPutStr + ".";
		}
        printf("%s\n", buffer.c_str());
	}
}

std::string MapManager::logOutString(MapItem mapItem)
{
	if (mapItem.mapDataType == MapDataType::NONE)
    {
		return "";
	}
    else if (mapItem.mapDataType == MapDataType::ENEMY)
    {
		return ("E");
	}
    else if (mapItem.mapDataType == MapDataType::MAP_ITEM)
    {
		return ("@");
	}
    else if (mapItem.mapDataType == MapDataType::OBSTACLE)
    {
		return ("O");
	}
    else if (mapItem.mapDataType == MapDataType::PLAYER)
    {
		return ("P");
	}
    else
    {
        char buff[100];
        sprintf(buff, "%d", mapItem.moveDist);
        std::string buffAsStdStr = buff;
		return buffAsStdStr;
	}
}
