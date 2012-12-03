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
    for (int i = 1; i <= 3; i++) {
        for (int j = 1; j <= 3; j++) {
            Mark* m = Mark::markWithTexture(markEmptyTexture1, this);
            CCLog("m: %X", m);
            m->setPosition(m->getCoordsForPosition(j,i));
            addChild(m);
            markList.push_back(m);
        }
    }

    //ccDrawLine(ccp(center.x-halfMark-markSize-lineWidth, VisibleRect::leftTop().y), ccp(center.x-halfMark-markSize-lineWidth, VisibleRect::leftBottom().y));
    //ccDrawLine(ccp(VisibleRect::leftTop().x, center.y+halfMark+markSize+lineWidth), ccp(VisibleRect::rightTop().x, center.y+halfMark+markSize+lineWidth));
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
    //CCLog("mark::onEnter(): called");
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

Mark* Mark::markWithTexture(CCTexture2D* aTexture, TicTacToe* board) {
    Mark* pMark = new Mark(board);
    pMark->initWithTexture(aTexture);
    pMark->autorelease();
    return pMark;
}

bool Mark::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    CCLog("Mark::ccTouchBegan(): called");
    if ( !containsTouchLocation(touch) ) return false;
    return true;
}

void Mark::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    //CCAssert(m_state == kPaddleStateGrabbed, L"Paddle - Unexpected state!");    
    CCLog("Mark::ccTouchEnded(): called");
    switch (state) {
        case MARK_STATE_EMPTY:
            setTexture(CCTextureCache::sharedTextureCache()->addImage(TTTx));
            state = MARK_STATE_X;
            break;
        case MARK_STATE_X:
            setTexture(CCTextureCache::sharedTextureCache()->addImage(TTTo));
            state = MARK_STATE_O;
            break;
        case MARK_STATE_O:
            setTexture(CCTextureCache::sharedTextureCache()->addImage(TTTempty));
            state = MARK_STATE_EMPTY;
            break;
    }
}

CCPoint Mark::getCoordsForPosition(int16_t x, int16_t y) {
    CCPoint c = board->getBoard00();
    CCPoint p;
    int16_t markSize = board->getMarkSize();
    int16_t lineWidt = board->getLineWidth();
    int16_t halfSize = markSize/2;

    p.x = c.x+((x-1)*markSize)+halfSize;
    p.y = c.y+((3-y)*markSize)+halfSize;
    CCLog("p.x: %.02f p.y: %02.f", p.x, p.y);
    return p;
}

bool Mark::containsTouchLocation(CCTouch* touch)
{
    return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}