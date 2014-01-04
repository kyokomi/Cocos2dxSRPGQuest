//
//  RogueScene.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2014/01/02.
//
//

#include "RogueScene.h"
#include "ActorSprite.h"

USING_NS_CC;

RogueScene::RogueScene()
:m_baseMapSize(Size::ZERO),
m_baseTileSize(Size::ZERO),
m_baseContentSize(Size::ZERO)
{
}

RogueScene::~RogueScene()
{
}

Scene* RogueScene::scene()
{
    Scene *scene = Scene::create();
    RogueScene *layer = RogueScene::create();
    scene->addChild(layer);
    return scene;
}

bool RogueScene::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    // TouchEvent settings
    auto listener = EventListenerTouchOneByOne::create();
    //listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(RogueScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(RogueScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(RogueScene::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto winSize = Director::getInstance()->getWinSize();
    
    // ---------------------
    // タイルマップを生成
    // ---------------------
    auto pTiledMap = TMXTiledMap::create("tmx/desert.tmx");
    pTiledMap->setPosition(Point::ZERO);
    this->addChild(pTiledMap, RogueScene::zTiledMapIndex, RogueScene::kTiledMapTag);
    
    m_baseMapSize = pTiledMap->getMapSize();
    m_baseTileSize = pTiledMap->getTileSize();
    m_baseContentSize = pTiledMap->getContentSize();
    
    m_mapManager.init(0, (int)m_baseMapSize.height, 0, (int)m_baseMapSize.width);
    
    // フロントレイヤー
    auto pFrontLayer = Layer::create();
    pTiledMap->addChild(pFrontLayer,
                        RogueScene::TiledMapIndex::zTiledMapFrontIndex,
                        RogueScene::TiledMapTag::kTiledMapFrontTag);

    // エネミーレイヤー
    auto pEnemyLayer = Layer::create();
    pTiledMap->addChild(pEnemyLayer,
                        RogueScene::TiledMapIndex::zTiledMapEnemyBaseIndex,
                        RogueScene::TiledMapTag::kTiledMapEnemyBaseTag);
    
    // ---------------------
    // グリッド線を生成
    // ---------------------
    auto draw = DrawNode::create();
    draw->setPosition(Point::ZERO);
    
    // 線の太さと色
    float lineSize = 1 * 0.5;
    Color4F color = Color4F::MAGENTA;
    
    // 縦線を引く
    for (int x = 1; x < m_baseMapSize.width; x++)
    {
        float xPoint = x * m_baseTileSize.width;
        draw->drawSegment(Point(xPoint, 0), Point(xPoint, m_baseContentSize.height), lineSize, color);
    }
    // 横線を引く
    for (int y = 1; y < m_baseMapSize.height; y++)
    {
        float yPoint = y * m_baseTileSize.height;
        draw->drawSegment(Point(0, yPoint), Point(m_baseContentSize.width, yPoint), lineSize, color);
    }
    
    // マップのフロントレイヤーに追加
    pFrontLayer->addChild(draw, RogueScene::TiledMapIndex::zGridLineIndex, RogueScene::TiledMapTag::kGridLineTag);
