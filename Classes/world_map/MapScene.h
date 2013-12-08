//
//  MapScene.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/09/29.
//
//

#ifndef __Cocos2dxSRPGQuest__MapScene__
#define __Cocos2dxSRPGQuest__MapScene__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;

class MapScene : public Layer
, public cocosbuilder::CCBSelectorResolver /** CocosBuilderからメニューとかコントロールボタンを取得するのに必要 */
{
    // CCBSelector
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Object* pTarget, const char* pSelectorName);
    virtual Control::Handler onResolveCCBCCControlSelector(Object * pTarget, const char* pSelectorName);
    
    void tappedMenuItem1(Object * pTarget);
    void tappedMenuItem2(Object * pTarget);
    void tappedMenuItem3(Object * pTarget);
public:
    // コンストラクタ、デストラクタ
    MapScene();
    virtual ~MapScene();
    
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    
    // createメソッド
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(MapScene, create);
};

#endif /* defined(__Cocos2dxSRPGQuest__MapScene__) */
