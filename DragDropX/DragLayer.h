//
//  DragLayer.h
//  DragDropX
//
//  Created by wxf on 17/6/13.
//
//

#ifndef __DragDropX__DragLayer__
#define __DragDropX__DragLayer__

#include "cocos2d.h"
#include "CCDragableItem.h"


class DragDrop : public cocos2d::CCLayer, public cocos2d::CCDragableItemDelegate
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(DragDrop);
    //static DragDrop *create();
    CCNode *movedNode();
    //virtual ~CCDragableItemDelegate() {}
    //virtual cocos2d::CCNode *movedNodeForItem(cocos2d::CCDragableItem *item);
    virtual void onDragBegan(cocos2d::CCDragableItem *item);
    virtual void onDragging(cocos2d::CCDragableItem *item, cocos2d::CCFloat *x, cocos2d::CCFloat *y);
    virtual void onDragEnded(cocos2d::CCDragableItem *item, cocos2d::CCFloat *x, cocos2d::CCFloat *y);
    
    cocos2d::CCSprite *icon;
};

#endif /* defined(__DragDropX__DragLayer__) */
