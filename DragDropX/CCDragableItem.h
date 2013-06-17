//
//  CCDragableItem.h
//  DragDropX
//
//  Created by Shawn on 13/6/13.
//
//

#ifndef __DragDropX__CCDragableItem__
#define __DragDropX__CCDragableItem__


#include "base_nodes/CCNode.h"
#include "CCProtocols.h"
#include "cocoa/CCArray.h"

NS_CC_BEGIN

class CCSprite;
class CCSpriteFrame;
#define kCCItemSize 32

/**
 * @addtogroup GUI
 * @{
 * @addtogroup menu
 * @{
 */

/** @brief CCDragableItem base class
 *
 *  Subclass CCDragableItem (or any subclass) to create your custom CCDragableItem objects.
 */
class CC_DLL CCDragableItem : public CCNode
{
protected:
    /** whether or not the item is selected
     @since v0.8.2
     */
    bool m_bSelected;
    bool m_bEnabled;
    
public:
    CCDragableItem()
    : m_bSelected(false)
    , m_bEnabled(false)
    , m_pListener(NULL)
    , m_pfnSelector(NULL)
    , m_nScriptTapHandler(0)
    {}
    virtual ~CCDragableItem();
    
    /** Creates a CCDragableItem with no target/selector */
    static CCDragableItem* create();
    /** Creates a CCDragableItem with a target/selector */
    static CCDragableItem* create(CCObject *rec, SEL_MenuHandler selector);
    /** Initializes a CCDragableItem with a target/selector */
    bool initWithTarget(CCObject *rec, SEL_MenuHandler selector);
    /** Returns the outside box */
    CCRect rect();
    /** Activate the item */
    //virtual void activate();
    /** The item was selected (not activated), similar to "mouse-over" */
    virtual void selected();
    /** The item was unselected */
    virtual void unselected();
    
    /** Register menu handler script function */
    virtual void registerScriptTapHandler(int nHandler);
    virtual void unregisterScriptTapHandler(void);
    int getScriptTapHandler() { return m_nScriptTapHandler; };
    
    virtual bool isEnabled();
    //@note: It's 'setIsEnable' in cocos2d-iphone.
    virtual void setEnabled(bool value);
    virtual bool isSelected();
    
    
    /** set the target/selector of the menu item*/
    void setTarget(CCObject *rec, SEL_MenuHandler selector);
    
protected:
    CCObject*       m_pListener;
    SEL_MenuHandler    m_pfnSelector;
    int             m_nScriptTapHandler;
};

NS_CC_END
#endif /* defined(__DragDropX__CCDragableItem__) */
