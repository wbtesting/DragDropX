//
//  CCDragableLayer.h
//  DragDropX
//
//  Created by wxf on 13/6/13.
//
//

#ifndef __DragDropX__CCDragableLayer__
#define __DragDropX__CCDragableLayer__

#include "CCDragableItem.h"
#include "layers_scenes_transitions_nodes/CCLayer.h"

NS_CC_BEGIN

/**
 * @addtogroup GUI
 * @{
 * @addtogroup menu
 * @{
 */

/** @brief A CCDragableLaye
* 
* Features and Limitation:
*  - You can add DragableItem objects in runtime using addChild:
*  - But the only accepted children are DragableItem objects
*/
class CC_DLL CCDragableLayer : public CCLayerRGBA
{
    /** whether or not the menu will receive events */
    bool m_bEnabled;
    
public:
    CCDragableLayer() : m_pSelectedItem(NULL) {}
    virtual ~CCDragableLayer(){}
    
    /** creates an empty CCDragableLaye */
    static CCDragableLayer* create();
    
    /** creates a CCDragableLaye with CCDragableItem objects */
    static CCDragableLayer* create(CCDragableItem* item, ...);
    
    /** creates a CCDragableLayer with a CCArray of CCDragableItem objects */
    static CCDragableLayer* createWithArray(CCArray* pArrayOfItems);
    
    /** creates a CCDragableLayer with it's item, then use addChild() to add
     * other items. It is used for script, it can't init with undetermined
     * number of variables.
     */
    static CCDragableLayer* createWithItem(CCDragableItem* item);
    
    /** creates a CCDragableLayer with CCDragableItem objects */
    static CCDragableLayer* createWithItems(CCDragableItem *firstItem, va_list args);
    
    /** initializes an empty CCDragableLayer */
    bool init();
    
    /** initializes a CCDragableLayer with a NSArray of CCDragableItem objects */
    bool initWithArray(CCArray* pArrayOfItems);
    
    

    
    //super methods
    virtual void addChild(CCNode * child);
    virtual void addChild(CCNode * child, int zOrder);
    virtual void addChild(CCNode * child, int zOrder, int tag);
    virtual void removeChild(CCNode* child, bool cleanup);
    
   
    /** FIXME: seems do not need this method
     override onExit
    */
    virtual void onExit();

    virtual bool isEnabled() { return m_bEnabled; }
    virtual void setEnabled(bool value) { m_bEnabled = value; };
    
protected:
    CCDragableItem* itemForTouch(CCTouch * touch);
    tCCDragableLayerState m_eState;
    CCDragableItem *m_pSelectedItem;
};

// end of GUI group
/// @}
/// @}

NS_CC_END
#endif /* defined(__DragDropX__CCDragableLayer__) */
