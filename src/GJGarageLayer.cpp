#include "GJGarageLayer.hpp"
#include "SimplePlayer.hpp"
#include "Utils.hpp"

bool MyGJGarageLayer::init() {
    if (!GJGarageLayer::init()) return false;

    if (m_playerObject) {
        static_cast<MySimplePlayer*>(m_playerObject)->setOutlineColor(alpha::fine_outline::getP1Color());
    }
    if (auto player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
        static_cast<MySimplePlayer*>(player2)->setOutlineColor(alpha::fine_outline::getP2Color());
    }

    auto fields = m_fields.self();
    auto shardsMenu = getChildByID("shards-menu");
    if (shardsMenu) {
        auto swapButton = static_cast<CCMenuItemSpriteExtra*>(shardsMenu->getChildByID("swap-2p-button"));
        if (swapButton) {
            fields->m_swapHandler = swapButton->m_pfnSelector;
            swapButton->m_pfnSelector = menu_selector(MyGJGarageLayer::onSwap);
        }
    }

    return true;
}

void MyGJGarageLayer::onSelect(cocos2d::CCObject* sender) {
    GJGarageLayer::onSelect(sender);
    
    if (m_playerObject) {
        static_cast<MySimplePlayer*>(m_playerObject)->setOutlineColor(alpha::fine_outline::getP1Color());
    }
    if (auto player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
        static_cast<MySimplePlayer*>(player2)->setOutlineColor(alpha::fine_outline::getP2Color());
    }
}

void MyGJGarageLayer::onSwap(cocos2d::CCObject* sender) {
    auto fields = m_fields.self();
    (this->*fields->m_swapHandler)(sender);
    
    auto overrideP1 = alpha::fine_outline::hasOverrideP1();
    auto overrideP2 = alpha::fine_outline::hasOverrideP2();

    auto oColorP1 = alpha::fine_outline::getOverrideColorP1();
    auto oColorP2 = alpha::fine_outline::getOverrideColorP2();

    auto rColorP1 = alpha::fine_outline::getRegularColorP1();
    auto rColorP2 = alpha::fine_outline::getRegularColorP2();

    Mod::get()->setSavedValue("override-color", overrideP2); 
    Mod::get()->setSavedValue("override-color-p2", overrideP1); 

    Mod::get()->setSavedValue("p1-color", oColorP2); 
    Mod::get()->setSavedValue("p2-color", oColorP1); 

    Mod::get()->setSavedValue("outline-color-p1", rColorP2); 
    Mod::get()->setSavedValue("outline-color-p2", rColorP1);

    if (m_playerObject) {
        static_cast<MySimplePlayer*>(m_playerObject)->setOutlineColor(alpha::fine_outline::getP1Color());
    }
    if (auto player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
        static_cast<MySimplePlayer*>(player2)->setOutlineColor(alpha::fine_outline::getP2Color());
    }
}