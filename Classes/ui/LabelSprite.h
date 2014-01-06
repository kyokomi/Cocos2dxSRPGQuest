//
//  CCLabelSprite.h
//  NovelGame
//
//  Created by kyokomi on 2013/09/07.
//
//

#ifndef __NovelGame__LabelSprite__
#define __NovelGame__LabelSprite__

#include "cocos2d.h"

class LabelSprite : public cocos2d::Sprite
{
    cocos2d::LabelTTF* m_pLabel;
public:
    LabelSprite();
    
    virtual bool initWithLabel(const std::string& pszFileName, const std::string& string, const std::string& fontName, float fontSize, cocos2d::Color3B color);
    static LabelSprite* createWithLabel(const std::string& pszFileName, const std::string& string, const std::string& fontName, float fontSize, cocos2d::Color3B color);
    void setLabelText(const char *string);
    
};

#endif /* defined(__NovelGame__CCLabelSprite__) */
