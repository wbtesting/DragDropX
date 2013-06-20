//
//  CCDragableLayer.cpp
//  DragDropX
//
//  Created by wxf on 13/6/13.
//
//

#include "CCDragableLayer.h"

#include "CCDirector.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "touch_dispatcher/CCTouch.h"

#include "support/CCPointExtension.h"

#include "cocoa/CCInteger.h"

#include <vector>
#include <stdarg.h>

using namespace std;

NS_CC_BEGIN

static std::vector<unsigned int> ccarray_to_std_vector(CCArray* pArray)
{
    std::vector<unsigned int> ret;
    CCObject* pObj;
    CCARRAY_FOREACH(pArray, pObj)
    {
        CCInteger* pInteger = (CCInteger*)pObj;
        ret.push_back((unsigned int)pInteger->getValue());
    }
    return ret;
}

enum
{
    kDefaultPadding =  5,
};

//
//CCDragableLayer
//

CCDragableLayer* CCDragableLayer::createWithArray(CCArray* pArrayOfItems)
{
    CCDragableLayer *pRet = new CCDragableLayer();
    if (pRet && pRet->initWithArray(pArrayOfItems))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}




bool CCDragableLayer::init()
{
    return initWithArray(NULL);
}

bool CCDragableLayer::initWithArray(CCArray* pArrayOfItems)
{
    if (CCLayer::init())
    {
        setTouchPriority(kCCDragableLayerHandlerPriority);
        setTouchMode(kCCTouchesOneByOne);
        setTouchEnabled(true);
        
        m_bEnabled = true;
        // menu in the center of the screen
        CCSize s = CCDirector::sharedDirector()->getWinSize();
        
        this->ignoreAnchorPointForPosition(false);
        setAnchorPoint(ccp(0.5f, 0.5f));
        this->setContentSize(s);
        
        setPosition(ccp(s.width/2, s.height/2));
        
        if (pArrayOfItems != NULL)
        {
            int z=0;
            CCObject* pObj = NULL;
            CCARRAY_FOREACH(pArrayOfItems, pObj)
            {
                /*
                CCDragableItem* item = (CCDragableItem*)pObj;
                item->setAnchorPoint(ccp(0.5f,0.5f));
                this->addChild(item, z);
                z++;
                 */
            }
        }
        

        m_pSelectedItem = NULL;
        m_eState = kCCDragableLayerStateWaiting;
        
        /*
        // enable cascade color and opacity on menus
        setCascadeColorEnabled(true);
        setCascadeOpacityEnabled(true);
        */
        
        return true;
    }
    return false;
}


/*
 * override add:
 */
void CCDragableLayer::addChild(CCNode * child)
{
    CCLayer::addChild(child);
}

void CCDragableLayer::addChild(CCNode * child, int zOrder)
{
    CCLayer::addChild(child, zOrder);
}

void CCDragableLayer::addChild(CCNode * child, int zOrder, int tag)
{
    CCAssert( dynamic_cast<CCDragableItem*>(child) != NULL, "Menu only supports MenuItem objects as children");
    CCLayer::addChild(child, zOrder, tag);
}

void CCDragableLayer::onExit()
{
    if (m_eState == kCCDragableLayerStateTrackingTouch)
    {
        if (m_pSelectedItem)
        {
            //m_pSelectedItem->unselected();
            m_pSelectedItem = NULL;
        }
        
        m_eState = kCCDragableLayerStateWaiting;
    }
    
    CCLayer::onExit();
}

void CCDragableLayer::removeChild(CCNode* child, bool cleanup)
{
    CCDragableItem *pMenuItem = dynamic_cast<CCDragableItem*>(child);
    CCAssert(pMenuItem != NULL, "Menu only supports MenuItem objects as children");
    
    if (m_pSelectedItem == pMenuItem)
    {
        m_pSelectedItem = NULL;
    }
    
    CCNode::removeChild(child, cleanup);
}



NS_CC_END