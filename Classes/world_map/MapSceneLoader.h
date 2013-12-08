//
//  MacpSceneLoader.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/09/29.
//
//

#ifndef __Cocos2dxSRPGQuest__MacpSceneLoader__
#define __Cocos2dxSRPGQuest__MacpSceneLoader__

#include "MapScene.h"

#include "extensions/cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"

class MapSceneLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(MapSceneLoader, loader);
    
    static Scene* scene();
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(MapScene);
};

#endif /* defined(__Cocos2dxSRPGQuest__MacpSceneLoader__) */
