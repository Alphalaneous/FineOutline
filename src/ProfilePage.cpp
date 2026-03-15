#include "ProfilePage.hpp"
#include "../include/FineOutline.hpp"

void MyProfilePage::getUserInfoFinished(GJUserScore* p0) {
    ProfilePage::getUserInfoFinished(p0);
    if (!m_ownProfile) return;

    auto fields = m_fields.self();

    auto playerMenu = m_mainLayer->getChildByID("player-menu");
    setOutlineColor(false);

    if (auto twoPToggler = static_cast<CCMenuItemSpriteExtra*>(m_mainLayer->getChildByID("left-menu")->getChildByID("2p-toggler"))) {
        if (!fields->m_playerToggle) {
            fields->m_playerToggle = twoPToggler->m_pfnSelector;
            twoPToggler->m_pfnSelector = menu_selector(MyProfilePage::on2PToggle);
        }
    }

    if (Loader::get()->isModLoaded("weebify.separate_dual_icons") && !Loader::get()->isModLoaded("rynat.better_unlock_info")) {
        if (auto shipToggler = static_cast<CCMenuItemSpriteExtra*>(playerMenu->getChildByID("player-ship"))) {
            if (!fields->m_shipToggle) {
                fields->m_shipToggle = shipToggler->m_pfnSelector;
                shipToggler->m_pfnSelector = menu_selector(MyProfilePage::onShipToggle);
            }
        }
    }
}

void MyProfilePage::toggleShip(CCObject* sender) {
    ProfilePage::toggleShip(sender);
    if (!m_ownProfile) return;

    auto playerMenu = m_mainLayer->getChildByID("player-menu");
    if (auto shipNode = playerMenu->getChildByID("player-ship")) {
        if (auto player = shipNode->getChildByType<SimplePlayer>(0)) {
            alpha::fine_outline::updateOutlineS(player);
        }
    }
}

void MyProfilePage::setOutlineColor(bool p2) {
    auto playerMenu = m_mainLayer->getChildByID("player-menu");
    if (!playerMenu) return;

    for (auto menuChild : playerMenu->getChildrenExt()) {
        if (auto player = menuChild->getChildByType<SimplePlayer>(0)) {
            alpha::fine_outline::setOutlineColorS(player, alpha::fine_outline::getColor(static_cast<alpha::fine_outline::PlayerIcon>(p2)));
        }
    }
}

void MyProfilePage::on2PToggle(CCObject* sender) {
    auto fields = m_fields.self();

    (this->*fields->m_playerToggle)(sender);
    if (!m_ownProfile) return;

    setOutlineColor(!static_cast<CCMenuItemToggler*>(sender)->isToggled());
}

void MyProfilePage::onShipToggle(CCObject* sender) {
    auto fields = m_fields.self();

    (this->*fields->m_shipToggle)(sender);
    if (!m_ownProfile) return;

    auto playerMenu = m_mainLayer->getChildByID("player-menu");
    if (auto shipNode = playerMenu->getChildByID("player-ship")) {
        if (auto player = shipNode->getChildByType<SimplePlayer>(0)) {
            alpha::fine_outline::updateOutlineS(player);
        }
    }
}