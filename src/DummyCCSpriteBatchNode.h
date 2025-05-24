#include <Geode/Geode.hpp>

using namespace geode::prelude;

#define FORWARD_BASE(RET, METHOD, PARAMS, ARGS) \
    virtual RET METHOD PARAMS { return CCSpriteBatchNode::METHOD ARGS; }

class DummyCCSpriteBatchNode : public CCSpriteBatchNode {

    FORWARD_BASE(CCTexture2D*, getTexture, (), ());
    FORWARD_BASE(void, setTexture, (CCTexture2D* p0), (p0));
    FORWARD_BASE(void, setBlendFunc, (ccBlendFunc p0), (p0));
    FORWARD_BASE(ccBlendFunc, getBlendFunc, (), ());

    virtual void visit(void) {
        CCNode::visit();
    }

    FORWARD_BASE(void, addChild, (CCNode* p0), (p0));
    FORWARD_BASE(void, addChild, (CCNode* p0, int p1), (p0, p1));
    FORWARD_BASE(void, addChild, (CCNode* p0, int p1, int p2), (p0, p1, p2));
    FORWARD_BASE(void, reorderChild, (CCNode* p0, int p1), (p0, p1));
    FORWARD_BASE(void, removeChild, (CCNode* p0, bool p1), (p0, p1));
    FORWARD_BASE(void, removeAllChildrenWithCleanup, (bool p1), (p1));
    FORWARD_BASE(void, sortAllChildren, (), ());

    virtual void draw(void) {
        CCNode::draw();
    }

};