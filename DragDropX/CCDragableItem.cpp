//
//  CCDragableItem.cpp
//  DragDropX
//
//  Created by wxf on 13/6/13.
//
//

#include "CCDragableItem.h"
#include "support/CCPointExtension.h"
#include "CCSprite.h"

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
    return CCDragableItem::create(normalSprite, normalSprite, normalSprite, NULL, NULL);
}

CCDragableItem * CCDragableItem::create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite)
{
    return CCDragableItem::create(normalSprite, selectedSprite, disabledSprite, NULL, NULL);
}

CCDragableItem * CCDragableItem::create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector)
{
    return CCDragableItem::create(normalSprite, selectedSprite, NULL, target, selector);
}

CCDragableItem * CCDragableItem::create(CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, CCObject *target, SEL_MenuHandler selector)
{
    CCDragableItem *pRet = new CCDragableItem();
    pRet->initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector);
    pRet->autorelease();
    return pRet;
}

bool CCDragableItem::initWithNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector)
{

    setNormalImage(normalSprite);
    setSelectedImage(selectedSprite);
    setDisabledImage(disabledSprite);
    
    if(m_pNormalImage)
    {
        this->setContentSize(m_pNormalImage->getContentSize());
    }
    

    return true;
}

/**
 @since v0.99.5
 */
void CCDragableItem::selected()
{

    
    if (m_pNormalImage)
    {
        if (m_pDisabledImage)
        {
            m_pDisabledImage->setVisible(false);
        }
        
        if (m_pSelectedImage)
        {
            m_pNormalImage->setVisible(false);
            m_pSelectedImage->setVisible(true);
        }
        else
        {
            m_pNormalImage->setVisible(true);
        }
    }
}

void CCDragableItem::unselected()
{

    if (m_pNormalImage)
    {
        m_pNormalImage->setVisible(true);
        
        if (m_pSelectedImage)
        {
            m_pSelectedImage->setVisible(false);
        }
        
        if (m_pDisabledImage)
        {
            m_pDisabledImage->setVisible(false);
        }
    }
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

bool CCDragableItem::isSelected()
{
    return m_bSelected;
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



NS_CC_END

