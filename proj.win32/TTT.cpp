#include "TTT.h"

const char* TTTempty = "empty.png";
const char* TTTx = "x.png";
const char* TTTo = "O.png";

TicTacToe::TicTacToe(int16_t lw, int16_t ms) : lineWidth(lw), markSize(ms) {
    halfMark = (markSize/2);
    center.x = (VisibleRect::rightBottom().x - VisibleRect::leftBottom().x)/2;
    center.y = (VisibleRect::leftTop().y - VisibleRect::leftBottom().y)/2;
    board00.x = center.x-halfMark-markSize;
    board00.y = center.y-halfMark-markSize-lineWidth;
    lastState = MARK_STATE_EMPTY;
    foundWinner = false;

    CCLog("center.x: %.02f, center.y: %.02f\n", center.x, center.y);

    CCTexture2D* markEmptyTexture1 = CCTextureCache::sharedTextureCache()->addImage(TTTempty);
    //m1 = Mark::markWithTexture(markEmptyTexture1, this);
    //m1->setPosition(m1->getCoordsForPosition(2,1));
    //addChild(m1);

    Mark* m = Mark::markWithTexture(markEmptyTexture1, this);
    //m->setPosition(m->getCoordsForPosition(1,1));
    //addChild(m);
    //markList.push_back(m);

    //
    for (uint16_t i = 1; i <= 3; i++) {
        markList.push_back(std::vector<Mark*>());
    }

    for (uint16_t y = 1; y <= 3; y++) {
        for (uint16_t x = 1; x <= 3; x++) {
            Mark* m = Mark::markWithTexture(markEmptyTexture1, this, x-1, y-1);
            m->setPosition(m->getCoordsForPosition(x,y));
            addChild(m);
            markList[x-1].push_back(m);
        }
    }

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(TicTacToe::boardUpdate), BOARD_UPDATED, NULL);

    //ccDrawLine(ccp(center.x-halfMark-markSize-lineWidth, VisibleRect::leftTop().y), ccp(center.x-halfMark-markSize-lineWidth, VisibleRect::leftBottom().y));
    //ccDrawLine(ccp(VisibleRect::leftTop().x, center.y+halfMark+markSize+lineWidth), ccp(VisibleRect::rightTop().x, center.y+halfMark+markSize+lineWidth));
}

TicTacToe::~TicTacToe() {
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, BOARD_UPDATED);
}

int16_t TicTacToe::getMarkSize() {
    return markSize;
}

int16_t TicTacToe::getLineWidth() {
    return lineWidth;
}

CCPoint TicTacToe::getCenter() {
    return center;
}

CCPoint TicTacToe::getBoard00() {
    return board00;
}


void TicTacToe::draw() {
    CHECK_GL_ERROR_DEBUG();
    glLineWidth(lineWidth);
    ccDrawColor4B(0,0,0,255);

    ccDrawLine(
        ccp(center.x-halfMark-lineWidth, center.y-halfMark-markSize-lineWidth),
        ccp(center.x-halfMark-lineWidth, center.y+halfMark+markSize+lineWidth)
     );
    ccDrawLine(
        ccp(center.x+halfMark, center.y-halfMark-markSize-lineWidth),
        ccp(center.x+halfMark, center.y+halfMark+markSize+lineWidth)
     );
    ccDrawLine(
        ccp(center.x-halfMark-markSize, center.y-halfMark-lineWidth),
        ccp(center.x+halfMark+markSize, center.y-halfMark-lineWidth)
     );
    ccDrawLine(
        ccp(center.x-halfMark-markSize, center.y+halfMark),
        ccp(center.x+halfMark+markSize, center.y+halfMark)
     );

    glLineWidth(1);
    ccDrawLine(ccp(center.x, VisibleRect::leftTop().y), ccp(center.x, VisibleRect::leftBottom().y));
    ccDrawLine(ccp(VisibleRect::leftTop().x, center.y), ccp(VisibleRect::rightTop().x, center.y));


    glLineWidth(1);
    ccDrawColor4B(0,255,0,255);

    //for (int i = 1; i <= 3; i++) {
    //    for (int j = 1; j <= 3; j++) {
    //        CCPoint a = m1->getCoordsForPosition(j,i);
    //        ccDrawLine(ccp(a.x, VisibleRect::leftTop().y), ccp(a.x, VisibleRect::leftBottom().y));
    //        ccDrawLine(ccp(VisibleRect::leftTop().x, a.y), ccp(VisibleRect::rightTop().x, a.y));
    //    }
    //}

    ccDrawLine(ccp(board00.x, VisibleRect::leftTop().y), ccp(board00.x, VisibleRect::leftBottom().y));
    ccDrawLine(ccp(VisibleRect::leftTop().x, board00.y), ccp(VisibleRect::rightTop().x, board00.y));

    if (foundWinner) {
        glLineWidth(10);
        ccDrawColor4B(255,0,0,255);
        ccDrawLine(winnerStart, winnerEnd);
    }
}

