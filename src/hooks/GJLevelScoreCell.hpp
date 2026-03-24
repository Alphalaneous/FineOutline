#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/GJLevelScoreCell.hpp>

using namespace geode::prelude;

class $modify(MyGJLevelScoreCell, GJLevelScoreCell) {
    void loadFromScore(GJUserScore* p0);
};