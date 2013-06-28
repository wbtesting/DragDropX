//
//  ScrollLayer.cpp
//  DragDropX
//
//  Created by Shawn on 27/6/13.
//
//

#include "ScrollLayer.h"


using namespace cocos2d;


CCScene* ScrollLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ScrollLayer *layer = ScrollLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}



// on "init" you need to initialize your instance
bool ScrollLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    //////////////////////////////
    // 2. add scrollview
    
    
    /////////////////////////////
    // 3. add your codes below...
    
    // add a label shows "Hello World"
    // create and initialize a label
    // ask director the window size
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCArray *images = CCArray::create(CCString::create("bird.png"), CCString::create("cat.png"), CCString::create("dog.png"), CCString::create("turtle.png"), NULL);

    CCLayer *layer = CCLayer::create();
    for (int i = 0; i < images->count(); i++) {
        CCString *imageName = (CCString *)images->objectAtIndex(i);
        CCSprite *sprite = CCSprite::create(imageName->getCString());
        float offsetFraction = ((float)(i+1))/(images->count()+1);
            //item->setPosition(winSize.width*offsetFraction, winSize.height/2);
        sprite->setPosition(ccp(20,winSize.height*offsetFraction));
        layer->addChild(sprite);
        //movableItems->addObject(item);
        
    }
    this->addChild(layer);
    return true;
}