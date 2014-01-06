#include "TableViewTestScene.h"
#include "CustomTableViewCell.h"

#include "AppMacros.h"

USING_NS_CC;
USING_NS_CC_EXT;

TableViewTestLayer::TableViewTestLayer()
:m_textArray(std::vector<std::string>())
{

}

TableViewTestLayer* TableViewTestLayer::createWithTextArray(std::vector<std::string> textArray, Size contentSize)
{
    TableViewTestLayer *pRet = new TableViewTestLayer();
    if (pRet && pRet->init(textArray, contentSize))
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
bool TableViewTestLayer::init(std::vector<std::string> textArray, Size contentSize)
{
    if ( !CCLayerColor::init() )
    {
        return false;
    }
    m_textArray = textArray;

    this->setContentSize(contentSize);
    
//	Size winSize = Director::getInstance()->getWinSize();
    
	TableView* tableView = TableView::create(this, Size(contentSize.width, contentSize.height * 0.9));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
    tableView->setPosition(Point(tableView->getPositionX(), contentSize.height * 0.1));
	tableView->setDelegate(this);
//    tableView->setBounceable(false); // スクロールオーバー
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::BOTTOM_UP);
    tableView->setTag(kTag_TableView);
	this->addChild(tableView);
	tableView->reloadData();

    this->setColor(Color3B::BLACK);
    this->setOpacity(128);
    
    return true;
}

void TableViewTestLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
//    CCLOG("cell touched at index: %i", cell->getIdx());
}

Size TableViewTestLayer::tableCellSizeForIndex(TableView *table, long idx)
{
//	Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = getContentSize();
    return Size(contentSize.width, contentSize.height / 5);
}

TableViewCell* TableViewTestLayer::tableCellAtIndex(TableView *table, long idx)
{
//    Size winSize = Director::getInstance()->getWinSize();
    Size contentSize = getContentSize();
    std::string text = "hogehoge";
    
    CCLOG("idx = %ld size = %ld", idx, m_textArray.size());
    
    if (m_textArray.size() > 0 && idx < m_textArray.size())
    {
        text = m_textArray[idx];
    }
    String *pTextString = String::createWithFormat("[%ld] : %s", idx, text.c_str());
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new CustomTableViewCell();
        cell->autorelease();
        
        LayerColor * textLayer = CCLayerColor::create(Color4B(0, 0, 0, 255 * 0.7),
                                                        contentSize.width * 0.9, contentSize.height / 5 * 0.9);
        textLayer->setPosition(contentSize.width * 0.05, contentSize.height / 5 * 0.05);
        textLayer->setTag(kTag_TextLayer);
        
        // 本文テキスト
        int baseFontSize = 10; // TODO: あとで
        
        LabelTTF* textLabel = CCLabelTTF::create(pTextString->getCString(), "Arial", baseFontSize);
        //textLabel->setAnchorPoint(Point::ZERO);
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
        pTextLabel->setString(pTextString->getCString());
    }


    return cell;
}

long TableViewTestLayer::numberOfCellsInTableView(TableView *table)
{
    return m_textArray.size();
}

void TableViewTestLayer::makeTextLog(std::vector<std::string> textArray)
{
    m_textArray = textArray;
    static_cast<TableView*>(this->getChildByTag(kTag_TableView))->reloadData();
}



