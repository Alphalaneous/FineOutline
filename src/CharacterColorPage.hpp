#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/CharacterColorPage.hpp>

using namespace geode::prelude;

class CustomCharacterColorPage : public CharacterColorPage {};

class $modify(MyCharacterColorPage, CharacterColorPage) {

    static void onModify(auto& self) {
        (void)self.setHookPriority("CharacterColorPage::init", -2);
        (void)self.setHookPriority("CharacterColorPage::toggleShip", -2);
    }

    struct Fields {
        CCMenuItemSpriteExtra* m_outlineColorBtn;
        CCMenuItemToggler* m_outlineTab;
        CCMenuItemToggler* m_customColorToggle;
        CCSprite* m_outlineSelector;
        CCLabelBMFont* m_customColorLabel;
        bool m_disable = false;
    };

    void updateColor(ccColor3B const& c, bool dual);
    void moveToMenu(CCNode* menu, CCMenu* orig, std::string_view id);
    void setOutlineSelectorPos(int color);
    bool init();
    void onCustomColorToggle(CCObject* sender);
    void onOutlineColor(CCObject* sender);
    void showLocksForTab(int tab);
    void onMode(CCObject* sender);
    void onPlayerColor(cocos2d::CCObject* sender);
    void toggleShip(CCObject* sender);
    void setColorOnGarage();
    #ifndef GEODE_IS_ANDROID
    void keyBackClicked();
    #endif
    void onCloseH(CCObject* sender);
};