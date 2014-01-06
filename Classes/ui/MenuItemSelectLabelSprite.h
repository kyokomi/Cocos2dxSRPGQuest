//
//  MenuItemSelectLabelSprite.h
//  NovelGame
//
//  Created by kyokomi on 2013/09/07.
//
//

#ifndef __NovelGame__MenuItemSelectLabelSprite__
#define __NovelGame__MenuItemSelectLabelSprite__

#include "cocos2d.h"
#include "LabelSprite.h"

class MenuItemSelectLabelSprite : public cocos2d::MenuItemSprite
{
public:
    virtual bool initWithLabelSprite(const std::string& pszFileName, const std::string& string, const std::string& fontName, float fontSize, cocos2d::Color3B normalColor, cocos2d::Color3B selectedColor, cocos2d::Color3B disabledColor, const cocos2d::ccMenuCallback& callback);
    
    static MenuItemSelectLabelSprite* createWithLabelSprite(const std::string& pszFileName, const std::string& string, const std::string& fontName, float fontSize, cocos2d::Color3B normalColor, cocos2d::Color3B selectedColor, cocos2d::Color3B disabledColor, const cocos2d::ccMenuCallback& callback);
    
    void setNovelText(const char *string);
    
    std::string m_labelText;
};

#endif /* defined(__NovelGame__MenuItemSelectLabelSprite__) */
