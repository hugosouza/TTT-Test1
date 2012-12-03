#ifndef _TTT_H_
#define _TTT_H_

#include <vector>

#include "cocos2d.h"
#include "VisibleRect.h"

const uint16_t markSize = 70;
const float lineWidth = 10;

USING_NS_CC;
using namespace std;

class TicTacToe;

enum MARK_STATE {
    MARK_STATE_EMPTY,
    MARK_STATE_X,
    MARK_STATE_O
};

class Mark : public CCSprite, public CCTargetedTouchDelegate {
public:
    Mark(TicTacToe* b, int16_t x = 0, int16_t y = 0);
    virtual ~Mark(void);

    CCRect rect();
    bool initWithTexture(CCTexture2D* aTexture);
    static Mark* markWithTexture(CCTexture2D* aTexture, TicTacToe* board);

    virtual void onEnter();
    virtual void onExit();
    bool containsTouchLocation(CCTouch* touch);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    //virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    //virtual CCObject* copyWithZone(CCZone *pZone);

    //virtual void touchDelegateRetain();
    //virtual void touchDelegateRelease();
    CCPoint getCoordsForPosition(int16_t x, int16_t y);
private:
    int16_t x, y;
    TicTacToe* board;
    MARK_STATE state;
};

class TicTacToe : public CCLayer {
public:
    TicTacToe(int16_t lineWidth, int16_t markSize);
    virtual void draw();
    void createScene();
    int16_t getMarkSize();
    int16_t getLineWidth();
    CCPoint getCenter();
    CCPoint getBoard00();
    //void menuCallback(CCObject* pSender);
private:
    CCPoint center;
    CCPoint board00;
    float halfMark;
    Mark* m1;
    int16_t lineWidth;
    int16_t markSize;
    vector<Mark*> markList;

};

#endif