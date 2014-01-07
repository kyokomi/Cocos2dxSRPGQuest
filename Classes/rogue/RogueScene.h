//
//  RogueScene.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2014/01/02.
//
//

#ifndef __Cocos2dxSRPGQuest__RogueScene__
#define __Cocos2dxSRPGQuest__RogueScene__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "MapManager.h"
#include "ActorSprite.h"
#include "DropItemSprite.h"
#include "ItemWindowLayer.h"

class RogueScene : public cocos2d::Layer
{
protected:
    
    enum GameStatus {
        INIT        = 0,
        PLAYER_TURN = 10,
        PLAYER_ACTION  = 11,
        ENEMY_TURN  = 20,
        ENEMY_ACTION  = 21,
    };
    // TiledMap（background, colision)
        // TiledMapFront
        // TiledMapEnemy
    
    enum TiledMapTag {
        kGridLineTag             = 1000,
        kTiledMapDropItemBaseTag = 10000, // + seqNo
        kTiledMapEnemyBaseTag    = 20000, // + seqNo
        kTiledMapFrontTag        = 30000,
    };
    
    enum TiledMapIndex {
        zGridLineIndex             = 1000,
        zTiledMapDropItemBaseIndex = 10000,
        zTiledMapEnemyBaseIndex    = 20000,
        zTiledMapFrontIndex        = 30000,
    };
    
    enum Tag {
        kTiledMapTag          = 1,
//        kCursorBaseTag      = 9000,
//        kCursorMoveFindTag  = 9001,
//        kCursorMoveStepTag  = 9002,
//        kCursorSelectedTag  = 9003,
        kActorBaseTag         = 100000,
        kMiniMapTag           = 150000,
        kStatusBarTag         = 200000,
        kStatusBar2Tag        = 200001,
        kGameLogTag           = 210000,
        kItemListTag          = 220000,
        kItemDetailTag        = 221000,
        kMenuTag              = 300000,
        kModalTag             = 900000,
    };
    enum Index {
        zTiledMapIndex   = 1,
//        zCursorBaseIndex     = 9000,
//        zCursorMoveFindIndex = 9001,
//        zCursorMoveStepIndex = 9002,
//        zCursorSelectedIndex = 9003,
        zActorBaseIndex  = 100000,
        zMiniMapIndex    = 150000,
        zStatusBarIndex  = 200000,
        zStatusBar2Index = 200001,
        zGameLogIndex    = 210000,
        zItemListIndex   = 220000,
        zItemDetailIndex = 221000,
        zMenuIndex       = 300000,
        zModalIndex      = 900000,
    };

private:
    // ゲーム管理
    GameStatus m_gameStatus;
    int m_TurnCount;
    void changeGameStatus(GameStatus gameStatus);
    void enemyTurn();
    
    // マップベース情報
    cocos2d::Size m_baseContentSize;
    cocos2d::Size m_baseMapSize;
    cocos2d::Size m_baseTileSize;

    // マップ制御
    MapManager m_mapManager;
    
    // タッチイベント系
    void touchEventExec(cocos2d::Point touchPoint);
    void touchEventExec(MapIndex touchPointMapIndex);
    MapIndex checkTouchEventIndex(MapIndex touchPointMapIndex);
    void moveMap(MapIndex addMoveIndex, int actorSeqNo, MapDataType mapDataType, cocos2d::CallFunc* moveFinishedCallFunc);
    bool isTiledMapColisionLayer(MapIndex touchPointMapIndex);
    bool isMapLayerOver(MapIndex touchPointMapIndex);
    
    // UI関連
    void logMessage(const char * pszFormat, ...);
    
    // アイテムリスト
    void showItemList(int showTextIndex);
    void hideItemList();
    void refreshStatus();
    
    // tileSet関連
    bool tileSetEnemyActorMapItem(ActorSprite::ActorDto enemyActorDto, MapIndex mapIndex);
    bool tileSetDropMapItem(DropItemSprite::DropItemDto dropItemDto, MapIndex mapIndex);
    
    // マップ座標変換
    cocos2d::Point indexToPoint(int mapIndex_x, int mapIndex_y);
    cocos2d::Point indexToPoint(MapIndex mapIndex);
    cocos2d::Point indexToPointNotTileSize(int mapIndex_x, int mapIndex_y);
    cocos2d::Point indexToPointNotTileSize(MapIndex mapIndex);
    
    MapIndex pointToIndex(cocos2d::Point point);
    MapIndex touchPointToIndex(cocos2d::Point point);
    // MapIndexからtileIndexに変換
    MapIndex mapIndexToTileIndex(MapIndex mapIndex);
    
    
    ActorSprite* getPlayerActorSprite(int seqNo);
    ActorSprite* getEnemyActorSprite(int seqNo);
    
    ItemWindowLayer* getItemWindowLayer();
//    // TODO: プレイヤー情報（あとで別の場所に持っていく）
//    std::list<DropItemSprite::DropItemDto> m_playerItemList;
    
public:
    RogueScene();
    ~RogueScene();
    
    virtual bool init();
    
    static cocos2d::Scene* scene();
    
    CREATE_FUNC(RogueScene);
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);
};

#endif /* defined(__Cocos2dxSRPGQuest__RogueScene__) */
