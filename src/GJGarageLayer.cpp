#include "GJGarageLayer.hpp"
#include "../include/FineOutline.hpp"
#include "Geode/loader/Log.hpp"

bool MyGJGarageLayer::init() {
    if (!GJGarageLayer::init()) return false;

    if (m_playerObject) {
        log::info("before set p1: {}", m_playerObject);

        // it freezes running this
        alpha::fine_outline::setOutlineColorS(m_playerObject, {255, 0, 255});//alpha::fine_outline::getColor(alpha::fine_outline::PlayerIcon::ONE));
        log::info("after set p1");
    }
    if (auto player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
        log::info("before set p2");
        alpha::fine_outline::setOutlineColorS(player2, alpha::fine_outline::getColor(alpha::fine_outline::PlayerIcon::TWO));
        log::info("after set p2");
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
        alpha::fine_outline::setOutlineColorS(m_playerObject, alpha::fine_outline::getColor(alpha::fine_outline::PlayerIcon::ONE));
    }
    if (auto player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
        alpha::fine_outline::setOutlineColorS(player2, alpha::fine_outline::getColor(alpha::fine_outline::PlayerIcon::TWO));
    }
}

void MyGJGarageLayer::onSwap(cocos2d::CCObject* sender) {
    auto fields = m_fields.self();
    (this->*fields->m_swapHandler)(sender);
    
    auto overrideP1 = alpha::fine_outline::usesCustomColor(alpha::fine_outline::PlayerIcon::ONE);
    auto overrideP2 = alpha::fine_outline::usesCustomColor(alpha::fine_outline::PlayerIcon::TWO);

    auto oColorP1 = alpha::fine_outline::getCustomColor(alpha::fine_outline::PlayerIcon::ONE);
    auto oColorP2 = alpha::fine_outline::getCustomColor(alpha::fine_outline::PlayerIcon::TWO);

    auto rColorP1 = alpha::fine_outline::getColorIndex(alpha::fine_outline::PlayerIcon::ONE);
    auto rColorP2 = alpha::fine_outline::getColorIndex(alpha::fine_outline::PlayerIcon::TWO);

    alpha::fine_outline::useCustomColor(alpha::fine_outline::PlayerIcon::ONE, overrideP2);
    alpha::fine_outline::useCustomColor(alpha::fine_outline::PlayerIcon::TWO, overrideP1);

    alpha::fine_outline::setCustomColor(alpha::fine_outline::PlayerIcon::ONE, oColorP2);
    alpha::fine_outline::setCustomColor(alpha::fine_outline::PlayerIcon::TWO, oColorP1);

    alpha::fine_outline::setColorIndex(alpha::fine_outline::PlayerIcon::ONE, rColorP2);
    alpha::fine_outline::setColorIndex(alpha::fine_outline::PlayerIcon::TWO, rColorP1);

    if (m_playerObject) {
        alpha::fine_outline::setOutlineColorS(m_playerObject, alpha::fine_outline::getColor(alpha::fine_outline::PlayerIcon::ONE));
    }
    if (auto player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
        alpha::fine_outline::setOutlineColorS(player2, alpha::fine_outline::getColor(alpha::fine_outline::PlayerIcon::TWO));
    }
}