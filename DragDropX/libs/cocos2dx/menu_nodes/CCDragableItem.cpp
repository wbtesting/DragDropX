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
/*
const unsigned int    kCurrentItem = 0xc0c05001;
const unsigned int    kZoomActionTag = 0xc0c05002;
*/
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


CCNode * CCDragableItem::getMovedImage()
{
    return m_pMovedImage;
}

void CCDragableItem::setMovedImage(CCNode* pImage)
{
    if (pImage != m_pMovedImage)
    {
        
        m_pMovedImage = pImage;
        
        if (m_pMovedImage) {
            pImage->setAnchorPoint(ccp(0, 0));
            removeChild(m_pMovedImage,true);
            addChild(pImage, 0, kSelectedTag);
        }
        this->updateImagesVisibility();
    }
}


//
//CCDragableItem
//

CCDragableItem * CCDragableItem::create(CCNode* normalSprite)
{
    CCDragableItem *pRet = new CCDragableItem();
    pRet->initWithNormalSprite(normalSprite, true);
    pRet->autorelease();
    return pRet;
}



bool CCDragableItem::initWithNormalSprite(CCNode* normalSprite, bool dragable)
{
    setNormalImage(normalSprite);
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
    CCNode *moveNode = this;
    if (this->getMovedImage()) {
        moveNode = this->getMovedImage();
    }
    return CCRectMake( moveNode->getPosition().x - moveNode->getContentSize().width * moveNode->getAnchorPoint().x,
                      moveNode->getPosition().y - moveNode->getContentSize().height * moveNode->getAnchorPoint().y,
                      moveNode->getContentSize().width,
                      moveNode->getContentSize().height);
    /*
    return CCRectMake( m_obPosition.x - m_obContentSize.width * m_obAnchorPoint.x,
                      m_obPosition.y - m_obContentSize.height * m_obAnchorPoint.y,
                      m_obContentSize.width, m_obContentSize.height);*/
}


// Helper
void CCDragableItem::updateImagesVisibility()
{
    if (m_bEnabled)
    {
        if (m_eState == kCCDragableItemStateTrackingTouch) { //moving
            if (m_pNormalImage)   m_pNormalImage->setVisible(true);
            if (m_pMovedImage) m_pMovedImage->setVisible(true);
        }else { 
            if (m_pNormalImage)   m_pNormalImage->setVisible(true);
            if (m_pMovedImage) m_pMovedImage->setVisible(false);
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
    
    if (isTouchInside) {
        if (m_pDelegate != NULL) {
            //setMovedImage(m_pDelegate->onD(this));
            if (m_pDelegate) m_pDelegate->onDragBegan(this);
        }
    }
    /*
    if (m_pMovedImage) {
        if (m_pDelegate) m_pDelegate->onDragBegan(m_pMovedImage);
    }else
     */
    // can only be the item here,so 
    
    
    return isTouchInside;
}

void CCDragableItem::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
    CC_UNUSED_PARAM(touch);
    CC_UNUSED_PARAM(event);
    CCAssert(m_eState == kCCDragableItemStateTrackingTouch, "[Menu ccTouchEnded] -- invalid state");
    m_eState = kCCDragableItemStateWaiting;

    
    //CCPoint nodePoint = this->convertTouchToNodeSpace(touch);
    if (m_pDelegate != NULL){

        this->getParent()->reorderChild(this, kCCDragableItemStillItemZOrder);
        
        if (m_pMovedImage) {
            m_pDelegate->onDragEnded(m_pMovedImage, this->convertToWorldSpace(m_pMovedImage->getPosition()));
            //setMovedImage(NULL);
        }else{
            m_pDelegate->onDragEnded(this,this->convertToWorldSpace(this->getPosition()));
        }
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
    if (m_pMovedImage) {
        CCPoint anchorPoint = this->getAnchorPointInPoints();
        m_pMovedImage->setPosition(ccpSub(nodePoint, anchorPoint));
        if (m_pDelegate != NULL)
            m_pDelegate->onDragging(m_pMovedImage, this->convertToWorldSpace(m_pMovedImage->getPosition()));
        
    }else{
        this->setPosition(this->convertToWorldSpace(nodePoint));
        if (m_pDelegate != NULL)
            m_pDelegate->onDragging(this, this->convertToWorldSpace(this->getPosition()));
    }
    this->updateImagesVisibility();
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