//    this->addChild(draw, RogueScene::zGridLineIndex, RogueScene::kGridLineTag);
    
    // ---------------------
    // プレイヤー生成
    // ---------------------
    ActorSprite::ActorDto actorDto;
    actorDto.attackRange = 1;
    actorDto.movePoint = 5;
    actorDto.playerId = 4;

    ActorMapItem actorMapItem;
    actorMapItem.mapDataType = MapDataType::PLAYER;
    actorMapItem.mapIndex = pointToIndex(Point(winSize.width/2, winSize.height/2));
    actorMapItem.seqNo = 1;
    actorMapItem.moveDist = 1; // TODO: 使わないかも
    actorMapItem.attackDist = 1;
    actorMapItem.moveDone = false;
    actorMapItem.attackDone = false;
    
    auto actorSprite = ActorSprite::createWithActorDto(actorDto);
    actorSprite->setPosition(indexToPoint(actorMapItem.mapIndex)); // 画面の中心
    actorSprite->setActorMapItem(actorMapItem);
    actorSprite->runBottomAction();
    // プレイヤーは画面中心にくるのでmapLayerに追加しない
    this->addChild(actorSprite, RogueScene::zActorBaseIndex, (RogueScene::kActorBaseTag + actorMapItem.seqNo));
    
    // マップに追加
    m_mapManager.addActor(actorSprite->getActorMapItem());
    
    // ---------------------
    // 敵キャラ生成
    // ---------------------
    ActorSprite::ActorDto enemyDto;
    enemyDto.attackRange = 1;
    enemyDto.movePoint = 5;
    enemyDto.playerId = 901;
    
    ActorMapItem enemyMapItem;
    enemyMapItem.mapDataType = MapDataType::ENEMY;
    enemyMapItem.mapIndex = {4, 4, MoveDirectionType::MOVE_DOWN};
    enemyMapItem.seqNo = 2;
    enemyMapItem.moveDist = 1; // TODO: 使わないかも
    enemyMapItem.attackDist = 1;
    enemyMapItem.moveDone = false;
    enemyMapItem.attackDone = false;
    
    auto enemySprite = ActorSprite::createWithActorDto(enemyDto);
    enemySprite->setPosition(indexToPoint(enemyMapItem.mapIndex)); // 画面の中心
    enemySprite->setActorMapItem(enemyMapItem);
    enemySprite->runBottomAction();
    pEnemyLayer->addChild(enemySprite, RogueScene::zTiledMapEnemyBaseIndex, (RogueScene::zTiledMapEnemyBaseIndex + enemyMapItem.seqNo));
//    this->addChild(enemySprite, RogueScene::zActorBaseIndex, (RogueScene::kActorBaseTag + enemyMapItem.seqNo));
    
    // マップに追加
    m_mapManager.addActor(enemySprite->getActorMapItem());
    
    //-------------------------
    // ステータスバー？
    //-------------------------
    auto statusLayer = LayerColor::create(Color4B::BLACK);
    statusLayer->setContentSize(Size(winSize.width, m_baseTileSize.height * 0.8));
    statusLayer->setPosition(Point(0, winSize.height - statusLayer->getContentSize().height));
    
    auto sampleText = LabelTTF::create(" 1F Lv1 HP 15/15 満腹度 100/100        0 Gold", "", 12);
    sampleText->setPosition(Point(sampleText->getContentSize().width / 2, statusLayer->getContentSize().height / 2));
    statusLayer->addChild(sampleText);
    
    this->addChild(statusLayer, RogueScene::zStatusBarIndex, RogueScene::kStatusBarTag);
    
    // ------------------------
    // ミニマップ
    // ------------------------
    // 青で半透明
    auto miniMapLayer = LayerColor::create(Color4B(0, 0, 196, 128));
    // 1/8サイズ
    miniMapLayer->setContentSize(Size(m_baseMapSize.width * m_baseTileSize.width / 8,
                                      m_baseMapSize.height * m_baseTileSize.height / 8));
    // ステータスバーの下くらい
    miniMapLayer->setPosition(0, miniMapLayer->getPositionY() + winSize.height - miniMapLayer->getContentSize().height - statusLayer->getContentSize().height);
    
    // プレイヤーの位置表示用（同じく1/8サイズ）TODO: 数が多くなるならBatchNodeとかにしないと？
    auto miniMapActorLayer = LayerColor::create(Color4B::YELLOW);
    // タイルの1/8サイズ
    miniMapActorLayer->setContentSize(m_baseTileSize / 8);
    // 現在位置からPositionを取得して1/8にする
    miniMapActorLayer->setPosition(indexToPointNotTileSize(actorSprite->getActorMapItem()->mapIndex) / 8);
    // 移動時に更新できるようにplayerIdをtag管理
    miniMapActorLayer->setTag(actorSprite->getActorDto()->playerId);
    // add
    miniMapLayer->addChild(miniMapActorLayer);
    
    // TODO: 敵の数だけよばないといけないので関数化するといい
    
    // 敵の位置表示用（同じく1/8サイズ）TODO: 数が多くなるならBatchNodeとかにしないと？
    auto miniMapEnemyLayer = LayerColor::create(Color4B::RED);
    // タイルの1/8サイズ
    miniMapEnemyLayer->setContentSize(m_baseTileSize / 8);
    // 現在位置からPositionを取得して1/8にする
    miniMapEnemyLayer->setPosition(indexToPointNotTileSize(enemySprite->getActorMapItem()->mapIndex) / 8);
    // 移動時に更新できるようにplayerIdをtag管理
    miniMapEnemyLayer->setTag(enemySprite->getActorDto()->playerId);
    // add
    miniMapLayer->addChild(miniMapEnemyLayer);
    
    
    this->addChild(miniMapLayer, RogueScene::zMiniMapIndex, RogueScene::kMiniMapTag);
    
    return true;
}

