//
//  DragLayer.cpp
//  DragDropX
//
//  Created by wxf on 17/6/13.
//
//

#include "DragLayer.h"


using namespace cocos2d;


CCScene* DragDrop::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    DragDrop *layer = DragDrop::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}



// on "init" you need to initialize your instance
bool DragDrop::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    // ask director the window size
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCArray *images = CCArray::create(CCString::create("bird.png"), CCString::create("cat.png"), CCString::create("dog.png"), CCString::create("turtle.png"), NULL);
    //CCArray *movableItems = new CCArray();
    
    CCLayer *layer = CCLayer::create();
    for (int i = 0; i < images->count(); i++) {
        CCString *imageName = (CCString *)images->objectAtIndex(i);
        CCSprite *sprite = CCSprite::create(imageName->getCString());
        float offsetFraction = ((float)(i+1))/(images->count()+1);
        CCDragableItem *item = CCDragableItem::create(sprite);
        //item->setPosition(winSize.width*offsetFraction, winSize.height/2);
        item->setPosition(20,winSize.height*offsetFraction);
        item->setDelegate(this);
        layer->addChild(item);
        //movableItems->addObject(item);
    }
    
    //CCDragableLayer *layer = CCDragableLayer::createWithArray(movableItems);
    this->addChild(layer, 1);
    
    
    return true;
}

//cocos2d::CCNode *DragDrop::movedNodeForItem(cocos2d::CCDragableItem *item)
cocos2d::CCNode *DragDrop::movedNode()
{
    //return NULL;
    CCSprite *sprite = CCSprite::create("cat.png");
    sprite->runAction(CCRotateTo::create(0.1,0));
    CCRotateBy *rotLeft = CCRotateBy::create(0.1, -4.0);
    CCRotateBy *rotCenter = CCRotateBy::create(0.1, 0.0);
    CCRotateBy *rotRight = CCRotateBy::create(0.1, 4.0);
    CCSequence *rotSeq = CCSequence::create(rotLeft,rotCenter,rotRight,rotCenter,NULL);
    sprite->runAction(CCRepeatForever::create(rotSeq));
    return sprite;
}


void DragDrop::onDragBegan(cocos2d::CCDragableItem *item)
{
    CCLog("DragDrop::item  %p Did Touched ", item);
    //item->setMovedImage(this->movedNode());
}
void DragDrop::onDragging(cocos2d::CCNode *node, cocos2d::CCPoint point)
{
    CCLog("DragDrop::item  %p move To Position (%f,%f) ", node, point.x, point.y);
}

void DragDrop::onDragEnded(cocos2d::CCNode *node, cocos2d::CCPoint point)
{
    node->removeFromParentAndCleanup(true);
    CCLog("DragDrop::item  %p DidDragedToPosition (%f,%f) ", node, point.x, point.y);
}



