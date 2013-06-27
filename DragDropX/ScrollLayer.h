//
//  ScrollLayer.h
//  DragDropX
//
//  Created by Shawn on 27/6/13.
//
//

#ifndef __DragDropX__ScrollLayer__
#define __DragDropX__ScrollLayer__

#include "cocos2d.h"

class ScrollLayer : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(ScrollLayer);
};

#endif /* defined(__DragDropX__ScrollLayer__) */
