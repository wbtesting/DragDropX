//
//  CCDragableItem.h
//  DragDropX
//
//  Created by wxf on 13/6/13.
//
//

#ifndef __DragDropX__CCDragableItem__
#define __DragDropX__CCDragableItem__

#include "base_nodes/CCNode.h"
#include "CCProtocols.h"
#include "cocoa/CCArray.h"
#include "touch_dispatcher/CCTouchDelegateProtocol.h"

NS_CC_BEGIN


#define kCCItemSize 32

/**
 * @addtogroup GUI
 * @{
 * @addtogroup menu
 * @{
 */

typedef enum  {
	kCCDragableLayerStateWaiting,
	kCCDragableLayerStateTrackingTouch
} tCCDragableLayerState;

enum {
	//* priority used by the menu for the event handler
    //* use the same value as CCDragableLaye
	kCCDragableLayerHandlerPriority = -128,
};


class CCDragableItem;

class CCDragableItemDelegate
{
public:
    virtual ~CCDragableItemDelegate() {};
    
    /*
     *  position is in world space
     */
    virtual void itemDidDragedToPosition(CCDragableItem* item,CCPoint point) = 0;
};




/** @brief CCDragableItem base class
 *
 *  Modify from CCMenuItemSprite
 */
class CC_DLL CCDragableItem : public CCNode,public CCTouchDelegate
{
    /** the image used when the item is not selected */
    CC_PROPERTY(CCNode*, m_pNormalImage, NormalImage);
    /** the image used when the item is selected */
    CC_PROPERTY(CCNode*, m_pSelectedImage, SelectedImage);
    /** the image used when the item is disabled */
    CC_PROPERTY(CCNode*, m_pDisabledImage, DisabledImage);
public:
    CCDragableItem()
    :m_bEnabled(true)
    ,m_bDragable(true)
    ,m_pDelegate(NULL)
    ,m_pNormalImage(NULL)
    ,m_pSelectedImage(NULL)
    ,m_pDisabledImage(NULL)
    {}
    
    static CCDragableItem * create(CCNode* normalSprite);
    bool initWithNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, bool dragable);
    
    /** Returns the outside box */
    CCRect rect();

    virtual bool isEnabled();
    //@note: It's 'setIsEnable' in cocos2d-iphone.
    virtual void setEnabled(bool value);

    CCDragableItemDelegate* getDelegate() { return m_pDelegate; }
    void setDelegate(CCDragableItemDelegate* pDelegate) { m_pDelegate = pDelegate; }
protected:
    virtual void updateImagesVisibility();
    
    //touch event

    
    void setHandlerPriority(int newPriority);
    virtual void registerWithTouchDispatcher();
    
    /**
     @brief For phone event handle functions
     */
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    
    
    /** whether or not the item is selected  @since v0.8.2
     */
    bool isTouchInside(CCTouch *touch);
    
    bool m_bDragable;
    bool m_bEnabled;
    tCCDragableLayerState m_eState;
    CCDragableItemDelegate* m_pDelegate;
};



NS_CC_END
#endif /* defined(__DragDropX__CCDragableItem__) */
