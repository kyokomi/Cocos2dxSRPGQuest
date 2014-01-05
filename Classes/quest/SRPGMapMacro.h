//
//  SRPGMapMacro.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/10/05.
//
//

#ifndef Cocos2dxSRPGQuest_SRPGMapMacro_h
#define Cocos2dxSRPGQuest_SRPGMapMacro_h

//enum MapDataType {
//    NONE        = 0,
//    MOVE_DIST   = 1,
//    ATTACK_DIST = 2,
//    MAP_ITEM    = 3,
//    PLAYER      = 4,
//    ENEMY       = 5,
//    MOVE_STEP_DIST   = 6, // 移動経路
//    SELECTED_DIST = 7, // 選択位置
//};
//
//enum MoveDirectionType {
//	MOVE_DOWN  = 0,
//	MOVE_LEFT  = 1,
//	MOVE_RIGHT = 2,
//	MOVE_UP    = 3
//};
//
//typedef struct _MapIndex {
//    int x;
//    int y;
//    MoveDirectionType moveDictType;
//}MapIndex;
//
//typedef struct _MapItem {
//    /** 短形マップ上の区分. */
//    MapDataType mapDataType;
//    
////    /** 短形マップ縦軸表示位置. */
////    int mapPointX;
////    
////    /** 短形マップ横軸表示位置. */
////    int mapPointY;
////    
//    MapIndex mapIndex;
//    
//    /** 移動可能距離. */
//    int moveDist;
//    /** 攻撃可能距離. */
//    int attackDist;
//}MapItem;
//
//typedef struct _ActorMapItem : public MapItem {
//    /** プレイヤーを一意に識別するID. */
//    int seqNo;
//    /** 移動済みフラグ. */
//    bool moveDone;
//    /** 攻撃済みフラグ. */
//    bool attackDone;
//}ActorMapItem;
//
//#define MAP_INDEX_DIFF(mapIndexA, mapIndexB) (mapIndexA.x == mapIndexB.x && mapIndexA.y == mapIndexB.y)

// TileMapから取得する
//#define GRID_X 128
//#define GRID_Y 64
//public static PointF indexToDisp(Point pMapIndex) {
//    return indexToDisp(pMapIndex.x, pMapIndex.y);
//}
//public static PointF indexToDisp(int x, int y) {
//    PointF dispPointF = new PointF();
//    dispPointF.set(
//                   (y + x) * (GRID_X / 2),
//                   ((BASE_Y - 1) - x + y) * (GRID_Y / 2));
//    return dispPointF;
//}
//
//public static Point dispToIndex(PointF pDispPointF) {
//    return dispToIndex(pDispPointF.x, pDispPointF.y);
//}
//public static Point dispToIndex(float x, float y) {
//    int view_y = (int)((y - GRID_Y * BASE_Y / 2) * 2);
//    return new Point(
//                     (int)((x - view_y + GRID_X * 10) / GRID_X - 10),
//                     (int)((x + view_y + GRID_X * 10) / GRID_X - 10));
//}

#endif
