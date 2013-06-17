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
 *  Modify from CCMenuItemSprite
 */
class CC_DLL CCDragableItem : public CCNode
{
    /** the image used when the item is not selected */
    CC_PROPERTY(CCNode*, m_pNormalImage, NormalImage);
    /** the image used when the item is selected */
    CC_PROPERTY(CCNode*, m_pSelectedImage, SelectedImage);
    /** the image used when the item is disabled */
    CC_PROPERTY(CCNode*, m_pDisabledImage, DisabledImage);
public:
    CCDragableItem()
    :m_bEnabled(false)
    ,m_pNormalImage(NULL)
    ,m_pSelectedImage(NULL)
    ,m_pDisabledImage(NULL)
    {}
    
    static CCDragableItem * create(CCNode* normalSprite);
    /** creates a menu item with a normal, selected and disabled image*/
    static CCDragableItem * create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite = NULL);
    /** creates a menu item with a normal and selected image with target/selector */
    static CCDragableItem * create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector);
    /** creates a menu item with a normal,selected  and disabled image with target/selector */
    static CCDragableItem * create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector);
    
    /** initializes a menu item with a normal, selected  and disabled image with target/selector */
    bool initWithNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector);
    
    /** Returns the outside box */
    CCRect rect();
    /** Activate the item */
    //virtual void activate();
    /** The item was selected (not activated), similar to "mouse-over" */
    virtual void selected();
    /** The item was unselected */
    virtual void unselected();
    
        
    virtual bool isEnabled();
    //@note: It's 'setIsEnable' in cocos2d-iphone.
    virtual void setEnabled(bool value);
    virtual bool isSelected();
 
    
protected:
    virtual void updateImagesVisibility();

    /** whether or not the item is selected  @since v0.8.2
     */
    bool m_bSelected;
    bool m_bEnabled;

};
NS_CC_END
#endif /* defined(__DragDropX__CCDragableItem__) */
