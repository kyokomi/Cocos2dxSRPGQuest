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

int GetRandom(int min,int max);

RogueScene::RogueScene()
:m_gameStatus(GameStatus::INIT),
m_baseMapSize(Size::ZERO),
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
    
    // 乱数
    srand((unsigned int)time(NULL));
    
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

    //-------------------------
    // ゲームログ表示
    //-------------------------
    // TODO: あとで
//    auto statusLayer = LayerColor::create(Color4B::BLACK);
//    statusLayer->setContentSize(Size(winSize.width, m_baseTileSize.height * 0.8));
//    statusLayer->setPosition(Point(0, winSize.height - statusLayer->getContentSize().height));
//    
//    auto sampleText = LabelTTF::create(" 1F Lv1 HP 15/15 満腹度 100/100        0 Gold", "", 12);
//    sampleText->setPosition(Point(sampleText->getContentSize().width / 2, statusLayer->getContentSize().height / 2));
//    statusLayer->addChild(sampleText);
//    
//    this->addChild(statusLayer, RogueScene::zStatusBarIndex, RogueScene::kStatusBarTag);
    
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
    
    // プレイヤーの先行
    changeGameStatus(GameStatus::PLAYER_TURN);
    
    return true;
}

#pragma mark
#pragma mark ゲームステート関連

void RogueScene::changeGameStatus(GameStatus gameStatus)
{
    CCLOG("change gameStatus %d => %d", m_gameStatus, gameStatus);
    GameStatus beforeGameStatus = m_gameStatus;
    m_gameStatus = gameStatus;
    
    // 敵のターン開始時
    if (beforeGameStatus == GameStatus::PLAYER_TURN && m_gameStatus == GameStatus::ENEMY_TURN)
    {
        enemyTurn();
    }
}

#pragma mark
#pragma mark モンスターのAI関連

void RogueScene::enemyTurn()
{
    // モンスターの数だけ繰り返す
    std::list<ActorMapItem> enemyList = m_mapManager.findEnemyMapItem();
    for (ActorMapItem enemyMapItem : enemyList) {
        
        // TODO: 全部同時には動かないようにしないといけない（アニメーションのコールバック系どうするかな・・・）
//        while (true)
//        {
//            
//        }
        
        // ランダムでとどまるか移動するかきめる
        int rand = GetRandom(1, 2);
        if (rand == 1)
        {
            // とどまる
            CCLOG("とどまる seqNo = %d", enemyMapItem.seqNo);
        }
        else if (rand == 2)
        {
            // 移動 or 攻撃
            
            // 移動方向もとりあえずランダム
            int moveDict = GetRandom(0, 3);
            MapIndex addMoveIndex;
            if (moveDict == MoveDirectionType::MOVE_DOWN)
            {
                addMoveIndex.x = 0;
                addMoveIndex.y = -1;
                addMoveIndex.moveDictType = MoveDirectionType::MOVE_DOWN;
            }
            else if (moveDict == MoveDirectionType::MOVE_UP)
            {
                addMoveIndex.x = 0;
                addMoveIndex.y = 1;
                addMoveIndex.moveDictType = MoveDirectionType::MOVE_UP;
            }
            else if (moveDict == MoveDirectionType::MOVE_RIGHT)
            {
                addMoveIndex.x = 1;
                addMoveIndex.y = 0;
                addMoveIndex.moveDictType = MoveDirectionType::MOVE_RIGHT;
            }
            else if (moveDict == MoveDirectionType::MOVE_LEFT)
            {
                addMoveIndex.x = -1;
                addMoveIndex.y = 0;
                addMoveIndex.moveDictType = MoveDirectionType::MOVE_LEFT;
            }
            
            // 移動有無関係なく向きは変える
            auto pEnemyMapLayer = getChildByTag(RogueScene::kTiledMapTag)->getChildByTag(RogueScene::TiledMapTag::kTiledMapEnemyBaseTag);
            auto pEnemySprite = (ActorSprite*)pEnemyMapLayer->getChildByTag(RogueScene::TiledMapTag::kTiledMapEnemyBaseTag + enemyMapItem.seqNo);
            pEnemySprite->runMoveAction(addMoveIndex);
            
            // 画面外判定とか用に移動先のmapIndex作成
            MapIndex moveMapIndex = {
                pEnemySprite->getActorMapItem()->mapIndex.x + addMoveIndex.x,
                pEnemySprite->getActorMapItem()->mapIndex.y + addMoveIndex.y,
                addMoveIndex.moveDictType
            };
            if (m_baseMapSize.width <= moveMapIndex.x || m_baseMapSize.height <= moveMapIndex.y || 0 > moveMapIndex.x || 0 > moveMapIndex.y)
            {
                CCLOG("移動不可 seqNo = %d", enemyMapItem.seqNo);
            }
            else if (isTiledMapColisionLayer(moveMapIndex))
            {
                CCLOG("壁ドーン seqNo = %d", enemyMapItem.seqNo);
            }
            else if (m_mapManager.getActorMapItem(&moveMapIndex)->mapDataType == MapDataType::ENEMY)
            {
                CCLOG("敵ドーン seqNo = %d", enemyMapItem.seqNo);
            }
            else if (m_mapManager.getActorMapItem(&moveMapIndex)->mapDataType == MapDataType::PLAYER)
            {
                // 攻撃イベント
                CCLOG("プレイヤーに攻撃開始 seqNo = %d", enemyMapItem.seqNo);
            }
            else
            {
                // 移動中のステータスへ
                changeGameStatus(GameStatus::ENEMY_ACTION);
                // 移動開始
                moveMap(addMoveIndex, enemyMapItem.seqNo, enemyMapItem.mapDataType, CallFunc::create([this](void) {

                    // 移動終わり次のモンスターへいきたいところ
                    // listをメンバ変数で持っちゃえばいけるか？
//                    changeGameStatus(GameStatus::ENEMY_TURN);
                    changeGameStatus(GameStatus::PLAYER_TURN);
                }));
                CCLOG("移動した seqNo = %d", enemyMapItem.seqNo);
            }
        }
    }
    // 留まった時とかはここでターン終了
    if (m_gameStatus != GameStatus::ENEMY_ACTION)
    {
        changeGameStatus(GameStatus::PLAYER_TURN);
    }
}

