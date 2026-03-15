#define GEODE_DEFINE_EVENT_EXPORTS
#include "../include/FineOutline.hpp"
#include "SimplePlayer.hpp"
#include "PlayerObject.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace alpha::fine_outline {

    void setOutlineColorP(PlayerObject* player, cocos2d::ccColor3B color) {
        log::info("PlayerObject outline set");
        auto pl = static_cast<MyPlayerObject*>(player);
        pl->setOutlineColor(color);
    }

    cocos2d::ccColor3B getOutlineColorP(PlayerObject* player) {
        log::info("PlayerObject outline get");
        auto pl = static_cast<MyPlayerObject*>(player);
        return pl->getOutlineColor();
    }

    void updateOutlineP(PlayerObject* player) {
        log::info("PlayerObject outline update");
        auto pl = static_cast<MyPlayerObject*>(player);
        pl->updateOutlineColors();
    }

    void setOutlineColorS(SimplePlayer* player, cocos2d::ccColor3B color) {
        log::info("SimplePlayer outline set");
        //auto pl = static_cast<MySimplePlayer*>(player);
        //pl->setOutlineColor(color);
    }

    cocos2d::ccColor3B getOutlineColorS(SimplePlayer* player) {
        log::info("SimplePlayer outline get");
        //auto pl = static_cast<MySimplePlayer*>(player);
        //if (!pl) return {};
        //return pl->getOutlineColor();
        return {};
    }

    void updateOutlineS(SimplePlayer* player) {
        log::info("SimplePlayer outline update");
        //auto pl = static_cast<MySimplePlayer*>(player);
        //if (!pl) return;
        //pl->updateOutlineColors();
    }

    bool isSeparate() {
        log::info("isSeparate");
        return Loader::get()->getLoadedMod("weebify.separate_dual_icons");
    }

    bool editingP2() {
        log::info("editingP2");
        auto sdi = Loader::get()->getLoadedMod("weebify.separate_dual_icons");
        if (!sdi) return false;

        return sdi->getSavedValue<bool>("2pselected");
    }

    bool usesCustomColor(alpha::fine_outline::PlayerIcon player) {
        log::info("usesCustomColor");
        switch (player) {
            case alpha::fine_outline::PlayerIcon::SELECTED: {
                return Mod::get()->getSavedValue<bool>(editingP2() ? "override-color-p2" : "override-color"); 
            }
            case alpha::fine_outline::PlayerIcon::ONE: {
                return Mod::get()->getSavedValue<bool>("override-color"); 
            }
            case alpha::fine_outline::PlayerIcon::TWO: {
                return Mod::get()->getSavedValue<bool>(Loader::get()->getLoadedMod("weebify.separate_dual_icons") ? "override-color-p2" : "override-color"); 
            }
        }
        return false;
    }

    void useCustomColor(alpha::fine_outline::PlayerIcon player, bool use) {
        log::info("useCustomColor");
        switch (player) {
            case alpha::fine_outline::PlayerIcon::SELECTED: {
                Mod::get()->setSavedValue(editingP2() ? "override-color-p2" : "override-color", use);
                break;
            }
            case alpha::fine_outline::PlayerIcon::ONE: {
                Mod::get()->setSavedValue("override-color", use); 
                break;
            }
            case alpha::fine_outline::PlayerIcon::TWO: {
                Mod::get()->setSavedValue("override-color-p2", use); 
                break;
            }
        }
    }

    cocos2d::ccColor3B getCustomColor(alpha::fine_outline::PlayerIcon player) {
        log::info("getCustomColor");
        switch (player) {
            case alpha::fine_outline::PlayerIcon::SELECTED: {
                return Mod::get()->getSavedValue<ccColor3B>(editingP2() ? "p2-color" : "p1-color");
            }
            case alpha::fine_outline::PlayerIcon::ONE: {
                return Mod::get()->getSavedValue<ccColor3B>("p1-color");
            }
            case alpha::fine_outline::PlayerIcon::TWO: {
                return Mod::get()->getSavedValue<ccColor3B>(Loader::get()->getLoadedMod("weebify.separate_dual_icons") ? "p2-color" : "p1-color");
            }
        }
        return {};
    }

    void setCustomColor(alpha::fine_outline::PlayerIcon player, cocos2d::ccColor3B color) {
        log::info("setCustomColor");
        switch (player) {
            case alpha::fine_outline::PlayerIcon::SELECTED: {
                Mod::get()->setSavedValue(editingP2() ? "p2-color" : "p1-color", color);
                break;
            }
            case alpha::fine_outline::PlayerIcon::ONE: {
                Mod::get()->setSavedValue("p1-color", color);
                break;
            }
            case alpha::fine_outline::PlayerIcon::TWO: {
                Mod::get()->setSavedValue("p2-color", color);
                break;
            }
        }
    }

    int getColorIndex(alpha::fine_outline::PlayerIcon player) {
        log::info("getColorIndex");
        switch (player) {
            case alpha::fine_outline::PlayerIcon::SELECTED: {
                return Mod::get()->getSavedValue<int>(editingP2() ? "outline-color-p2" : "outline-color-p1");
            }
            case alpha::fine_outline::PlayerIcon::ONE: {
                return Mod::get()->getSavedValue<int>("outline-color-p1");
            }
            case alpha::fine_outline::PlayerIcon::TWO: {
                return Mod::get()->getSavedValue<int>(Loader::get()->getLoadedMod("weebify.separate_dual_icons") ? "outline-color-p2" : "outline-color-p1");
            }
        }
        return 0;
    }

    void setColorIndex(alpha::fine_outline::PlayerIcon player, int color) {
        log::info("setColorIndex");
        switch (player) {
            case alpha::fine_outline::PlayerIcon::SELECTED: {
                Mod::get()->setSavedValue(editingP2() ? "outline-color-p2" : "outline-color-p1", color);
                break;
            }
            case alpha::fine_outline::PlayerIcon::ONE: {
                Mod::get()->setSavedValue("outline-color-p1", color);
                break;
            }
            case alpha::fine_outline::PlayerIcon::TWO: {
                Mod::get()->setSavedValue("outline-color-p2", color);
                break;
            }
        }
    }

    cocos2d::ccColor3B getColor(alpha::fine_outline::PlayerIcon player) {
        log::info("getColor");
        return usesCustomColor(player) ? getCustomColor(player) : GameManager::get()->colorForIdx(getColorIndex(player));
    }
}