bool RogueScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{    
    return true;
}

void RogueScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void RogueScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
//    auto mapLayer = (TMXTiledMap*)getChildByTag(kTiledMapTag);
    auto touchPoint = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));

    // 行動判定
    touchEventExec(touchPoint);
}

void RogueScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    this->unscheduleUpdate();
}


//void RogueScene::touchEventExec(MapIndex touchPointMapIndex)
void RogueScene::touchEventExec(cocos2d::Point touchPoint)
{
    auto pMapLayer = (TMXTiledMap*)getChildByTag(kTiledMapTag);
    // マップ移動分を考慮
    MapIndex touchPointMapIndex = touchPointToIndex(touchPoint - pMapLayer->getPosition());
    CCLOG("onTouchBegan touchPointMapIndex x = %d y = %d", touchPointMapIndex.x, touchPointMapIndex.y);
    
    // 画面外判定
    if (m_baseMapSize.width <= touchPointMapIndex.x || m_baseMapSize.height <= touchPointMapIndex.y || 0 > touchPointMapIndex.x || 0 > touchPointMapIndex.y)
    {
        return;
    }
    // タッチした位置が有効なIndexか判定
    MapIndex addMoveIndex = checkTouchEventIndex(touchPointMapIndex);
    if (addMoveIndex.x == 0 && addMoveIndex.y == 0)
    {
        // タッチ判定とみなさない
        return;
    }
    
    // キャラの向きを変更
    auto pActorSprite = (ActorSprite*)getChildByTag((RogueScene::kActorBaseTag + 1));
    pActorSprite->runMoveAction(addMoveIndex);
    
    // 敵をタッチした
    if (m_mapManager.getActorMapItem(&touchPointMapIndex)->mapDataType == MapDataType::ENEMY)
    {
        // 攻撃イベント
        CCLOG("敵に攻撃開始");
    }
    else
    {
        // 障害物判定
        if (isTiledMapColisionLayer(touchPointMapIndex))
        {
            // TODO: ぶつかるSE再生
            CCLOG("壁ドーン");
        }
        else
        {
            // 移動処理
            moveMap(addMoveIndex);
        }
    }
    
    // 会話
        // 会話イベント
    
    // 宝箱
        // 宝箱イベント
}