void TicTacToe::setLastState(MARK_STATE state) {
    lastState = state;
}

MARK_STATE TicTacToe::getLastState() {
    return lastState;
}

void TicTacToe::boardUpdate(CCObject* obj) {
    CCLog("Board updated message rcvd, state: %d", ((Mark*)obj)->getState());
    for (int16_t y = 0; y < 3; y++) {
        CCLog("[(0,%d):%d (1,%d):%d (2,%d):%d]", 
            y, markList[0][y]->getState(),
            y, markList[1][y]->getState(),
            y, markList[2][y]->getState());
    }
    checkForWinner(obj);
}

MARK_STATE TicTacToe::getStateForPos(int16_t x, int16_t y) {
    if ((x < 0) || (x > 2) || (y < 0) || (y > 2)) {
        return MARK_STATE_EMPTY;
    }
    //CCLog("Getting state for %d,%d: %d", x, y, markList[x][y]->getState());
    return markList[x][y]->getState();
}

std::vector<MARK_STATE> TicTacToe::getLineState(int16_t x, int16_t y, LINE_DIRECTION direction) {
    int16_t xStep = 0, yStep = 0, xStart = 0, yStart = 0;
    std::vector<MARK_STATE> states;
    switch (direction) {
        case DIR_VERTICAL:
            xStep = 0; yStep = -1; xStart = x; yStart = y+2;
            break;
        case DIR_DIAGONAL_RIGHT:
            xStep = 1; yStep = -1; xStart = x-2; yStart = y+2;
            break;
        case DIR_HORIZONTAL:
            xStep = 1; yStep = 0; xStart = x-2; yStart = y;
            break;
        case DIR_DIAGONAL_LEFT:
            xStep = 1; yStep = 1; xStart = x-2; yStart = y-2;
            break;
    }
    int16_t tx = xStart, ty = yStart;
    for (int c = 0; c <= 4; c++) {
        states.push_back(getStateForPos(tx, ty));
        CCLog("[%d,%d]: %d", tx, ty, getStateForPos(tx, ty));
        tx += xStep;
        ty += yStep;
    }
    return states;
}

bool TicTacToe::checkForWinner(CCObject* obj) {
    Mark* m = (Mark*) obj;
    CCLog("checking line with [%d, %d]: %d", m->getX(), m->getY(), m->getState());
    int16_t winner = -1;
    LINE_DIRECTION dir;
    for(int16_t d = 0; d < DIR_END; d++) {
        dir = LINE_DIRECTION(d);
        auto s = getLineState(m->getX(), m->getY(), dir);
        winner = checkFor3Marks(s);
        CCLog("dir: %d, winner: %d", dir, winner);
        if (winner >= 0) break;
    }
    if (winner>=0) {
        drawWinLine(m->getX(), m->getY(), dir, m);
    }
    CCLog("Found winner: %s (%d)", winner >= 0 ? "yes" : "no", winner);
    return false;
}

int16_t TicTacToe::checkFor3Marks(std::vector<MARK_STATE> marks) {
    MARK_STATE prev = MARK_STATE_EMPTY;
    int c = 0;
    int i = 0, start = 0;
    for (auto m : marks) {
        if (m == MARK_STATE_EMPTY) {
            continue;
        } else if (m == prev) {
            ++c;
        } else if (m != prev) {
            prev = m;
            c = 1;
            start = i;
        }
        i++;
    }
    CCLog("[%d,%d,%d,%d,%d]: %s", marks[0], marks[1], marks[2], marks[3], marks[4], c>2?"yes":"no");
    return (c > 2 ? start : -1);
}

