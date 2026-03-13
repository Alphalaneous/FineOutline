#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/SimplePlayer.hpp>

using namespace geode::prelude;

class $modify(MySimplePlayer, SimplePlayer) {
    struct Fields {
        bool m_isGlobedSelf = false;
        bool m_isShaderSpr = false;
        bool m_shaderSprDual = false;
    };

    void removeAllShaders();
    void setOutlineColor(const ccColor3B& color, bool dual);
    void updatePlayerShaders(bool dual);
    void updatePlayerFrame(int p0, IconType p1);
};