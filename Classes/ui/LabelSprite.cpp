//
//  LabelSprite.cpp
//  NovelGame
//
//  Created by kyokomi on 2013/09/07.
//
//

#include "LabelSprite.h"

USING_NS_CC;

LabelSprite::LabelSprite()
:m_pLabel(NULL)
{
}

LabelSprite* LabelSprite::createWithLabel(const std::string& pszFileName, const std::string& string, const std::string& fontName, float fontSize, cocos2d::Color3B color)
{
    LabelSprite *pRet = new LabelSprite();
    if (pRet && pRet->initWithLabel(pszFileName, string, fontName, fontSize, color))
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

bool LabelSprite::initWithLabel(const std::string& pszFileName, const std::string& string, const std::string& fontName, float fontSize, Color3B color)
{
    if (!Sprite::initWithFile(pszFileName))
    {
        return false;
    }
    m_pLabel = LabelTTF::create(string, fontName, fontSize);
    if (m_pLabel == NULL)
    {
        return false;
    }
    m_pLabel->setPosition(Point(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5));
    this->addChild(m_pLabel);
    
    this->setColor(color);
    this->setOpacity(128);
    return true;
}

void LabelSprite::setLabelText(const char *string)
{
    this->m_pLabel->setString(string);
}


