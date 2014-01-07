//
//  ItemLogic.h
//  Cocos2dxSRPGQuest
//
//  Created by kyokomi on 2014/01/07.
//
//

#ifndef __Cocos2dxSRPGQuest__ItemLogic__
#define __Cocos2dxSRPGQuest__ItemLogic__

#include "cocos2d.h"
#include "ActorSprite.h"

enum ItemType {
    HP_RECOVER_VALUE   = 100,
    HP_RECOVER_PERCENT = 101,
    MP_RECOVER_VALUE   = 200,
    MP_RECOVER_PERCENT = 201,
};

class ItemLogic
{
private:
    
public:
    static std::string use(int itemId, ActorSprite::ActorDto* pUseActorDto);
};

#endif /* defined(__Cocos2dxSRPGQuest__ItemLogic__) */
