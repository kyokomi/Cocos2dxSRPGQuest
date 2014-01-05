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
#include "ActorSprite.h"
#include "MapManager.h"

USING_NS_CC;

class SRPGMapLayer : public Layer
{
protected:
    enum kTag {
        kTiledMapTag    = 1,
        kCursorBaseTag      = 9000,
        kCursorMoveFindTag  = 9001,
        kCursorMoveStepTag  = 9002,
        kCursorSelectedTag  = 9003,
        kGridLineTag    = 10000,
        kActorBaseTag   = 100000,
    };
    enum zIndex {
        zTiledMapIndex   = 1,
        zCursorBaseIndex     = 9000,
        zCursorMoveFindIndex = 9001,
        zCursorMoveStepIndex = 9002,
        zCursorSelectedIndex = 9003,
        zGridLineIndex   = 10000,
        zActorBaseIndex  = 100000,
    };
public:
    // コンストラクタ、デストラクタ
    SRPGMapLayer();
    ~SRPGMapLayer();
    
    // 初期化、生成
    virtual bool init();
    CREATE_FUNC(SRPGMapLayer);
    
    // グリッドの表示/非表示
    bool isShowGrid();
    void showGrid();
    void hideGrid();
    
    // マップ座標変換
    Point indexToPoint(int mapIndex_x, int mapIndex_y);
    Point indexToPoint(MapIndex mapIndex);
    MapIndex pointToIndex(Point point);
    MapIndex touchPointToIndex(Point point);
    
    // マップ移動生成
    Point createTouchMoveMapPoint(Point mapPosition, float updateDelta, Point pDelta);

    // タッチイベント関連
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);

    // カーソル全消去
    void clearAllMapCursor();
    // カーソル表示/非表示
    bool isMapCursor(MapDataType mapDataType);
    void showMapCursor(MapDataType mapDataType);
    void hideMapCursor(MapDataType mapDataType);
    
private:
    bool m_moveAnimation;

    // マップ制御
    MapManager m_mapManager;
    
    // マップベース情報
    Size m_baseContentSize;
    Size m_baseMapSize;
    Size m_baseTileSize;
    
    // カーソル追加
    void addMapCursor(MapDataType pMapDataType, std::list<MapIndex> moveMapPointList);
    void visibleMapCursor(MapDataType mapDataType, bool visible);
    
    // アクター追加
    ActorMapItem* addActor(MapDataType pMapDataType, int pSeqNo, int pMapPointX, int pMapPointY, ActorSprite::ActorDto pActorDto);
    // アクター検索
    ActorSprite* findActorSprite(int seqNo);
    
    Point m_touchStartPoint;
    Point convertToSRPGMapPoint(Touch *pTouch);
    
    void executeMapIndex(MapIndex* mapIndex);
};


#endif /* defined(__Cocos2dxSRPGQuest__SRPGMapLayer__) */
