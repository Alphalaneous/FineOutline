#include "ProfilePage.hpp"
#include "SimplePlayer.hpp"

void MyProfilePage::getUserInfoFinished(GJUserScore* p0) {
    ProfilePage::getUserInfoFinished(p0);
    if (!m_ownProfile) return;

    auto fields = m_fields.self();

    auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
    auto dual = sdi && sdi->getSavedValue<bool>("2pselected");
    auto playerMenu = m_mainLayer->getChildByID("player-menu");
    for (auto menuChild : playerMenu->getChildrenExt()) {
        if (auto player = menuChild->getChildByType<SimplePlayer>(0)) {
            static_cast<MySimplePlayer*>(player)->updatePlayerShaders(dual);
        }
        if (auto innerNode = menuChild->getChildByType(0)) {
            if (auto player = innerNode->getChildByType<SimplePlayer>(0)) {
                static_cast<MySimplePlayer*>(player)->updatePlayerShaders(dual);
            }
        }
    }

    if (auto twoPToggler = static_cast<CCMenuItemSpriteExtra*>(m_mainLayer->getChildByID("left-menu")->getChildByID("2p-toggler"))) {
        fields->m_playerToggle = twoPToggler->m_pfnSelector;
        twoPToggler->m_pfnSelector = menu_selector(MyProfilePage::on2PToggle);
    }

    if (Loader::get()->isModLoaded("weebify.separate_dual_icons") && !Loader::get()->isModLoaded("rynat.better_unlock_info")) {
        if (auto shipToggler = static_cast<CCMenuItemSpriteExtra*>(playerMenu->getChildByID("player-ship"))) {
            fields->m_shipToggle = shipToggler->m_pfnSelector;
            shipToggler->m_pfnSelector = menu_selector(MyProfilePage::onShipToggle);
        }
    }
}

void MyProfilePage::toggleShip(CCObject* sender) {
    ProfilePage::toggleShip(sender);
    if (!m_ownProfile) return;

    auto playerMenu = m_mainLayer->getChildByID("player-menu");
    if (auto shipNode = playerMenu->getChildByID("player-ship")) {
        if (auto player = shipNode->getChildByType<SimplePlayer>(0)) {
            auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
            static_cast<MySimplePlayer*>(player)->updatePlayerShaders(sdi && sdi->getSavedValue<bool>("2pselected"));
        }
    }
}

void MyProfilePage::on2PToggle(CCObject* sender) {
    auto fields = m_fields.self();

    (this->*fields->m_playerToggle)(sender);
    if (!m_ownProfile) return;

    auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
    auto dual = sdi && sdi->getSavedValue<bool>("2pselected");
    auto playerMenu = m_mainLayer->getChildByID("player-menu");
    for (auto menuChild : playerMenu->getChildrenExt()) {
        if (auto player = menuChild->getChildByType<SimplePlayer>(0)) {
            static_cast<MySimplePlayer*>(player)->updatePlayerShaders(dual);
        }
        if (auto innerNode = menuChild->getChildByType(0)) {
            if (auto player = innerNode->getChildByType<SimplePlayer>(0)) {
                static_cast<MySimplePlayer*>(player)->updatePlayerShaders(dual);
            }
        }
    }
}

void MyProfilePage::onShipToggle(CCObject* sender) {
    auto fields = m_fields.self();

    (this->*fields->m_shipToggle)(sender);
    if (!m_ownProfile) return;

    auto playerMenu = m_mainLayer->getChildByID("player-menu");
    if (auto shipNode = playerMenu->getChildByID("player-ship")) {
        if (auto player = shipNode->getChildByType<SimplePlayer>(0)) {
            auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
            static_cast<MySimplePlayer*>(player)->updatePlayerShaders(sdi && sdi->getSavedValue<bool>("2pselected"));
        }
    }
}