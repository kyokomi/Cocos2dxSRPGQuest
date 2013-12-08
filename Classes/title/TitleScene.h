//
//  TitleScene.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/09/29.
//
//

#ifndef __Cocos2dxSRPGQuest__TitleScene__
#define __Cocos2dxSRPGQuest__TitleScene__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TitleScene : public Layer
, public cocosbuilder::CCBSelectorResolver /** CocosBuilderからメニューとかコントロールボタンを取得するのに必要 */
//, public CCBMemberVariableAssigner /** CocosBuilderから変数を読み込むときに必要 */
{
//    // ccb
//    ControlButton* mStartConrolButton;
    // CCBSelector
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Object* pTarget, const char* pSelectorName);
    virtual Control::Handler onResolveCCBCCControlSelector(Object * pTarget, const char* pSelectorName);
//    // CCBMember
//    virtual bool onAssignCCBMemberVariable(Object* pTarget, const char* pMemberVariableName, Node* pNode);

    // Startボタンのタップイベント用
    void tappedStartButton(Object * pTarget, Control::EventType pControlEventType);
public:
    // コンストラクタ、デストラクタ
    TitleScene();
    virtual ~TitleScene();
    
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    
    // createメソッド
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(TitleScene, create);
};

#endif /* defined(__Cocos2dxSRPGQuest__TitleScene__) */
