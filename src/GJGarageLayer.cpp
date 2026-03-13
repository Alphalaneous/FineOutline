#include "GJGarageLayer.hpp"
#include "SimplePlayer.hpp"

bool MyGJGarageLayer::init() {
    if (!GJGarageLayer::init()) return false;

    if (m_playerObject) {
        static_cast<MySimplePlayer*>(m_playerObject)->updatePlayerShaders(false);
    }
    if (auto player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
        static_cast<MySimplePlayer*>(player2)->updatePlayerShaders(true);
    }

    return true;
}

void MyGJGarageLayer::onSelect(cocos2d::CCObject* sender) {
    GJGarageLayer::onSelect(sender);
    
    if (m_playerObject) {
        static_cast<MySimplePlayer*>(m_playerObject)->updatePlayerShaders(false);
    }
    if (auto player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
        static_cast<MySimplePlayer*>(player2)->updatePlayerShaders(true);
    }
}