MapIndex RogueScene::checkTouchEventIndex(MapIndex touchPointMapIndex)
{
    // 移動距離計算
    auto pActorSprite = (ActorSprite*)getChildByTag((RogueScene::kActorBaseTag + 1));
    
    MapIndex addMoveIndex;
    addMoveIndex.x = touchPointMapIndex.x - pActorSprite->getActorMapItem()->mapIndex.x;
    addMoveIndex.y = touchPointMapIndex.y - pActorSprite->getActorMapItem()->mapIndex.y;
    
    if (addMoveIndex.x == 1 && addMoveIndex.y == 0)
    {
        addMoveIndex.moveDictType = MoveDirectionType::MOVE_RIGHT;
    }
    else if (addMoveIndex.x == -1 && addMoveIndex.y == 0)
    {
        addMoveIndex.moveDictType = MoveDirectionType::MOVE_LEFT;
    }
    else if (addMoveIndex.x == 0 && addMoveIndex.y == 1)
    {
        addMoveIndex.moveDictType = MoveDirectionType::MOVE_UP;
    }
    else if (addMoveIndex.x == 0 && addMoveIndex.y == -1)
    {
        addMoveIndex.moveDictType = MoveDirectionType::MOVE_DOWN;
    }
    else
    {
        // 上記以外は有効なタッチじゃない
        addMoveIndex.x = 0;
        addMoveIndex.y = 0;
        addMoveIndex.moveDictType = MoveDirectionType::MOVE_DOWN;
        return addMoveIndex;
    }
    
    // プレイヤーから1マス以内なら移動or攻撃と判断
    CCLOG("move ok %d,%d %d,%d", pActorSprite->getActorMapItem()->mapIndex.x, pActorSprite->getActorMapItem()->mapIndex.y, touchPointMapIndex.x, touchPointMapIndex.y);
    
    CCLOG("addMoveIndex %d,%d", addMoveIndex.x, addMoveIndex.y);
    return addMoveIndex;
}

void RogueScene::moveMap(MapIndex addMoveIndex)
{
    // 移動距離計算
    auto pActorSprite = (ActorSprite*)getChildByTag((RogueScene::kActorBaseTag + 1));
    
//    MapIndex addMoveIndex;
//    addMoveIndex.x = touchPointMapIndex.x - pActorSprite->getActorMapItem()->mapIndex.x;
//    addMoveIndex.y = touchPointMapIndex.y - pActorSprite->getActorMapItem()->mapIndex.y;
//    
//    if (addMoveIndex.x == 1 && addMoveIndex.y == 0)
//    {
//        addMoveIndex.moveDictType = MoveDirectionType::MOVE_RIGHT;
//    }
//    else if (addMoveIndex.x == -1 && addMoveIndex.y == 0)
//    {
//        addMoveIndex.moveDictType = MoveDirectionType::MOVE_LEFT;
//    }
//    else if (addMoveIndex.x == 0 && addMoveIndex.y == 1)
//    {
//        addMoveIndex.moveDictType = MoveDirectionType::MOVE_UP;
//    }
//    else if (addMoveIndex.x == 0 && addMoveIndex.y == -1)
//    {
//        addMoveIndex.moveDictType = MoveDirectionType::MOVE_DOWN;
//    }
//    else
//    {
//        // 上記以外は有効なタッチじゃない
//        return;
//    }
//    
//    // プレイヤーから1マス以内なら移動or攻撃と判断
//    CCLOG("move ok %d,%d %d,%d", pActorSprite->getActorMapItem()->mapIndex.x, pActorSprite->getActorMapItem()->mapIndex.y, touchPointMapIndex.x, touchPointMapIndex.y);
//    
//    CCLOG("addMoveIndex %d,%d", addMoveIndex.x, addMoveIndex.y);
    
//    // キャラの向きを変更?これは別でやるべき？
//    if (addMoveIndex.moveDictType == MOVE_DOWN)
//    {
//        pActorSprite->runBottomAction();
//    }
//    else if (addMoveIndex.moveDictType == MOVE_LEFT)
//    {
//        pActorSprite->runLeftAction();
//    }
//    else if (addMoveIndex.moveDictType == MOVE_RIGHT)
//    {
//        pActorSprite->runRightAction();
//    }
//    else if (addMoveIndex.moveDictType == MOVE_UP)
//    {
//        pActorSprite->runTopAction();
//    }
  
    // 移動可能方向かチェック
    
    auto pMapLayer = (TMXTiledMap*)getChildByTag(kTiledMapTag);
    
//    // 障害物判定
//    auto pColisionLayer = pMapLayer->getLayer("colision");
//    // TileMapは左上から0,0なので座標変換する
//    auto touchPointTileIndex = mapIndexToTileIndex(touchPointMapIndex);
//    auto pTileSprite = pColisionLayer->getTileAt(Point(touchPointTileIndex.x, touchPointTileIndex.y));
//    if (pTileSprite)
//    {
//        // 障害物なので移動とかできない
//        CCLOG("colision touchPointTileIndex x = %d y = %d", touchPointTileIndex.x, touchPointTileIndex.y);
//        // TODO: ぶつかるSE再生
//        return;
//    }
    
    // 移動する距離をPointに変換
    auto addMovePoint = Point(m_baseTileSize.width * addMoveIndex.x, m_baseTileSize.height * addMoveIndex.y);
    
    // マップを指定index分移動
    auto pMoveToAction = MoveTo::create(0.3, pMapLayer->getPosition() - addMovePoint);
    pMapLayer->runAction(pMoveToAction);
    
    // マップ情報を更新
    MapIndex moveMapIndex;
    moveMapIndex.moveDictType = addMoveIndex.moveDictType;
    auto actorMapItem = pActorSprite->getActorMapItem();
    moveMapIndex.x = actorMapItem->mapIndex.x + addMoveIndex.x;
    moveMapIndex.y = actorMapItem->mapIndex.y + addMoveIndex.y;
    m_mapManager.moveActor(actorMapItem, &moveMapIndex);
    // actor情報も更新
    actorMapItem->mapIndex = moveMapIndex;
    
    // ミニマップも更新
    auto pMiniMapLayer = getChildByTag(kMiniMapTag);
    auto pMiniMapActorNode = (Node*) pMiniMapLayer->getChildByTag(pActorSprite->getActorDto()->playerId);
    pMiniMapActorNode->setPosition(indexToPointNotTileSize(actorMapItem->mapIndex) / 8);
}

