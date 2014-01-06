#ifndef __TABLEVIEWTESTSCENE_H__
#define __TABLEVIEWTESTSCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class TableViewTestLayer :
public cocos2d::LayerColor,
public cocos2d::extension::TableViewDataSource,
public cocos2d::extension::TableViewDelegate
{
protected:
    
    enum kTag
    {
        kTag_TextLayer = 1,
        kTag_TextLabel,
        kTag_TableView,
    };
    
public:
    // 一覧タップ時のコールバック
    typedef std::function<void(Object*, long idx)> TableCellTouchedCallback;
    void setCallback(const TableCellTouchedCallback& callback);
    
    // 初期
    virtual bool init(std::list<std::string> itemList, cocos2d::Size contentSize);
    TableViewTestLayer();
    static TableViewTestLayer* createWithTextArray(std::list<std::string> itemList, cocos2d::Size contentSize);
    
    // デリゲート関連
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, long idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, long idx);
    virtual long numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
    // リスト作成
    void makeItemList(std::list<std::string> itemList);
    
private:
    // TODO: とりあえず文字列だけ（アイコンの表示とかでimgResIdがほしい）typedef作るか
    std::list<std::string> m_itemList;
    
    // コールバック
    TableCellTouchedCallback m_callback;
};

#endif // __TABLEVIEWTESTSCENE_H__
