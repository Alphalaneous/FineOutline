#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(MyProfilePage, ProfilePage) {
    struct Fields {
        SEL_MenuHandler m_playerToggle;
        SEL_MenuHandler m_shipToggle;
    };

    void getUserInfoFinished(GJUserScore* p0);
    void toggleShip(CCObject* sender);
    void on2PToggle(CCObject* sender);
    void setOutlineColor(bool p2);
    void onShipToggle(CCObject* sender);
};