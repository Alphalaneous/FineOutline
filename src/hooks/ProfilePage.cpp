#include "ProfilePage.hpp"
#include "../FineOutline.hpp"
#include <hiimjasmine00.user_data_api/include/Events.hpp>
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>

void MyProfilePage::getUserInfoFinished(GJUserScore* p0) {
    ProfilePage::getUserInfoFinished(p0);
    auto fields = m_fields.self();

    auto playerMenu = m_mainLayer->getChildByID("player-menu");

    if (auto twoPToggler = static_cast<CCMenuItemSpriteExtra*>(m_mainLayer->getChildByID("left-menu")->getChildByID("2p-toggler"))) {
        if (!fields->m_playerToggle) {
            fields->m_playerToggle = twoPToggler->m_pfnSelector;
            twoPToggler->m_pfnSelector = menu_selector(MyProfilePage::on2PToggle);
        }
    }

    if (m_ownProfile) {
        setOutlineColor(false);

        matjson::Value data;
        data["player-1-color"] = cc3bToHexString(alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
        data["player-2-color"] = cc3bToHexString(alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::TWO));

        user_data::upload(data);
    }
}

void MyProfilePage::setPlayerOutline(const matjson::Value& value, CCNode* playerMenu) {
    if (!value.isString()) return;

    auto hexRes = value.asString();
    if (!hexRes) return;

    auto colorRes = cc3bFromHexString(hexRes.unwrap());
    if (!colorRes) return;

    auto color = colorRes.unwrap();
    for (auto menuChild : playerMenu->getChildrenExt()) {
        if (auto player = menuChild->getChildByType<SimplePlayer>(0)) {
            alpha::fine_outline::impl::setOutlineColor(player, color);
        }
    }
}

void MyProfilePage::refreshIcons() {
    if (m_ownProfile) return;

    if (m_score->m_accountID == GJAccountManager::get()->m_accountID) {
        setOutlineColor(false);
        return;
    }

    auto playerMenu = m_mainLayer->getChildByID("player-menu");
    if (!playerMenu) return;

    auto valueRes = user_data::get<matjson::Value>(m_score);
    if (!valueRes) return;

    auto value = valueRes.unwrap();
    if (!value.isObject()) return;

    setPlayerOutline(value["player-1-color"], playerMenu);
    // maybe one day we will see separate dual icons using user data api... I am prepared
}

void MyProfilePage::onUpdate(cocos2d::CCObject* sender) {
    ProfilePage::onUpdate(sender);
    refreshIcons();
}

void MyProfilePage::loadPageFromUserInfo(GJUserScore* score) {
    ProfilePage::loadPageFromUserInfo(score);
    refreshIcons();

    user_data::handleProfilePage(this, [this] (GJUserScore* score) {
        refreshIcons();
    });
}

void MyProfilePage::toggleShip(CCObject* sender) {
    ProfilePage::toggleShip(sender);

    auto playerMenu = m_mainLayer->getChildByID("player-menu");
    if (auto shipNode = playerMenu->getChildByID("player-ship")) {
        if (auto player = shipNode->getChildByType<SimplePlayer>(0)) {
            alpha::fine_outline::impl::updateOutline(player);
        }
    }
}

void MyProfilePage::setOutlineColor(bool p2) {
    auto playerMenu = m_mainLayer->getChildByID("player-menu");
    if (!playerMenu) return;

    for (auto menuChild : playerMenu->getChildrenExt()) {
        if (auto player = menuChild->getChildByType<SimplePlayer>(0)) {
            alpha::fine_outline::impl::setOutlineColor(player, alpha::fine_outline::impl::getColor(static_cast<alpha::fine_outline::PlayerIcon>(p2)));
        }
    }
}

void MyProfilePage::on2PToggle(CCObject* sender) {
    auto fields = m_fields.self();

    (this->*fields->m_playerToggle)(sender);
    if (!m_ownProfile) return;

    setOutlineColor(!static_cast<CCMenuItemToggler*>(sender)->isToggled());
}