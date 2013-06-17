//
//  CCDragableItem.cpp
//  DragDropX
//
//  Created by Shawn on 13/6/13.
//
//

#include "CCDragableItem.h"
#include "support/CCPointExtension.h"

NS_CC_BEGIN

static unsigned int _globalFontSize = kCCItemSize;
static std::string _globalFontName = "Marker Felt";
static bool _globalFontNameRelease = false;

const unsigned int    kCurrentItem = 0xc0c05001;
const unsigned int    kZoomActionTag = 0xc0c05002;

const unsigned int    kNormalTag = 0x1;
const unsigned int    kSelectedTag = 0x2;
const unsigned int    kDisableTag = 0x3;
//
// CCDragableItem
//

CCDragableItem* CCDragableItem::create()
{
    return CCDragableItem::create(NULL, NULL);
}

CCDragableItem* CCDragableItem::create(CCObject *rec, SEL_MenuHandler selector)
{
    CCDragableItem *pRet = new CCDragableItem();
    pRet->initWithTarget(rec, selector);
    pRet->autorelease();
    return pRet;
}

bool CCDragableItem::initWithTarget(CCObject *rec, SEL_MenuHandler selector)
{
    setAnchorPoint(ccp(0.5f, 0.5f));
    m_pListener = rec;
    m_pfnSelector = selector;
    m_bEnabled = true;
    m_bSelected = false;
    return true;
}

CCDragableItem::~CCDragableItem()
{
    unregisterScriptTapHandler();
}

void CCDragableItem::selected()
{
    m_bSelected = true;
}

void CCDragableItem::unselected()
{
    m_bSelected = false;
}

void CCDragableItem::registerScriptTapHandler(int nHandler)
{
    unregisterScriptTapHandler();
    m_nScriptTapHandler = nHandler;
    LUALOG("[LUA] Add CCDragableItem script handler: %d", m_nScriptTapHandler);
}

void CCDragableItem::unregisterScriptTapHandler(void)
{
    if (m_nScriptTapHandler)
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nScriptTapHandler);
        LUALOG("[LUA] Remove CCDragableItem script handler: %d", m_nScriptTapHandler);
        m_nScriptTapHandler = 0;
    }
}
/*
void CCDragableItem::activate()
{
    if (m_bEnabled)
    {
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);
        }
        
        if (kScriptTypeNone != m_eScriptType)
        {
            CCScriptEngineManager::sharedManager()->getScriptEngine()->executeMenuItemEvent(this);
        }
    }
}
*/
void CCDragableItem::setEnabled(bool enabled)
{
    m_bEnabled = enabled;
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

void CCDragableItem::setTarget(CCObject *rec, SEL_MenuHandler selector)
{
    m_pListener = rec;
    m_pfnSelector = selector;
}


NS_CC_END

