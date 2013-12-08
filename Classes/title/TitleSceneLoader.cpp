//
//  TitleSceneLoader.cpp
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/09/29.
//
//

#include "TitleSceneLoader.h"

Scene* TitleSceneLoader::createScene()
{
    // CocosBuilderのファイルを読み込みゲーム画面を生成する
    cocosbuilder::NodeLoaderLibrary* nodeLoaderLibrary = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
    nodeLoaderLibrary->registerNodeLoader("TitleScene", TitleSceneLoader::loader());
    
    cocosbuilder::CCBReader* ccbReader = new cocosbuilder::CCBReader(nodeLoaderLibrary);
    Node* node = ccbReader->readNodeGraphFromFile("TitleScene.ccbi");
    
    // シーンを用意し、ゲーム画面を設置する
    Scene* pScene = Scene::create();
    if (node != NULL)
    {
        pScene->addChild(node);
    }
    ccbReader->release();
    
    return pScene;
}