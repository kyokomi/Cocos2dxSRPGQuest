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
    // マップデータ（カーソル用？）
    std::vector<std::vector<MapItem>> m_mapCursorDataArray;
    std::list<MapIndex> m_mapMoveCursorList;
    
    int m_top;
    int m_bottom;
    int m_left;
    int m_right;
    
public:
    void init(int top, int bottom, int left, int right);
    
    std::list<MapIndex> createActorFindDist(MapIndex mapIndex, int dist);
    void clearCursorMapItemArray();
    void findDist(int x, int y, int dist, bool first);
    bool chkMove(int mapPointX, int mapPointY, int dist);
    void addDistCursor(int mapPointX, int mapPointY, int dist);
};

#endif /* defined(__Cocos2dxSRPGQuest__MapManager__) */
