//
//  CCDragableItem.cpp
//  DragDropX
//
//  Created by wxf on 13/6/13.
//
//

#include "CCDragableItem.h"

#include "CCDirector.h"
#include "touch_dispatcher/CCTouchDispatcher.h"


#include "support/CCPointExtension.h"


#define	kCCDragableItemMovedItemZOrder  401
#define kCCDragableItemStillItemZOrder  400

NS_CC_BEGIN

//static unsigned int _globalFontSize = kCCItemSize;
static std::string _globalFontName = "Marker Felt";
//static bool _globalFontNameRelease = false;

const unsigned int    kCurrentItem = 0xc0c05001;
const unsigned int    kZoomActionTag = 0xc0c05002;

const unsigned int    kNormalTag = 0x1;
const unsigned int    kSelectedTag = 0x2;
const unsigned int    kDisableTag = 0x3;
//
// CCDragableItem
//



CCNode * CCDragableItem::getNormalImage()
{
    return m_pNormalImage;
}

void CCDragableItem::setNormalImage(CCNode* pImage)
{
    if (pImage != m_pNormalImage)
    {
        if (pImage)
        {
            addChild(pImage, 0, kNormalTag);
            pImage->setAnchorPoint(ccp(0, 0));
        }
        
        if (m_pNormalImage)
        {
            removeChild(m_pNormalImage, true);
        }
        
        m_pNormalImage = pImage;
        this->setContentSize(m_pNormalImage->getContentSize());
        this->updateImagesVisibility();
    }
}

CCNode * CCDragableItem::getSelectedImage()
{
    return m_pSelectedImage;
}

void CCDragableItem::setSelectedImage(CCNode* pImage)
{
    if (pImage != m_pNormalImage)
    {
        if (pImage)
        {
            addChild(pImage, 0, kSelectedTag);
            pImage->setAnchorPoint(ccp(0, 0));
        }
        
        if (m_pSelectedImage)
        {
            removeChild(m_pSelectedImage, true);
        }
        
        m_pSelectedImage = pImage;
        this->updateImagesVisibility();
    }
}

CCNode * CCDragableItem::getDisabledImage()
{
    return m_pDisabledImage;
}

void CCDragableItem::setDisabledImage(CCNode* pImage)
{
    if (pImage != m_pNormalImage)
    {
        if (pImage)
        {
            addChild(pImage, 0, kDisableTag);
            pImage->setAnchorPoint(ccp(0, 0));
        }
        
        if (m_pDisabledImage)
        {
            removeChild(m_pDisabledImage, true);
        }
        
        m_pDisabledImage = pImage;
        this->updateImagesVisibility();
    }
}



//
//CCDragableItem
//

CCDragableItem * CCDragableItem::create(CCNode* normalSprite)
{
    CCDragableItem *pRet = new CCDragableItem();
    pRet->initWithNormalSprite(normalSprite, NULL, NULL, true);
    pRet->autorelease();
    return pRet;
}



bool CCDragableItem::initWithNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, bool dragable)
{

    setNormalImage(normalSprite);
    setSelectedImage(selectedSprite);
    setDisabledImage(disabledSprite);
    setAnchorPoint(ccp(0.5,0.5));
    if(m_pNormalImage)
    {
        this->setContentSize(m_pNormalImage->getContentSize());
    }
    this->registerWithTouchDispatcher();

    return true;
}


void CCDragableItem::setEnabled(bool bEnabled)
{
    if( m_bEnabled != bEnabled )
    {
        this->updateImagesVisibility();
    }
}



bool CCDragableItem::isEnabled()
{
    return m_bEnabled;
}

CCRect CCDragableItem::rect()
{
    return CCRectMake( m_obPosition.x - m_obContentSize.width * m_obAnchorPoint.x,
                      m_obPosition.y - m_obContentSize.height * m_obAnchorPoint.y,
                      m_obContentSize.width, m_obContentSize.height);
}


// Helper
void CCDragableItem::updateImagesVisibility()
{
    if (m_bEnabled)
    {
        if (m_pNormalImage)   m_pNormalImage->setVisible(true);
        if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
        if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
    }
    else
    {
        if (m_pDisabledImage)
        {
            if (m_pNormalImage)   m_pNormalImage->setVisible(false);
            if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
            if (m_pDisabledImage) m_pDisabledImage->setVisible(true);
        }
        else
        {
            if (m_pNormalImage)   m_pNormalImage->setVisible(true);
            if (m_pSelectedImage) m_pSelectedImage->setVisible(false);
            if (m_pDisabledImage) m_pDisabledImage->setVisible(false);
        }
    }
}



//DragableItem - Touch Events

void CCDragableItem::setHandlerPriority(int newPriority)
{
    CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    pDispatcher->setPriority(newPriority, this);
}

void CCDragableItem::registerWithTouchDispatcher()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCDragableItemHandlerPriority, true);
}

bool CCDragableItem::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    //CCLOG("touch began");
    
    bool  isTouchInside = this->isTouchInside(touch);
    
    if (isTouchInside) {
        m_eState = kCCDragableItemStateTrackingTouch;
    }
    return isTouchInside;
}

void CCDragableItem::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCDragableItemStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");
    m_eState = kCCDragableItemStateWaiting;
    this->getParent()->reorderChild(this, kCCDragableItemStillItemZOrder);
    
    CCPoint nodePoint = this->convertTouchToNodeSpace(touch);
    if (m_pDelegate != NULL)
    {
        m_pDelegate->itemDidDragedToPosition(this, this->convertToWorldSpace(nodePoint));
    }
}

void CCDragableItem::ccTouchCancelled(CCTouch *touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCDragableItemStateTrackingTouch, "[Menu ccTouchCancelled] -- invalid state");
    m_eState = kCCDragableItemStateWaiting;
}

void CCDragableItem::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCDragableItemStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
    
    CCPoint nodePoint = this->convertTouchToNodeSpace(touch);
    this->setPosition(this->convertToWorldSpace(nodePoint));
    this->getParent()->reorderChild(this, kCCDragableItemMovedItemZOrder);
}



//CCDragableItem* CCDragableItem::itemForTouch(CCTouch *touch)
bool CCDragableItem::isTouchInside(CCTouch *touch)
{
    CCPoint touchLocation = touch->getLocation();
    CCPoint local = this->convertToNodeSpace(touchLocation);
    CCRect r = this->rect();
    r.origin = CCPointZero;

    return  r.containsPoint(local);
}


NS_CC_END