void TicTacToe::drawWinLine(int16_t x, int16_t y, LINE_DIRECTION d, Mark* m) {
    int16_t startX, startY, endX, endY;
    switch (d) {
        case DIR_HORIZONTAL:
            startX = 0;
            endX = 2;
            startY = endY = y;
            break;
        case DIR_VERTICAL:
            startX = endX = x;
            startY = 0;
            endY = 2;
            break;
        case DIR_DIAGONAL_RIGHT:
            startX = 0;
            startY = 2;
            endX = 2;
            endY = 0;
            break;
        case DIR_DIAGONAL_LEFT:
            startX = 0;
            startY = 0;
            endX = 2;
            endY = 2;
            break;
    }
    CCLog("(%d,%d - %d): Winner line from (%d,%d) to (%d,%d)", x, y, d, startX, startY, endX, endY);
    foundWinner = true;
    winnerStart = m->getCoordsForPosition(startX+1, startY+1);
    winnerEnd = m->getCoordsForPosition(endX+1, endY+1);
    CCLog("Line from (%.02f,%.02f) to (%.02f, %.02f)", winnerStart.x, winnerStart.y, winnerEnd.x, winnerEnd.y);
}


MARK_STATE Mark::getState() {
    return state;
}

Mark::Mark(TicTacToe* _b, int16_t _x, int16_t _y) : x(_x), y(_y), board(_b), state(MARK_STATE_EMPTY) {
}

Mark::~Mark(void) {
}

CCRect Mark::rect() {
    CCSize s = getTexture()->getContentSize();
    return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

void Mark::onEnter() {
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void Mark::onExit() {
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}

bool Mark::initWithTexture(CCTexture2D* aTexture) {
    if( CCSprite::initWithTexture(aTexture) ) {
        //CCLog("CCSprint::initWithTexture() failed");
    }
    return true;
}

Mark* Mark::markWithTexture(CCTexture2D* aTexture, TicTacToe* board, int16_t x, int16_t y) {
    Mark* pMark = new Mark(board, x, y);
    pMark->initWithTexture(aTexture);
    pMark->autorelease();
    return pMark;
}

bool Mark::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    //CCLog("Mark::ccTouchBegan(): called");
    if (!containsTouchLocation(touch)) return false;
    return true;
}

void Mark::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    //CCAssert(m_state == kPaddleStateGrabbed, L"Paddle - Unexpected state!");    
    //CCLog("Mark::ccTouchEnded(): called");
    if (state != MARK_STATE_EMPTY && board->getLastState() != MARK_STATE_EMPTY) {
        return;
    }
    switch (board->getLastState()) {
        case MARK_STATE_EMPTY:
            setTexture(CCTextureCache::sharedTextureCache()->addImage(TTTx));
            state = MARK_STATE_X;
            break;
        case MARK_STATE_X:
            setTexture(CCTextureCache::sharedTextureCache()->addImage(TTTo));
            state = MARK_STATE_O;
            break;
        case MARK_STATE_O:
            setTexture(CCTextureCache::sharedTextureCache()->addImage(TTTx));
            state = MARK_STATE_X;
            break;
    }
    board->setLastState(state);
    CCNotificationCenter::sharedNotificationCenter()->postNotification(BOARD_UPDATED, this);
}

CCPoint Mark::getCoordsForPosition(int16_t x, int16_t y) {
    CCPoint c = board->getBoard00();
    CCPoint p;
    int16_t markSize = board->getMarkSize();
    int16_t lineWidt = board->getLineWidth();
    int16_t halfSize = markSize/2;

    p.x = c.x+((x-1)*markSize)+halfSize;
    p.y = c.y+((3-y)*markSize)+halfSize;
    //CCLog("p.x: %.02f p.y: %02.f", p.x, p.y);
    return p;
}

bool Mark::containsTouchLocation(CCTouch* touch) {
    return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

int16_t Mark::getX() {
    return x;
}

int16_t Mark::getY() {
    return y;
}