#include "TableViewTestLayer.h"
#include "CustomTableViewCell.h"

#include "AppMacros.h"

USING_NS_CC;
USING_NS_CC_EXT;

TableViewTestLayer::TableViewTestLayer()
:m_itemList(std::list<std::string>()),
m_callback(nullptr)
{

}

TableViewTestLayer* TableViewTestLayer::createWithTextArray(std::list<std::string> itemList, Size contentSize)
{
    TableViewTestLayer *pRet = new TableViewTestLayer();
    if (pRet && pRet->init(itemList, contentSize))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

// on "init" you need to initialize your instance
bool TableViewTestLayer::init(std::list<std::string> itemList, Size contentSize)
{
    if ( !LayerColor::init() )
    {
        return false;
    }
    m_itemList = itemList;

    this->setContentSize(contentSize);
    
//	Size winSize = Director::getInstance()->getWinSize();
    
	TableView* tableView = TableView::create(this, Size(contentSize.width, contentSize.height * 0.9));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
    tableView->setPosition(Point(tableView->getPositionX(), contentSize.height * 0.1));
	tableView->setDelegate(this);
//    tableView->setBounceable(false); // スクロールオーバー
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableView->setTag(kTag_TableView);
	this->addChild(tableView);
	tableView->reloadData();

    this->setColor(Color3B::BLACK);
    this->setOpacity(128);
    
    return true;
}

void TableViewTestLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
    if (m_callback)
    {
        m_callback(this, cell->getIdx());
    }
}

Size TableViewTestLayer::tableCellSizeForIndex(TableView *table, long idx)
{
//	Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = getContentSize();
    return Size(contentSize.width, contentSize.height / 5);
}

TableViewCell* TableViewTestLayer::tableCellAtIndex(TableView *table, long idx)
{
    Size contentSize = getContentSize();
    std::string text;
    
    long listSize = m_itemList.size();
    if (listSize > 0 && idx < listSize)
    {
        auto it = m_itemList.begin();
        std::advance(it, idx);
        std::string itemName = *it;
        text = itemName;
        m_itemList.end();
    }
    CCLOG("idx = %ld size = %ld text = %s", idx, listSize, text.c_str());
    
    auto textString = StringUtils::format("[%ld] : %s", idx, text.c_str());
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new CustomTableViewCell();
        cell->autorelease();
        
        LayerColor * textLayer = LayerColor::create(Color4B(0, 0, 0, 255 * 0.7),
                                                        contentSize.width * 0.9, contentSize.height / 5 * 0.9);
        textLayer->setPosition(contentSize.width * 0.05, contentSize.height / 5 * 0.05);
        textLayer->setTag(kTag_TextLayer);
        
        // 本文テキスト
        int baseFontSize = 10;
        
        LabelTTF* textLabel = LabelTTF::create(textString, GAME_FONT(baseFontSize), baseFontSize);
        textLabel->setColor(Color3B::WHITE);
        textLabel->setPosition(Point(baseFontSize + textLabel->getContentSize().width / 2, textLayer->getContentSize().height / 2));
        textLabel->setTag(kTag_TextLabel);

        textLabel->setVerticalAlignment(cocos2d::TextVAlignment::TOP);
        textLabel->setHorizontalAlignment(TextHAlignment::LEFT);
        textLayer->addChild(textLabel);
        
        cell->addChild(textLayer);
    }
    else
    {
        LayerColor* pTextLayer = static_cast<LayerColor*>(cell->getChildByTag(kTag_TextLayer));
        LabelTTF* pTextLabel = static_cast<LabelTTF*>(pTextLayer->getChildByTag(kTag_TextLabel));
        pTextLabel->setString(textString);
    }

    return cell;
}

long TableViewTestLayer::numberOfCellsInTableView(TableView *table)
{
    return m_itemList.size();
}

#pragma mark
#pragma mark 要素設定とか

void TableViewTestLayer::makeItemList(std::list<std::string> itemList)
{
    m_itemList = itemList;
    static_cast<TableView*>(this->getChildByTag(kTag_TableView))->reloadData();
}

#pragma mark
#pragma mark コールバック関連

void TableViewTestLayer::setCallback(const TableCellTouchedCallback &callback)
{
    m_callback = callback;
}


