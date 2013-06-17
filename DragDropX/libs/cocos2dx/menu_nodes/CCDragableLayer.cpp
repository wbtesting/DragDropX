//
//  CCDragableLayer.cpp
//  DragDropX
//
//  Created by Shawn on 13/6/13.
//
//

#include "CCDragableLayer.h"
#include "CCDirector.h"
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

CCDragableLayer* CCDragableLayer::create()
{
    return CCDragableLayer::create(NULL, NULL);
}

CCDragableLayer * CCDragableLayer::create(CCDragableItem* item, ...)
{
    va_list args;
    va_start(args,item);
    
    CCDragableLayer *pRet = CCDragableLayer::createWithItems(item, args);
    
    va_end(args);
    
    return pRet;
}

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

CCDragableLayer* CCDragableLayer::createWithItems(CCDragableItem* item, va_list args)
{
    CCArray* pArray = NULL;
    if( item )
    {
        pArray = CCArray::create(item, NULL);
        CCDragableItem *i = va_arg(args, CCDragableItem*);
        while(i)
        {
            pArray->addObject(i);
            i = va_arg(args, CCDragableItem*);
        }
    }
    
    return CCDragableLayer::createWithArray(pArray);
}

CCDragableLayer* CCDragableLayer::createWithItem(CCDragableItem* item)
{
    return CCDragableLayer::create(item, NULL);
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
        
        this->ignoreAnchorPointForPosition(true);
        setAnchorPoint(ccp(0.5f, 0.5f));
        this->setContentSize(s);
        
        setPosition(ccp(s.width/2, s.height/2));
        
        if (pArrayOfItems != NULL)
        {
            int z=0;
            CCObject* pObj = NULL;
            CCARRAY_FOREACH(pArrayOfItems, pObj)
            {
                CCDragableItem* item = (CCDragableItem*)pObj;
                this->addChild(item, z);
                z++;
            }
        }
        
        //    [self alignItemsVertically];
        m_pSelectedItem = NULL;
        m_eState = kCCDragableLayerStateWaiting;
        
        // enable cascade color and opacity on menus
        setCascadeColorEnabled(true);
        setCascadeOpacityEnabled(true);
        
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
            m_pSelectedItem->unselected();
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



//Menu - Events


void CCDragableLayer::setHandlerPriority(int newPriority)
{
    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    //pDispatcher->setPriority(newPriority, this);
}

void CCDragableLayer::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    //pDirector->getTouchDispatcher()->addTargetedDelegate(this, this->getTouchPriority(), true);
}

bool CCDragableLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    if (m_eState != kCCDragableLayerStateWaiting || ! m_bVisible || !m_bEnabled)
    {
        return false;
    }
    
    for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    
    m_pSelectedItem = this->itemForTouch(touch);
    if (m_pSelectedItem)
    {
        m_eState = kCCDragableLayerStateTrackingTouch;
        m_pSelectedItem->selected();
        return true;
    }
    return false;
}

void CCDragableLayer::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCDragableLayerStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");
    if (m_pSelectedItem)
    {
        m_pSelectedItem->unselected();
        //m_pSelectedItem->activate();
    }
    m_eState = kCCDragableLayerStateWaiting;
}

void CCDragableLayer::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCDragableLayerStateTrackingTouch, "[Menu ccTouchCancelled] -- invalid state");
    if (m_pSelectedItem)
    {
        m_pSelectedItem->unselected();
    }
    m_eState = kCCDragableLayerStateWaiting;
}

void CCDragableLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCDragableLayerStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
    CCDragableItem *currentItem = this->itemForTouch(touch);
    if (currentItem != m_pSelectedItem)
    {
        if (m_pSelectedItem)
        {
            m_pSelectedItem->unselected();
        }
        m_pSelectedItem = currentItem;
        if (m_pSelectedItem)
        {
            m_pSelectedItem->selected();
        }
    }
}



CCDragableItem* CCDragableLayer::itemForTouch(CCTouch *touch)
{
    CCPoint touchLocation = touch->getLocation();
    
    if (m_pChildren && m_pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(m_pChildren, pObject)
        {
            CCDragableItem* pChild = dynamic_cast<CCDragableItem*>(pObject);
            if (pChild && pChild->isVisible() && pChild->isEnabled())
            {
                CCPoint local = pChild->convertToNodeSpace(touchLocation);
                CCRect r = pChild->rect();
                r.origin = CCPointZero;
                
                if (r.containsPoint(local))
                {
                    return pChild;
                }
            }
        }
    }
    
    return NULL;
}

NS_CC_END