#pragma mark
#pragma mark タッチイベント関連

bool RogueScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if (m_gameStatus == GameStatus::PLAYER_TURN)
    {
        return true;
    }
    return false;
}

void RogueScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{

}

void RogueScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto touchPoint = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));

    // 行動判定
    touchEventExec(touchPoint);
}

void RogueScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event)
{
    this->unscheduleUpdate();
}


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
            moveMap(addMoveIndex, pActorSprite->getActorMapItem()->seqNo, pActorSprite->getActorMapItem()->mapDataType, CallFunc::create([this](void) {
                changeGameStatus(GameStatus::ENEMY_TURN);
            }));
            // コールバックまでgameStatusを更新はしない
            return;
        }
    }
    
    // TODO: 会話
        // 会話イベント
    
    // TODO: 宝箱ひろう
        // 宝箱イベント
    
    // ターン終了
    changeGameStatus(GameStatus::ENEMY_TURN);
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

void RogueScene::moveMap(MapIndex addMoveIndex, int actorSeqNo, MapDataType mapDataType, CallFunc* moveFinishedCallFunc)
{
    ActorSprite* pActorSprite;
    if (mapDataType == MapDataType::PLAYER)
    {
        pActorSprite = (ActorSprite*)getChildByTag(RogueScene::kActorBaseTag + actorSeqNo);
    }
    else
    {
        auto pEnemyMapLayer = getChildByTag(RogueScene::kTiledMapTag)->getChildByTag(RogueScene::TiledMapTag::kTiledMapEnemyBaseTag);
        pActorSprite = (ActorSprite*)pEnemyMapLayer->getChildByTag(RogueScene::TiledMapTag::kTiledMapEnemyBaseTag + actorSeqNo);
    }
    
    // 移動距離計算
    auto pMapLayer = (TMXTiledMap*)getChildByTag(kTiledMapTag);
    
    // 移動する距離をPointに変換
    auto addMovePoint = Point(m_baseTileSize.width * addMoveIndex.x, m_baseTileSize.height * addMoveIndex.y);
    
    Node* pActionTargetNode;
    FiniteTimeAction* pMoveRunAction;
    // マップを指定index分移動
    if (pActorSprite->getActorMapItem()->mapDataType == MapDataType::PLAYER)
    {
        // プレイヤーならマップが移動
        pMoveRunAction = MoveTo::create(0.3, pMapLayer->getPosition() - addMovePoint);
//        pMoveRunAction->setTarget(pMapLayer);
//      pMapLayer->runAction(pMoveToAction);
        pActionTargetNode = pMapLayer;
    }
    else
    {
        // モンスターは普通にモンスターが移動
        pMoveRunAction = MoveTo::create(0.3, pActorSprite->getPosition() + addMovePoint);
//        pMoveRunAction->setTarget(pActorSprite);
//      pActorSprite->runAction(pMoveToAction);
        pActionTargetNode = pActorSprite;
    }

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
    
    // move実行
    pActionTargetNode->runAction(Sequence::create(pMoveRunAction, moveFinishedCallFunc, NULL));
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

#pragma mark
#pragma mark その他

int GetRandom(int min,int max)
{
	return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}

