//
//  MacpSceneLoader.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/09/29.
//
//

#include "MapSceneLoader.h"

Scene* MapSceneLoader::scene()
{
    // CocosBuilderのファイルを読み込みゲーム画面を生成する
    NodeLoaderLibrary* nodeLoaderLibrary = NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    nodeLoaderLibrary->registerNodeLoader("MapScene", MapSceneLoader::loader());
    
    CCBReader* ccbReader = new CCBReader(nodeLoaderLibrary);
    Node* node = ccbReader->readNodeGraphFromFile("MapScene.ccbi");
    
    // シーンを用意し、ゲーム画面を設置する
    Scene* pScene = Scene::create();
    if (node != NULL)
    {
        pScene->addChild(node);
    }
    ccbReader->release();
    
    return pScene;
}