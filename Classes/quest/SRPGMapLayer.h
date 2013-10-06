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

private:
    // マップ制御
    MapManager m_mapManager;
    
    // マップベース情報
    Size m_baseContentSize;
    Size m_baseMapSize;
    Size m_baseTileSize;
    
    // カーソル追加
    void addMapCursor(MapDataType pMapDataType, std::list<MapIndex> moveMapPointList);
    // アクター追加
    ActorMapItem* addActor(MapDataType pMapDataType, int pSeqNo, int pMapPointX, int pMapPointY, ActorSprite::ActorDto pActorDto);
    
    Point m_touchStartPoint;
    Point convertToSRPGMapPoint(Touch *pTouch);
};


#endif /* defined(__Cocos2dxSRPGQuest__SRPGMapLayer__) */
