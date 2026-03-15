#include "GJGarageLayer.hpp"
#include "../FineOutline.hpp"

bool MyGJGarageLayer::init() {
    if (!GJGarageLayer::init()) return false;

    if (m_playerObject) {
        alpha::fine_outline::impl::setOutlineColor(m_playerObject, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
    }
    if (auto player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
        alpha::fine_outline::impl::setOutlineColor(player2, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::TWO));
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
        alpha::fine_outline::impl::setOutlineColor(m_playerObject, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
    }
    if (auto player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
        alpha::fine_outline::impl::setOutlineColor(player2, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::TWO));
    }
}

void MyGJGarageLayer::onSwap(cocos2d::CCObject* sender) {
    auto fields = m_fields.self();
    (this->*fields->m_swapHandler)(sender);
    
    auto overrideP1 = alpha::fine_outline::impl::usesCustomColor(alpha::fine_outline::PlayerIcon::ONE);
    auto overrideP2 = alpha::fine_outline::impl::usesCustomColor(alpha::fine_outline::PlayerIcon::TWO);

    auto oColorP1 = alpha::fine_outline::impl::getCustomColor(alpha::fine_outline::PlayerIcon::ONE);
    auto oColorP2 = alpha::fine_outline::impl::getCustomColor(alpha::fine_outline::PlayerIcon::TWO);

    auto rColorP1 = alpha::fine_outline::impl::getColorIndex(alpha::fine_outline::PlayerIcon::ONE);
    auto rColorP2 = alpha::fine_outline::impl::getColorIndex(alpha::fine_outline::PlayerIcon::TWO);

    alpha::fine_outline::impl::useCustomColor(alpha::fine_outline::PlayerIcon::ONE, overrideP2);
    alpha::fine_outline::impl::useCustomColor(alpha::fine_outline::PlayerIcon::TWO, overrideP1);

    alpha::fine_outline::impl::setCustomColor(alpha::fine_outline::PlayerIcon::ONE, oColorP2);
    alpha::fine_outline::impl::setCustomColor(alpha::fine_outline::PlayerIcon::TWO, oColorP1);

    alpha::fine_outline::impl::setColorIndex(alpha::fine_outline::PlayerIcon::ONE, rColorP2);
    alpha::fine_outline::impl::setColorIndex(alpha::fine_outline::PlayerIcon::TWO, rColorP1);

    if (m_playerObject) {
        alpha::fine_outline::impl::setOutlineColor(m_playerObject, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
    }
    if (auto player2 = typeinfo_cast<SimplePlayer*>(getChildByID("player2-icon"))) {
        alpha::fine_outline::impl::setOutlineColor(player2, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::TWO));
    }
}