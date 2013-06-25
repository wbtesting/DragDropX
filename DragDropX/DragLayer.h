//
//  DragLayer.h
//  DragDropX
//
//  Created by Shawn on 17/6/13.
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
  
    //virtual ~CCDragableItemDelegate() {}
    virtual cocos2d::CCNode *movedNodeForItem(cocos2d::CCDragableItem *item);
    virtual void nodeDidTouched(cocos2d::CCNode *node);
    virtual void nodeMoveToPosition(cocos2d::CCNode *node,cocos2d::CCPoint point)  ;
    virtual void nodeDidDragToPosition(cocos2d::CCNode *node,cocos2d::CCPoint point);
};

#endif /* defined(__DragDropX__DragLayer__) */
