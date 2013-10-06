//
//  SRPGScene.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2013/10/05.
//
//

#ifndef __Cocos2dxSRPGQuest__SRPGScene__
#define __Cocos2dxSRPGQuest__SRPGScene__

#include "cocos2d.h"

class SRPGScene : public cocos2d::Layer
{
private:
    cocos2d::Point m_pStartPoint;
    cocos2d::Point m_pDelta;
    bool m_touched;
    
    bool checkFlick();
protected:
    enum kTag {
        kSRPGMapLayerTag = 1,
    };
public:
    SRPGScene();
    ~SRPGScene();
    
    virtual bool init();
    
    static cocos2d::Scene* scene();
    
    CREATE_FUNC(SRPGScene);
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);
    
    virtual void update(float delta);
    
    virtual void onEnterTransitionDidFinish();

    void touchUnSchedule(float time);
};

#endif /* defined(__Cocos2dxSRPGQuest__SRPGScene__) */

