#ifndef __TABLEVIEWTESTSCENE_H__
#define __TABLEVIEWTESTSCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class TableViewTestLayer :
public cocos2d::LayerColor,
public cocos2d::extension::TableViewDataSource,
public cocos2d::extension::TableViewDelegate
{
    std::vector<std::string> m_textArray;
    
    enum kTag
    {
        kTag_TextLayer = 1,
        kTag_TextLabel,
        kTag_TableView,
    };
    
public:
    virtual bool init(std::vector<std::string> textArray, cocos2d::Size contentSize);
   
    TableViewTestLayer();
//	void toExtensionsMainLayer(cocos2d::CCObject *sender);
    
//    CREATE_FUNC(TableViewTestLayer);
    
    static TableViewTestLayer* createWithTextArray(std::vector<std::string> textArray, cocos2d::Size contentSize);
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, long idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, long idx);
    
    virtual long numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
    void makeTextLog(std::vector<std::string> textArray);
};

#endif // __TABLEVIEWTESTSCENE_H__
