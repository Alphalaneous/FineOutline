#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/SimplePlayer.hpp>

using namespace geode::prelude;

class $modify(MySimplePlayer, SimplePlayer) {
    struct Fields {
        ccColor3B m_outlineColor;
        bool m_usesOutlineColor = true;
        bool m_usingDefaultColor = true;
        std::unordered_map<CCSprite*, CCSprite*> m_outlines;
    };

    void setupOutlines();
    void enableOutlineColor(bool enable);
    
    void setOutlineColor(const ccColor3B& color);
    ccColor3B getOutlineColor();

    void updateOutlineColors();
    void updatePlayerFrame(int p0, IconType p1);
};