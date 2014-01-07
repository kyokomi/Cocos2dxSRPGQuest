//
//  ModalLayer.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2014/01/07.
//
//

#ifndef __Cocos2dxSRPGQuest__ModalLayer__
#define __Cocos2dxSRPGQuest__ModalLayer__

#include "cocos2d.h"

class ModalLayer : public cocos2d::LayerColor
{
public:
    virtual bool init();
//    void menuCloseCallback(cocos2d::Object* pSender);
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    CREATE_FUNC(ModalLayer);
    
//    virtual void onTouchMoved(Touch *touch, Event *unused_event);
//    virtual void onTouchEnded(Touch *touch, Event *unused_event);
//    virtual void onTouchCancelled(Touch *touch, Event *unused_event);
};

#endif /* defined(__Cocos2dxSRPGQuest__ModalLayer__) */