bool RogueScene::isTiledMapColisionLayer(MapIndex touchPointMapIndex)
{
    // 障害物判定
    auto pMapLayer = (TMXTiledMap*)getChildByTag(kTiledMapTag);
    
    auto pColisionLayer = pMapLayer->getLayer("colision");
    // TileMapは左上から0,0なので座標変換する
    auto touchPointTileIndex = mapIndexToTileIndex(touchPointMapIndex);
    auto pTileSprite = pColisionLayer->getTileAt(Point(touchPointTileIndex.x, touchPointTileIndex.y));
    if (pTileSprite)
    {
        // 障害物なので移動とかできない
        CCLOG("colision touchPointTileIndex x = %d y = %d", touchPointTileIndex.x, touchPointTileIndex.y);
        return true;
    }
    
    return false;
}

#pragma mark
#pragma mark マップ座標変換

Point RogueScene::indexToPoint(MapIndex mapIndex)
{
    return indexToPoint(mapIndex.x, mapIndex.y);
}

Point RogueScene::indexToPoint(int mapIndex_x, int mapIndex_y)
{
    // タイルサイズを考慮
    return Point(m_baseTileSize.width * mapIndex_x + m_baseTileSize.width * 0.5, m_baseTileSize.height * mapIndex_y + m_baseTileSize.height * 0.5);
}

MapIndex RogueScene::pointToIndex(Point point)
{
    // タイルサイズを考慮
    point.x = point.x - m_baseTileSize.width * 0.5;
    point.y = point.y - m_baseTileSize.height * 0.5;
    return touchPointToIndex(point);
}

MapIndex RogueScene::touchPointToIndex(Point point)
{
    MapIndex mapIndex;
    mapIndex.x = point.x / m_baseTileSize.width;
    mapIndex.y = point.y / m_baseTileSize.height;
    return mapIndex;
}

Point RogueScene::indexToPointNotTileSize(MapIndex mapIndex)
{
    return indexToPointNotTileSize(mapIndex.x, mapIndex.y);
}

Point RogueScene::indexToPointNotTileSize(int mapIndex_x, int mapIndex_y)
{
    return Point(m_baseTileSize.width * mapIndex_x, m_baseTileSize.height * mapIndex_y);
}

MapIndex RogueScene::mapIndexToTileIndex(MapIndex mapIndex)
{
    MapIndex tileIndex;
    tileIndex.x = mapIndex.x;
    tileIndex.y = m_baseMapSize.height - mapIndex.y - 1;
    return tileIndex;
}
