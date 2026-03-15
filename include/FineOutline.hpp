#pragma once

#include <Geode/loader/Event.hpp>
#include <Geode/loader/Dispatch.hpp>
#include <Geode/binding/PlayerObject.hpp>
#include <Geode/binding/SimplePlayer.hpp>
#include <ccTypes.h>

#ifdef MY_MOD_ID
    #undef MY_MOD_ID
#endif

#define MY_MOD_ID "alphalaneous.fine_outline"

namespace alpha::fine_outline {

    inline void setOutlineColorP(PlayerObject* player, cocos2d::ccColor3B color)
    GEODE_EVENT_EXPORT_NORES(&setOutlineColorP, (player, color));

    inline cocos2d::ccColor3B getOutlineColorP(PlayerObject* player)
    GEODE_EVENT_EXPORT_NORES(&getOutlineColorP, (player));

    inline void updateOutlineP(PlayerObject* player)
    GEODE_EVENT_EXPORT_NORES(&updateOutlineP, (player));

    inline void setOutlineColorS(SimplePlayer* player, cocos2d::ccColor3B color)
    GEODE_EVENT_EXPORT_NORES(&setOutlineColorS, (player, color));
    
    inline cocos2d::ccColor3B getOutlineColorS(SimplePlayer* player)
    GEODE_EVENT_EXPORT_NORES(&getOutlineColorS, (player));

    inline void updateOutlineS(SimplePlayer* player)
    GEODE_EVENT_EXPORT_NORES(&updateOutlineS, (player));

    enum class PlayerIcon {
        SELECTED = -1,
        ONE,
        TWO
    };

    inline bool isSeparate()
    GEODE_EVENT_EXPORT_NORES(&isSeparate, ());

    inline bool editingP2()
    GEODE_EVENT_EXPORT_NORES(&editingP2, ());

    inline bool usesCustomColor(alpha::fine_outline::PlayerIcon player = alpha::fine_outline::PlayerIcon::SELECTED)
    GEODE_EVENT_EXPORT_NORES(&usesCustomColor, (player));

    inline void useCustomColor(alpha::fine_outline::PlayerIcon player, bool use)
    GEODE_EVENT_EXPORT_NORES(&useCustomColor, (player, use));

    inline cocos2d::ccColor3B getCustomColor(alpha::fine_outline::PlayerIcon player = alpha::fine_outline::PlayerIcon::SELECTED)
    GEODE_EVENT_EXPORT_NORES(&getCustomColor, (player));

    inline void setCustomColor(alpha::fine_outline::PlayerIcon player, cocos2d::ccColor3B color)
    GEODE_EVENT_EXPORT_NORES(&setCustomColor, (player, color));

    inline int getColorIndex(alpha::fine_outline::PlayerIcon player = alpha::fine_outline::PlayerIcon::SELECTED)
    GEODE_EVENT_EXPORT_NORES(&getColorIndex, (player));

    inline void setColorIndex(alpha::fine_outline::PlayerIcon player, int color)
    GEODE_EVENT_EXPORT_NORES(&setColorIndex, (player, color));

    inline cocos2d::ccColor3B getColor(alpha::fine_outline::PlayerIcon player = alpha::fine_outline::PlayerIcon::SELECTED)
    GEODE_EVENT_EXPORT_NORES(&getColor, (player));
}