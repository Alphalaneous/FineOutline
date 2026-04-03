#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

class $modify(MyGJBaseGameLayer, GJBaseGameLayer) {
    void createPlayer();
    void resetPlayer();
    void playExitDualEffect(PlayerObject* player);
};