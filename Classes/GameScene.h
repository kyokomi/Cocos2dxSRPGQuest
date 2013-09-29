//
//  GameScene.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/09/23.
//
//

#ifndef __Cocos2dxSRPGQuest__GameScene__
#define __Cocos2dxSRPGQuest__GameScene__

#include "cocos2d.h"

class GameScene : public cocos2d::Layer
{
private:
    cocos2d::Point m_pStartPoint;
    cocos2d::Point m_pDelta;
    
protected:
    enum kTag {
        kTiledMapTag = 1,
        kActorTag = 100,
        
        kActionBottom = 1000,
        kActionLeft   = 1001,
        kActionRight  = 1002,
        kActionTop    = 1003
    };
public:
    GameScene();
    ~GameScene();
    
    virtual bool init();
    
    static cocos2d::Scene* scene();
    
    CREATE_FUNC(GameScene);
    
    virtual bool ccTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void ccTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void ccTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void ccTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);
    
    virtual void update(float delta);
    
    virtual void onEnterTransitionDidFinish();
};

#endif /* defined(__Cocos2dxSRPGQuest__GameScene__) */
