//
//  MenuItemSelectLabelSprite.cpp
//  NovelGame
//
//  Created by kyokomi on 2013/09/07.
//
//

#include "MenuItemSelectLabelSprite.h"

USING_NS_CC;

MenuItemSelectLabelSprite* MenuItemSelectLabelSprite::createWithLabelSprite(const std::string& pszFileName, const std::string& string, const std::string& fontName, float fontSize, Color3B normalColor, Color3B selectedColor, Color3B disabledColor, const ccMenuCallback& callback)
{
    MenuItemSelectLabelSprite *pRet = new MenuItemSelectLabelSprite();
    if (pRet && pRet->initWithLabelSprite(pszFileName, string, fontName, fontSize, normalColor, selectedColor, disabledColor, callback))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool MenuItemSelectLabelSprite::initWithLabelSprite(const std::string& pszFileName, const std::string& string, const std::string& fontName, float fontSize, Color3B normalColor, Color3B selectedColor, Color3B disabledColor, const ccMenuCallback& callback)
{
    LabelSprite* normalLabelSprite = LabelSprite::createWithLabel(pszFileName, string, fontName, fontSize, normalColor);
    if (normalLabelSprite == NULL)
    {
        return false;
    }
    LabelSprite* selectedLabelSprite = LabelSprite::createWithLabel(pszFileName, string, fontName, fontSize, selectedColor);
    if (selectedLabelSprite == NULL)
    {
        return false;
    }
    LabelSprite* disabledLabelSprite = LabelSprite::createWithLabel(pszFileName, string, fontName, fontSize, disabledColor);
    if (disabledLabelSprite == NULL)
    {
        return false;
    }
    
    if (!MenuItemSprite::initWithNormalSprite(normalLabelSprite, selectedLabelSprite, disabledLabelSprite, callback))
    {
        return false;
    }
    m_labelText = string;
    
    return true;
}

void MenuItemSelectLabelSprite::setNovelText(const char *string)
{
    m_labelText = string;
    ((LabelSprite*) this->getNormalImage())->setLabelText(string);
    ((LabelSprite*) this->getSelectedImage())->setLabelText(string);
    ((LabelSprite*) this->getDisabledImage())->setLabelText(string);
}



