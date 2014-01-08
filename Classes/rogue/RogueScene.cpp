//
//  RogueScene.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2014/01/02.
//
//
#include "AppMacros.h"

#include "RogueScene.h"
// ------------------------------
#include "ActorSprite.h"
#include "DropItemSprite.h"

#include "TableViewTestLayer.h"
#include "ModalLayer.h"
#include "ItemWindowLayer.h"

#include "ItemLogic.h"
#include "BattleLogic.h"

//#include "MenuItemSelectLabelSprite.h"

USING_NS_CC;

int GetRandom(int min,int max);

std::size_t f_r(const std::string& s, char c)
{
    std::string::size_type pos = s.find(c);
    return (pos == std::string::npos) ? 0 : (1 + f_r(s.substr(pos+1), c));
}

RogueScene::RogueScene()
:m_gameStatus(GameStatus::INIT),
m_TurnCount(0),
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
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(RogueScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(RogueScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(RogueScene::onTouchEnded, this);
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    this->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
    
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

    // 使ってなかった
//    // フロントレイヤー
//    auto pFrontLayer = Layer::create();
//    pTiledMap->addChild(pFrontLayer,
//                        RogueScene::TiledMapIndex::zTiledMapFrontIndex,
//                        RogueScene::TiledMapTag::kTiledMapFrontTag);
    
    // エネミーレイヤー
    auto pEnemyLayer = Layer::create();
    pTiledMap->addChild(pEnemyLayer,
                        RogueScene::TiledMapIndex::zTiledMapEnemyBaseIndex,
                        RogueScene::TiledMapTag::kTiledMapEnemyBaseTag);
    
    // ドロップアイテムレイヤー
    auto pDropItemLayer = Layer::create();
    pTiledMap->addChild(pDropItemLayer,
                        RogueScene::TiledMapIndex::zTiledMapDropItemBaseIndex,
                        RogueScene::TiledMapTag::kTiledMapDropItemBaseTag);
    
    // 障害物をmapManagerに適応する
    auto pColisionLayer = pTiledMap->getLayer("colision");
    for (int x = 0; x < m_baseMapSize.width; x++)
    {
        for (int y = 0; y < m_baseMapSize.height; y++)
        {
            if (pColisionLayer->getTileAt(Point(x, y)))
            {
                MapIndex mapIndex = {x, y, MoveDirectionType::MOVE_NONE};
                auto tileMapIndex = mapIndexToTileIndex(mapIndex);
                m_mapManager.addObstacle(&tileMapIndex);
            }
        }
    }
    
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
    
    // マップに追加
    pTiledMap->addChild(draw, RogueScene::TiledMapIndex::zGridLineIndex, RogueScene::TiledMapTag::kGridLineTag);

    //-------------------------
    // ステータスバー？
    //-------------------------
    auto statusLayer = LayerColor::create(Color4B::BLACK);
    statusLayer->setContentSize(Size(winSize.width, m_baseTileSize.height * 0.8));
    statusLayer->setPosition(Point(0, winSize.height - statusLayer->getContentSize().height));
    
    // TODO: あとで更新する
    auto sampleText = LabelTTF::create(" --F Lv-- HP ---/--- 満腹度 ---/---          - G", GAME_FONT(16), 16);
    
    sampleText->setPosition(Point(sampleText->getContentSize().width / 2, statusLayer->getContentSize().height / 2));
    statusLayer->addChild(sampleText);
    
    this->addChild(statusLayer, RogueScene::zStatusBarIndex, RogueScene::kStatusBarTag);
    
    //    // 下のステータスバー2
    //    auto pStatusLayer2 = LayerColor::create(Color4B::BLACK);
    //    pStatusLayer2->setContentSize(Size(m_baseTileSize.width, m_baseTileSize.height));
    //    pStatusLayer2->setPosition(Point(0, 0));
    //
    //    // TODO: アイコン表示するかな（ステータスバー２？）
    //    auto pFaceSprite = Sprite::createWithSpriteFrame(SpriteFrame::create("actor_4_f.png", Rect(0, 0, 96, 96)));
    //    float scale = 1.0f / 3.0f;
    //    pFaceSprite->setScale(scale, scale);
    //    //    pFaceSprite->setContentSize(pFaceSprite->getContentSize() * scale);
    //    //    CCLOG("getContentSize (%f, %f) ", pFaceSprite->getContentSize().width, pFaceSprite->getContentSize().height);
    //    //    pFaceSprite->setPosition(Point(pFaceSprite->getContentSize().width / 2, pFaceSprite->getContentSize().height / 2));
    //    pFaceSprite->setPosition(Point(pFaceSprite->getContentSize().width * pFaceSprite->getScaleX() / 2, pFaceSprite->getContentSize().height * pFaceSprite->getScaleY() / 2));
    //    pStatusLayer2->addChild(pFaceSprite);
    //
    //    this->addChild(pStatusLayer2, RogueScene::zStatusBar2Index, RogueScene::kStatusBar2Tag);
    
    //-------------------------
    // ゲームログ表示
    //-------------------------
    //    float startWidth = pFaceSprite->getContentSize().width * pFaceSprite->getScaleX();
    auto pGameLogLayer = LayerColor::create(Color4B(0, 0, 0, 192));
    pGameLogLayer->setContentSize(Size(winSize.width * 0.8, m_baseTileSize.height * 1.5));
    pGameLogLayer->setPosition(winSize.width / 2 - pGameLogLayer->getContentSize().width / 2, 0);
    
    int baseFontSize = 10;
    auto pLogTextLabel = LabelTTF::create("", GAME_FONT(baseFontSize), baseFontSize, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::TOP);
    pLogTextLabel->setPosition(Point(pLogTextLabel->getContentSize().width / 2 + pLogTextLabel->getFontSize() / 4, pGameLogLayer->getContentSize().height - pLogTextLabel->getContentSize().height / 2 - pLogTextLabel->getFontSize() / 4));
    pGameLogLayer->addChild(pLogTextLabel);
    this->addChild(pGameLogLayer, RogueScene::zGameLogIndex, RogueScene::kGameLogTag);
    
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
    this->addChild(miniMapLayer, RogueScene::zMiniMapIndex, RogueScene::kMiniMapTag);
    
    // ------------------------
    // イベントリ作成
    // ------------------------
    showItemList(1);
    hideItemList();
    
    // ---------------------
    // プレイヤー生成
    // ---------------------
    ActorSprite::ActorDto actorDto;
    actorDto.name = "ジニー";
    actorDto.faceImgId = 0;
    actorDto.imageResId = 1015;
    // 基本
    actorDto.attackRange = 1;
    actorDto.movePoint = 5;
    actorDto.playerId = 4;
    // 攻守
    actorDto.attackPoint = 5;
    actorDto.defencePoint = 1;
    // 経験値
    actorDto.exp = 0;
    actorDto.nextExp = 10;
    // HP
    actorDto.hitPoint = 15;
    actorDto.hitPointLimit = 15;
    actorDto.lv = 1;
    // 満腹度？精神力？
    actorDto.magicPoint = 100;
    actorDto.magicPointLimit = 100;

    ActorMapItem actorMapItem;
    actorMapItem.mapDataType = MapDataType::PLAYER;
    // 画面の中心（固定）
    actorMapItem.mapIndex = pointToIndex(Point(winSize.width/2, winSize.height/2));
    actorMapItem.seqNo = 1;
    actorMapItem.moveDist = actorDto.movePoint;
    actorMapItem.attackDist = actorDto.attackRange;
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
    
    refreshStatus();
    
    // プレイヤーの位置表示用（同じく1/8サイズ）
    auto miniMapActorLayer = LayerColor::create(Color4B::YELLOW);
    // タイルの1/8サイズ
    miniMapActorLayer->setContentSize(m_baseTileSize / 8);
    // 現在位置からPositionを取得して1/8にする
    miniMapActorLayer->setPosition(indexToPointNotTileSize(actorSprite->getActorMapItem()->mapIndex) / 8);
    // 移動時に更新できるようにplayerIdをtag管理
    miniMapActorLayer->setTag(actorSprite->getTag());
    // add
    miniMapLayer->addChild(miniMapActorLayer);
    
    // ---------------------
    // 敵キャラ生成
    // ---------------------
    ActorSprite::ActorDto enemyDto;
    enemyDto.name = "スライム";
    enemyDto.faceImgId = 0;
    enemyDto.imageResId = 1011;
    // 基本
    enemyDto.attackRange = 1; // TODO: 未使用
    enemyDto.movePoint = 10; // 索敵範囲
    enemyDto.playerId = 901;
    // 攻守
    enemyDto.attackPoint = 2;
    enemyDto.defencePoint = 0;
    // 経験値
    enemyDto.exp = 0;
    enemyDto.nextExp = 10;
    // HP
    enemyDto.hitPoint = 10;
    enemyDto.hitPointLimit = 10;
    enemyDto.lv = 1;
    // 満腹度？精神力？
    enemyDto.magicPoint = 100;
    enemyDto.magicPointLimit = 100;
    
    MapIndex enemyMapIndex1 = {4, 4, MoveDirectionType::MOVE_DOWN};
    tileSetEnemyActorMapItem(enemyDto, enemyMapIndex1);
    
    ActorSprite::ActorDto enemyDto2 = enemyDto;
    MapIndex enemyMapIndex2 = {14,12, MoveDirectionType::MOVE_DOWN};
    tileSetEnemyActorMapItem(enemyDto2, enemyMapIndex2);
    
    ActorSprite::ActorDto enemyDto3 = enemyDto;
    MapIndex enemyMapIndex3 = {20,4, MoveDirectionType::MOVE_DOWN};
    tileSetEnemyActorMapItem(enemyDto3, enemyMapIndex3);
    
    //-------------------------
    // アイテム配置
    //-------------------------
    DropItemSprite::DropItemDto dropItemDto;
    dropItemDto.itemId = 1;
    dropItemDto.imageResId = 64; // imageId 10064
    dropItemDto.name = "ポーション";
    
    MapIndex mapIndex = {7, 5, MoveDirectionType::MOVE_NONE};
    tileSetDropMapItem(dropItemDto, mapIndex);

    DropItemSprite::DropItemDto dropItemDto2;
    dropItemDto2.itemId = 2;
    dropItemDto2.imageResId = 168; // imageId 10168
    dropItemDto2.name = "ぶどう";
    
    MapIndex mapIndex2 = {10, 9, MoveDirectionType::MOVE_NONE};
    tileSetDropMapItem(dropItemDto2, mapIndex2);
    
    // -------------------------------
    // メニュー
    // -------------------------------
    auto rect = Rect(0, 0, 300, 30);
    auto capRect = Rect(0, 0, 300, 30);
    auto pScale9Sprite1 = extension::Scale9Sprite::create("menu_button.png", rect, capRect);
    pScale9Sprite1->setContentSize(Size(40, 20));
    pScale9Sprite1->setOpacity(192);
    auto pScale9Sprite2 = extension::Scale9Sprite::create("menu_button.png", rect, capRect);
    pScale9Sprite2->setContentSize(Size(40, 20));
    pScale9Sprite2->setOpacity(128);
    
    auto pMenuItem1 = MenuItemSprite::create(pScale9Sprite1, pScale9Sprite2, [this](Object *pSender) {
        CCLOG("menuItem1が押された！");
            showItemList(1);
    });
    pMenuItem1->setColor(Color3B::GREEN);
    pMenuItem1->setPosition(Point(winSize.width - pMenuItem1->getContentSize().width / 2, pMenuItem1->getContentSize().height / 2));
    auto pMenu = Menu::create(pMenuItem1, NULL);
    pMenu->setPosition(Point::ZERO);
    this->addChild(pMenu, RogueScene::zMenuIndex, RogueScene::kMenuTag);

    // ---------------------------------
    // プレイヤーの先行
    changeGameStatus(GameStatus::PLAYER_TURN);
    
    return true;
}

#pragma mark
#pragma mark ゲームステート関連

void RogueScene::changeGameStatus(GameStatus gameStatus)
{
    CCLOG("turn %d change gameStatus %d => %d", m_TurnCount, m_gameStatus, gameStatus);
    
    GameStatus beforeGameStatus = m_gameStatus;
    m_gameStatus = gameStatus;
    
    // 敵のターン開始時
    if ((beforeGameStatus == GameStatus::PLAYER_TURN || beforeGameStatus == GameStatus::PLAYER_ACTION)
        && m_gameStatus == GameStatus::ENEMY_TURN)
    {
        enemyTurn();
    }
    else if (m_gameStatus == GameStatus::PLAYER_TURN)
    {
        // カーソルはクリアする
        m_mapManager.clearCursor();
        // ターン数を進める
        m_TurnCount++;
        
        // TODO: とりあえずここで・・・
        auto pPlayer = getPlayerActorSprite(1);
        auto pPlayerDto = pPlayer->getActorDto();
        
        // １ターンに1空腹度が減るという
        if (pPlayerDto->magicPoint > 0)
        {
            pPlayerDto->magicPoint--;
        }
        refreshStatus();
    }
}

#pragma mark
#pragma mark モンスターのAI関連

void RogueScene::enemyTurn()
{
    // モンスターの数だけ繰り返す
    std::list<ActorMapItem> enemyList = m_mapManager.findEnemyMapItem();
    for (ActorMapItem enemyMapItem : enemyList)
    {
        // ランダムでとどまるか移動するかきめる
        int rand = GetRandom(2, 2);
        if (rand == 1)
        {
            auto pEnemySprite = getEnemyActorSprite(enemyMapItem.seqNo);
            pEnemySprite->getActorMapItem()->moveDone = true;
            
            // とどまる
            logMessage("様子を見ている seqNo = %d", enemyMapItem.seqNo);
        }
        else if (rand == 2)
        {
            // プレイヤーに向かって移動 or プレイヤーに攻撃
            auto pPlayerActorSprite = getPlayerActorSprite(1);
            
            // プレイヤーの周辺で最もコストが低い箇所へ移動
            auto playerMapIndex = pPlayerActorSprite->getActorMapItem()->mapIndex;
            std::list<MapIndex> searchMapIndexList;
            searchMapIndexList.clear();
            
            // 右
            MapIndex searchMapIndex = playerMapIndex;
            searchMapIndex.x += 1;
            searchMapIndex.y += 0;
            searchMapIndex.moveDictType = MoveDirectionType::MOVE_LEFT;
            searchMapIndexList.push_back(searchMapIndex);
            // 左
            searchMapIndex = playerMapIndex;
            searchMapIndex.x += -1;
            searchMapIndex.y += 0;
            searchMapIndex.moveDictType = MoveDirectionType::MOVE_RIGHT;
            searchMapIndexList.push_back(searchMapIndex);
            // 上
            searchMapIndex = playerMapIndex;
            searchMapIndex.x += 0;
            searchMapIndex.y += 1;
            searchMapIndex.moveDictType = MoveDirectionType::MOVE_DOWN;
            searchMapIndexList.push_back(searchMapIndex);
            // 下
            searchMapIndex = playerMapIndex;
            searchMapIndex.x += 0;
            searchMapIndex.y += -1;
            searchMapIndex.moveDictType = MoveDirectionType::MOVE_UP;
            searchMapIndexList.push_back(searchMapIndex);
            
            // そもそもプレイヤーが隣接しているかチェック
            bool isPlayerAttack = false;
            {
                for (MapIndex mapIndex : searchMapIndexList)
                {
                    if (MAP_INDEX_DIFF(enemyMapItem.mapIndex, mapIndex))
                    {
                        isPlayerAttack = true;
                    }
                }
            }
            
            MapIndex moveMapIndex = enemyMapItem.mapIndex;
            if (isPlayerAttack)
            {
                // 攻撃
                moveMapIndex =pPlayerActorSprite->getActorMapItem()->mapIndex;
            }
            else
            {
                // 移動可能な経路情報を設定
                m_mapManager.createActorFindDist(enemyMapItem.mapIndex, enemyMapItem.moveDist);
                
                // 最も移動コストがかからない場所を抽出
                MapItem targetMoveDistMapItem = m_mapManager.searchTargetMapItem(searchMapIndexList);
                
                // 移動リスト作成
                if (targetMoveDistMapItem.mapDataType == MapDataType::MOVE_DIST)
                {
                    std::list<MapIndex> moveList = m_mapManager.createMovePointList(&targetMoveDistMapItem.mapIndex,
                                                                                    &enemyMapItem);
                    std::list<MapIndex>::iterator it = moveList.begin();
                    it++;
                    moveMapIndex = *it; // 2件目を取得(1件目は自分なので）
                    it = moveList.end();
                }
            }
            
            // 移動有無関係なく向きは変える
            auto pEnemySprite = getEnemyActorSprite(enemyMapItem.seqNo);
            MapIndex addMoveIndex = {
                moveMapIndex.x - pEnemySprite->getActorMapItem()->mapIndex.x,
                moveMapIndex.y - pEnemySprite->getActorMapItem()->mapIndex.y,
                m_mapManager.checkMoveDirectionType(moveMapIndex, pEnemySprite->getActorMapItem()->mapIndex)
            };
            pEnemySprite->runMoveAction(addMoveIndex);
            // 行動前にする
            pEnemySprite->getActorMapItem()->moveDone = false;
            pEnemySprite->getActorMapItem()->attackDone = false;
            
            if (isMapLayerOver(moveMapIndex))
            {
                CCLOG("移動不可 seqNo = %d (%d, %d)", enemyMapItem.seqNo, moveMapIndex.x, moveMapIndex.y);
                pEnemySprite->getActorMapItem()->moveDone = true;
            }
            else if (isTiledMapColisionLayer(moveMapIndex))
            {
                logMessage("壁ドーン seqNo = %d (%d, %d)", enemyMapItem.seqNo, moveMapIndex.x, moveMapIndex.y);
                pEnemySprite->getActorMapItem()->moveDone = true;
            }
            else if (m_mapManager.getActorMapItem(&moveMapIndex)->mapDataType == MapDataType::ENEMY)
            {
                if (MAP_INDEX_DIFF(enemyMapItem.mapIndex, moveMapIndex))
                {
                    //logMessage("待機 seqNo = %d (%d, %d)");
                }
                else
                {
                    logMessage("敵ドーン seqNo = %d (%d, %d)", enemyMapItem.seqNo, moveMapIndex.x, moveMapIndex.y);
                }
                pEnemySprite->getActorMapItem()->moveDone = true;
            }
            else if (m_mapManager.getActorMapItem(&moveMapIndex)->mapDataType == MapDataType::PLAYER)
            {
                auto pPlayerDto = pPlayerActorSprite->getActorDto();
                auto pEnemyDto = pEnemySprite->getActorDto();
                
                int damage = BattleLogic::exec(pEnemyDto, pPlayerDto);
                
                // 攻撃イベント
                logMessage("%sの攻撃: %sに%dダメージ", pEnemyDto->name.c_str(), pPlayerDto->name.c_str(), damage);
                
                pEnemySprite->getActorMapItem()->attackDone = true;
            }
            else
            {
                // 移動中のステータスへ
                changeGameStatus(GameStatus::ENEMY_ACTION);
                // 移動開始
                moveMap(addMoveIndex, enemyMapItem.seqNo, enemyMapItem.mapDataType, CallFuncN::create([this, pEnemySprite](Object* pObj) {

                    // 移動終わり次のモンスターへいきたいところ
                    // listをメンバ変数で持っちゃえばいけるか？
                    auto pEnemySprite = static_cast<ActorSprite*>(pObj);
                    pEnemySprite->getActorMapItem()->moveDone = true;
                    
                    bool isTurnEnd = true;
                    std::list<ActorMapItem> enemyList = m_mapManager.findEnemyMapItem();
                    for (ActorMapItem enemyMapItem : enemyList)
                    {
                        auto pEnemySprite = getEnemyActorSprite(enemyMapItem.seqNo);
                        auto pEnemyMapItem = pEnemySprite->getActorMapItem();
                        if (!pEnemyMapItem->moveDone && !pEnemyMapItem->attackDone)
                        {
                            isTurnEnd = false;
                            break;
                        }
                    }
                    if (isTurnEnd)
                    {
                        changeGameStatus(GameStatus::PLAYER_TURN);
                    }
                    else
                    {
                        changeGameStatus(GameStatus::ENEMY_TURN);
                    }
                }));
            }
        }
    }
    // 留まった時とかはここでターン終了
    if (m_gameStatus != GameStatus::ENEMY_ACTION)
    {
        changeGameStatus(GameStatus::PLAYER_TURN);
    }
    bool isTurnEnd = true;
    for (ActorMapItem enemyMapItem : enemyList)
    {
        auto pEnemySprite = getEnemyActorSprite(enemyMapItem.seqNo);
        auto pEnemyMapItem = pEnemySprite->getActorMapItem();
        if (!pEnemyMapItem->moveDone && !pEnemyMapItem->attackDone)
        {
            isTurnEnd = false;
            break;
        }
    }
}

#pragma mark
#pragma mark タッチイベント関連

bool RogueScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    CCLOG("RogueScene onTouchBegan");
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
    if (m_gameStatus == GameStatus::PLAYER_TURN)
    {
        auto touchPoint = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));

        // 行動判定
        touchEventExec(touchPoint);
    }
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
    if (isMapLayerOver(touchPointMapIndex))
    {
        // タッチ判定とみなさない
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
    auto pActorSprite = getPlayerActorSprite(1);
    pActorSprite->runMoveAction(addMoveIndex);
    
    // 敵をタッチした
    auto pEnemyMapItem = m_mapManager.getActorMapItem(&touchPointMapIndex);
    if (pEnemyMapItem->mapDataType == MapDataType::ENEMY)
    {
        auto pPlayerDto = pActorSprite->getActorDto();
        auto pEnemyDto = getEnemyActorSprite(pEnemyMapItem->seqNo)->getActorDto();
        
        int damage = BattleLogic::exec(pPlayerDto, pEnemyDto);
        
        // 攻撃イベント
        logMessage("%sの攻撃: %sに%dのダメージ", pPlayerDto->name.c_str(), pEnemyDto->name.c_str(), damage);
    }
    else
    {
        // 障害物判定
        if (isTiledMapColisionLayer(touchPointMapIndex))
        {
            // TODO: ぶつかるSE再生
            logMessage("壁ドーン");
        }
        else
        {
            changeGameStatus(GameStatus::PLAYER_ACTION);
            
            // アイテムに重なったときの拾う処理
            auto pTouchPointMapItem = m_mapManager.getMapItem(&touchPointMapIndex);
            if (pTouchPointMapItem->mapDataType == MapDataType::MAP_ITEM)
            {
                // ドロップアイテムを拾う
                auto pDropMapItem = (DropMapItem*) pTouchPointMapItem;
                
                // TODO: 拾うSE再生
                
                // itemを取得
                auto pDropItemLayer = getChildByTag(RogueScene::kTiledMapTag)->getChildByTag(RogueScene::TiledMapTag::kTiledMapDropItemBaseTag);
                auto pDropItemSprite = (DropItemSprite*) pDropItemLayer->getChildByTag(RogueScene::TiledMapTag::kTiledMapDropItemBaseTag + pDropMapItem->seqNo);
                
                // メッセージログ
                auto pDropItemDto = pDropItemSprite->getDropItemDto();
                logMessage("%sを拾った。", pDropItemDto->name.c_str());
                
                // イベントリに追加する
                getItemWindowLayer()->addItemList(pDropItemDto);
                
                // mapManagerから消す
                m_mapManager.removeMapItem(pDropMapItem);
                
                // ミニマップを更新
                auto pMiniMapLayer = getChildByTag(kMiniMapTag);
                pMiniMapLayer->removeChildByTag(pDropItemSprite->getTag());
                
                // Map上からremoveする
                pDropItemLayer->removeChild(pDropItemSprite);
            }
            
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
    
    // ターン終了
    changeGameStatus(GameStatus::ENEMY_TURN);
}

MapIndex RogueScene::checkTouchEventIndex(MapIndex touchPointMapIndex)
{
    // 移動距離計算
    auto pActorSprite = getPlayerActorSprite(1);
    
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
        pActorSprite = getPlayerActorSprite(actorSeqNo);
    }
    else
    {
        pActorSprite = getEnemyActorSprite(actorSeqNo);
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
        pMoveRunAction = MoveTo::create(0.2, pMapLayer->getPosition() - addMovePoint);
        pActionTargetNode = pMapLayer;
    }
    else
    {
        // モンスターは普通にモンスターが移動
        pMoveRunAction = MoveTo::create(0.2, pActorSprite->getPosition() + addMovePoint);
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
    auto pMiniMapActorNode = (Node*) pMiniMapLayer->getChildByTag(pActorSprite->getTag());
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

bool RogueScene::isMapLayerOver(MapIndex touchPointMapIndex)
{
    if (m_baseMapSize.width <= touchPointMapIndex.x ||
        m_baseMapSize.height <= touchPointMapIndex.y ||
        0 > touchPointMapIndex.x ||
        0 > touchPointMapIndex.y)
    {
        return true;
    }
    return false;
}

#pragma mark
#pragma mark UI関連

void RogueScene::logMessage(const char * pszFormat, ...)
{
    char szBuf[kMaxLogLen+1] = {0};
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, kMaxLogLen, pszFormat, ap);
    va_end(ap);
    
    CCLOG("logMessage: %s", szBuf);
    
    auto pGameLogNode = getChildByTag(RogueScene::kGameLogTag);
    // とりあえず子要素がないなら無理
    if (!pGameLogNode || pGameLogNode->getChildrenCount() <= 0)
    {
        return;
    }
    
    auto pGameLogText = static_cast<LabelTTF*>(pGameLogNode->getChildren().at(0)); // TODO: 1子しかaddしてないから動く。ちゃんとしないと・・・
    if (pGameLogText)
    {
        // TODO: 別クラスにしてログをlistで保持する。デフォルトの表示は1件だけで、center寄せ表示でいいかと
        auto pMessage = String::create(szBuf);
        
        pMessage->append("\n");

        std::string nowString = pGameLogText->getString();
        
        int count = f_r(nowString, '\n');
        
        // 3行まで表示
        if (count >= 3)
        {
            int size = nowString.size();
            unsigned int loc = nowString.find_last_of('\n', size);
            CCLOG("logMessage: loc = %d size = %d", loc, size);
            if (loc != std::string::npos)
            {
                nowString.erase(loc, nowString.size() - loc);
            }
        }
        pMessage->append(nowString);
        pGameLogText->setString(pMessage->getCString());
        pGameLogText->setVerticalAlignment(cocos2d::TextVAlignment::TOP);
        pGameLogText->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        pGameLogText->setPosition(Point(pGameLogText->getFontSize() / 4 + pGameLogText->getContentSize().width / 2, pGameLogNode->getContentSize().height - pGameLogText->getContentSize().height / 2 - pGameLogText->getFontSize() / 4));
    }
}

void RogueScene::showItemList(int showTextIndex)
{
    // いまのところ使用してないindex
    if (showTextIndex <= 0)
    {
        return;
    }
    
    auto winSize = Director::getInstance()->getWinSize();
    
    // モーダルレイヤー作成
    auto pModalLayer = static_cast<ModalLayer*>(this->getChildByTag(RogueScene::kModalTag));
    if (pModalLayer)
    {
        pModalLayer->setVisible(true);
    }
    else
    {
        pModalLayer = ModalLayer::create();
        this->addChild(pModalLayer, RogueScene::zModalIndex, RogueScene::kModalTag);
    }
    
    auto pItemWindowLayer = static_cast<ItemWindowLayer*>(pModalLayer->getChildByTag(RogueScene::kItemListTag));
    if (pItemWindowLayer)
    {
        pItemWindowLayer->reloadItemList();
        pItemWindowLayer->setVisible(true);
    }
    else
    {
        // TODO: アイテムの詳細ウィンドウ（以下のボタン操作のみ可能なモーダルウィンドウ）
            // アイテムを捨てる
            // アイテムを使う
            // アイテムを装備する
            // 閉じる
        pItemWindowLayer = ItemWindowLayer::createWithContentSize(winSize * 0.8);
        pItemWindowLayer->setPosition(Point(winSize.width / 2 - pItemWindowLayer->getContentSize().width / 2,
                                            winSize.height /2 - pItemWindowLayer->getContentSize().height / 2));
        pItemWindowLayer->setItemDropMenuCallback([this](Object* pSender, DropItemSprite::DropItemDto dropItemDto) {
            CCLOG("RogueScene::itemDropMenuCallback");
            
            auto pPlayerSprite = getPlayerActorSprite(1);
            
            // アイテムをマップのプレイヤーの足元に置く
            if (this->tileSetDropMapItem(dropItemDto, pPlayerSprite->getActorMapItem()->mapIndex))
            {
                this->logMessage("%sを床においた。", dropItemDto.name.c_str());
                
                // ターン消費
                this->changeGameStatus(RogueScene::ENEMY_TURN);
            }
            else
            {
                this->logMessage("%sを床におけなかった。", dropItemDto.name.c_str());
            }
            // インベントリは閉じる
            this->hideItemList();
        });
        pItemWindowLayer->setItemUseMenuCallback([this](Object* pSender, DropItemSprite::DropItemDto dropItemDto) {
            CCLOG("RogueScene::itemUseMenuCallback");
            
            auto pPlayerSprite = getPlayerActorSprite(1);
            
            // itemIdで処理してくれるlogicへ
            std::string useMessage = ItemLogic::use(dropItemDto.itemId, pPlayerSprite->getActorDto());
            
            this->logMessage(useMessage.c_str());
//            
//            this->refreshStatus();
            
            // インベントリは閉じる
            this->hideItemList();
            
            // ターン消費
            this->changeGameStatus(RogueScene::ENEMY_TURN);
        });
        pModalLayer->addChild(pItemWindowLayer, RogueScene::zItemListIndex, RogueScene::kItemListTag);
    }
    
    auto rect = Rect(0, 0, 300, 30);
    auto capRect = Rect(0, 0, 300, 30);
    auto pScale9Sprite1 = extension::Scale9Sprite::create("menu_button.png", rect, capRect);
    pScale9Sprite1->setColor(Color3B::RED);
    pScale9Sprite1->setOpacity(192);
    pScale9Sprite1->setContentSize(Size(40, 20));
    auto pScale9Sprite2 = extension::Scale9Sprite::create("menu_button.png", rect, capRect);
    pScale9Sprite2->setColor(Color3B::RED);
    pScale9Sprite2->setOpacity(128);
    pScale9Sprite2->setContentSize(Size(40, 20));
    
    auto pMenuItemSprite = MenuItemSprite::create(pScale9Sprite1, pScale9Sprite2, [this](Object* pSeneder) {
        // hoge
//        pModalLayer->removeFromParentAndCleanup(true);
        this->hideItemList();
    });
    pMenuItemSprite->setPosition(Point(winSize.width - pMenuItemSprite->getContentSize().width / 2, pMenuItemSprite->getContentSize().height / 2));
    
    auto pMenu = Menu::create(pMenuItemSprite, NULL);
    pMenu->setPosition(Point::ZERO);
    
    pModalLayer->addChild(pMenu);
}

void RogueScene::hideItemList()
{
    // モーダルレイヤー非表示にする
    auto pModalLayer = static_cast<ModalLayer*>(this->getChildByTag(RogueScene::kModalTag));
    if (pModalLayer)
    {
        pModalLayer->setVisible(false);
    }
}


bool RogueScene::tileSetEnemyActorMapItem(ActorSprite::ActorDto enemyActorDto, MapIndex mapIndex)
{
    // すでにプレイヤーが置いてある場合は置けない
    if (m_mapManager.getActorMapItem(&mapIndex)->mapDataType != MapDataType::NONE)
    {
        return false;
    }
    auto pTileMapLayer = getChildByTag(RogueScene::kTiledMapTag);
    auto pEnemyLayer = pTileMapLayer->getChildByTag(RogueScene::kTiledMapEnemyBaseTag);
    // TODO: 倒したときのことも考えないといけない 出現数のカウントでOK?
    long enemyCount = pEnemyLayer->getChildrenCount();
    
    ActorMapItem enemyMapItem;
    enemyMapItem.mapDataType = MapDataType::ENEMY;
    enemyMapItem.mapIndex = mapIndex;
    enemyMapItem.seqNo = enemyCount;
    enemyMapItem.moveDist = enemyActorDto.movePoint;
    enemyMapItem.attackDist = enemyActorDto.attackRange;
    enemyMapItem.moveDone = false;
    enemyMapItem.attackDone = false;
    
    auto enemySprite = ActorSprite::createWithActorDto(enemyActorDto);
    enemySprite->setPosition(indexToPoint(enemyMapItem.mapIndex));
    enemySprite->setActorMapItem(enemyMapItem);
    enemySprite->runBottomAction();
    pEnemyLayer->addChild(enemySprite, RogueScene::zTiledMapEnemyBaseIndex, (RogueScene::kTiledMapEnemyBaseTag + enemyMapItem.seqNo));
    
    // マップに追加
    m_mapManager.addActor(enemySprite->getActorMapItem());
    
    // ミニマップも更新
    auto pMiniMapLayer = getChildByTag(kMiniMapTag);
    
    // 敵の位置表示用（同じく1/8サイズ）TODO: 数が多くなるならBatchNodeとかにしないと？
    auto miniMapEnemyLayer = LayerColor::create(Color4B::RED);
    // タイルの1/8サイズ
    miniMapEnemyLayer->setContentSize(m_baseTileSize / 8);
    // 現在位置からPositionを取得して1/8にする
    miniMapEnemyLayer->setPosition(indexToPointNotTileSize(enemySprite->getActorMapItem()->mapIndex) / 8);
    // 移動時に更新できるようにplayerIdをtag管理
    miniMapEnemyLayer->setTag(enemySprite->getTag());
    // add
    pMiniMapLayer->addChild(miniMapEnemyLayer);
    
    return true;
}

bool RogueScene::tileSetDropMapItem(DropItemSprite::DropItemDto dropItemDto, MapIndex mapIndex)
{
    // すでにアイテムが置いてある場合は置けない
    if (m_mapManager.getDropMapItem(&mapIndex)->mapDataType != MapDataType::NONE)
    {
        return false;
    }
    
    auto pTileMapLayer = getChildByTag(RogueScene::kTiledMapTag);
    auto pDropItemLayer = pTileMapLayer->getChildByTag(RogueScene::kTiledMapDropItemBaseTag);
    
    DropMapItem dropMapItem;
    // 一意になるようにする x * 100 + y（100マスないからいけるはず）
    dropMapItem.seqNo = mapIndex.x * 100 + mapIndex.y;
    dropMapItem.itemId = dropItemDto.itemId;
    dropMapItem.mapDataType = MapDataType::MAP_ITEM;
    dropMapItem.moveDist = 0;
    dropMapItem.mapIndex = mapIndex;
    
    auto pDropItemSprite = DropItemSprite::createWithDropItemDto(dropItemDto);
    pDropItemSprite->setDropMapItem(dropMapItem);
    pDropItemSprite->setPosition(indexToPoint(dropMapItem.mapIndex));
    pDropItemLayer->addChild(pDropItemSprite, RogueScene::zTiledMapDropItemBaseIndex, RogueScene::kTiledMapDropItemBaseTag + dropMapItem.seqNo);
    
    // マップに追加
    m_mapManager.addDropItem(pDropItemSprite->getDropMapItem());
    
    // ミニマップも更新
    auto pMiniMapLayer = getChildByTag(kMiniMapTag);
    // TODO: spriteBatchNodeにしないとダメかも？
    auto pMiniMapItemLayer = LayerColor::create(Color4B(51, 204, 255, 255));
    // タイルの1/8サイズ
    pMiniMapItemLayer->setContentSize(m_baseTileSize / 8);
    // 現在位置からPositionを取得して1/8にする
    pMiniMapItemLayer->setPosition(indexToPointNotTileSize(pDropItemSprite->getDropMapItem()->mapIndex) / 8);
    // 移動時に更新できるようにtag管理
    pMiniMapItemLayer->setTag(pDropItemSprite->getTag());

    // add
    pMiniMapLayer->addChild(pMiniMapItemLayer);
    
    return true;
}

void RogueScene::refreshStatus()
{
    auto pStatusBarLayer = getChildByTag(RogueScene::kStatusBarTag);
    auto pStatusText = pStatusBarLayer->getChildren().at(0); // TODO: とりあえず1要素なので。。。
    if (pStatusText)
    {
        // プレイヤー取得
        auto pPlayerSprite = getPlayerActorSprite(1);
        auto pPlayerDto = pPlayerSprite->getActorDto();
        int floor = 1; // TODO: フロア情報
        int gold = 0; // TODO: player情報
        // 作成
        auto pStr = String::createWithFormat(" %2dF Lv%3d HP %3d/%3d 満腹度 %d/%d %10d G", floor, pPlayerDto->lv, pPlayerDto->hitPoint, pPlayerDto->hitPointLimit, pPlayerDto->magicPoint, pPlayerDto->magicPointLimit, gold);
        
        auto pLabelText = static_cast<LabelTTF*>(pStatusText);
        pLabelText->setString(pStr->getCString());
        pLabelText->setPositionX(pLabelText->getContentSize().width / 2);
        
        // TODO: 死亡判定ここで？
        if (pPlayerDto->hitPoint == 0)
        {
            logMessage("%sは死亡した。", pPlayerDto->name.c_str());
        }
        if (pPlayerDto->magicPoint == 0)
        {
            logMessage("%sは空腹で倒れた。", pPlayerDto->name.c_str());
        }
    }
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
#pragma mark 汎用

ActorSprite* RogueScene::getPlayerActorSprite(int seqNo)
{
    return (ActorSprite*)getChildByTag(RogueScene::kActorBaseTag + seqNo);
}

ActorSprite* RogueScene::getEnemyActorSprite(int seqNo)
{
    auto pEnemyMapLayer = getChildByTag(RogueScene::kTiledMapTag)->getChildByTag(RogueScene::TiledMapTag::kTiledMapEnemyBaseTag);
    return (ActorSprite*)pEnemyMapLayer->getChildByTag(RogueScene::TiledMapTag::kTiledMapEnemyBaseTag + seqNo);
}

ItemWindowLayer* RogueScene::getItemWindowLayer()
{
    auto pModalLayer = getChildByTag(RogueScene::kModalTag);
    return static_cast<ItemWindowLayer*>(pModalLayer->getChildByTag(RogueScene::kItemListTag));
}

#pragma mark
#pragma mark その他

int GetRandom(int min,int max)
{
	return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}

