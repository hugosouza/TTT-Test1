#ifndef _TTT_H_
#define _TTT_H_

#include <vector>
#include <algorithm>

#include "cocos2d.h"
#include "VisibleRect.h"

const uint16_t markSize = 70;
const float lineWidth = 10;

#define BOARD_UPDATED "BOARD_UPDATED"

USING_NS_CC;
using namespace std;

class TicTacToe;

enum MARK_STATE {
    MARK_STATE_EMPTY,
    MARK_STATE_X,
    MARK_STATE_O,
};

enum LINE_DIRECTION {
    DIR_VERTICAL,
    DIR_DIAGONAL_RIGHT,
    DIR_HORIZONTAL,
    DIR_DIAGONAL_LEFT,
    DIR_END,
};

class Mark : public CCSprite, public CCTargetedTouchDelegate {
public:
    Mark(TicTacToe* b, int16_t x = 0, int16_t y = 0);
    ~Mark();

    CCRect rect();
    bool initWithTexture(CCTexture2D* aTexture);
    static Mark* markWithTexture(CCTexture2D* aTexture, TicTacToe* board, int16_t x = -1, int16_t y = -1);

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
    MARK_STATE getState();
    int16_t getX();
    int16_t getY();
private:
    int16_t x, y;
    TicTacToe* board;
    MARK_STATE state;
};

class TicTacToe : public CCLayer {
public:
    TicTacToe(int16_t lineWidth, int16_t markSize);
    ~TicTacToe();
    virtual void draw();
    void createScene();
    int16_t getMarkSize();
    int16_t getLineWidth();
    CCPoint getCenter();
    CCPoint getBoard00();
    void setLastState(MARK_STATE state);
    MARK_STATE getLastState();
    void drawWinLine(int16_t x, int16_t y, LINE_DIRECTION d, Mark* m);
private:
    CCPoint center;
    CCPoint board00;
    float halfMark;
    Mark* m1;
    int16_t lineWidth;
    int16_t markSize;
    vector<vector<Mark*>> markList;
    MARK_STATE lastState;
    void boardUpdate(CCObject* obj);
    MARK_STATE getStateForPos(int16_t x, int16_t y);
    bool checkForWinner(CCObject* obj);
    int16_t checkFor3Marks(std::vector<MARK_STATE>);
    std::vector<MARK_STATE> getLineState(int16_t x, int16_t y, LINE_DIRECTION d);
    bool foundWinner;
    CCPoint winnerStart, winnerEnd;
};

#endif