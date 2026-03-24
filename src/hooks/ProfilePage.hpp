#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(MyProfilePage, ProfilePage) {
    struct Fields {
        SEL_MenuHandler m_playerToggle;
        SEL_MenuHandler m_shipToggle;
    };

    void setPlayerOutline(const matjson::Value& value, CCNode* playerMenu);
    void loadPageFromUserInfo(GJUserScore* score);
    void getUserInfoFinished(GJUserScore* p0);
    void toggleShip(CCObject* sender);
    void refreshIcons();
    void onUpdate(cocos2d::CCObject* sender);
    void on2PToggle(CCObject* sender);
    void setOutlineColor(bool p2);
};