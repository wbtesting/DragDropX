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
#include "CCFloat.h"
//#include "LuaUtil.h"
#define ScriptType 0
NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup menu
 * @{
 */

typedef enum  {
	kCCDragableItemStateWaiting,
	kCCDragableItemStateTrackingTouch
} tCCDragableItemState;

enum {
	//* priority used by the menu for the event handler
    //* use the same value as CCMenu
	kCCDragableItemHandlerPriority = -128,
};


class CCDragableItem;

class CCDragableItemDelegate
{
public:
    virtual ~CCDragableItemDelegate() {};
    
    /**
     *  if client provide movedImage, then will drag the movedImage
     *  use setMov
     */

    //virtual CCNode *movedNodeForItem(CCDragableItem *item) = 0;
    
    /**
     *  NOTICE: onDragBegan() provide a CCDragableItem instead of CCNode,
     *  because client need to provide moveImage if nessesary in onDragBegan
     */
    virtual void onDragBegan(CCDragableItem *item) = 0;
    /**
     *  For the 2 api below:
     *  if client provided movedImage, node will be the movedImage
     *  if not, node will be the CCDragableItem
     *  all the position is in world space.

     */
    virtual void onDragging(CCDragableItem *item,CCFloat *x, CCFloat *y) = 0 ;
    virtual void onDragEnded(CCDragableItem *item ,CCFloat *x, CCFloat *y) = 0;
};




/** @brief CCDragableItem base class
 *
 *  Inspire from CCMenuItemSprite
 */
class CC_DLL CCDragableItem : public CCNode,public CCTouchDelegate
{
    /** the image used when the item is not selected */
    CC_PROPERTY(CCNode*, m_pNormalImage, NormalImage);
    /** the image used when the item is selected */
    CC_PROPERTY(CCNode*, m_pMovedImage, MovedImage);
public:
    CCDragableItem()
    :m_bEnabled(true)
    //,m_bDragable(true)
    ,m_pDelegate(NULL)
    ,m_pNormalImage(NULL)
    ,m_pMovedImage(NULL)
    {}
    
    static CCDragableItem * create(CCNode* normalSprite);
    bool initWithNormalSprite(CCNode* normalSprite);
    
    /** Returns the outside box */
    CCRect rect();


    virtual bool isEnabled();
    virtual void setEnabled(bool value);

    CCDragableItemDelegate* getDelegate() { return m_pDelegate; }
    void setDelegate(CCDragableItemDelegate* pDelegate) { m_pDelegate = pDelegate; }
#if ScriptType == 1
    
    /**
     * 设置、获取事件委托（脚本）
     */
    CC_SYNTHESIZE(cocos2d::CCObject*, scriptDelegate, ScriptDelegate);
    
#endif
    
protected:
    virtual void updateImagesVisibility();
    
    //touch event
    void setHandlerPriority(int newPriority);
    bool isTouchInside(CCTouch *touch);
    virtual void registerWithTouchDispatcher();
    
    /**
     @brief For phone event handle functions
     */
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);


    //bool m_bDragable; no used for now
    bool m_bEnabled;
    tCCDragableItemState m_eState;
    CCDragableItemDelegate* m_pDelegate;
};



NS_CC_END
#endif /* defined(__DragDropX__CCDragableItem__) */
