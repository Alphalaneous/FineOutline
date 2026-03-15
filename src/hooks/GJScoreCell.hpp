#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/GJScoreCell.hpp>

using namespace geode::prelude;

class $modify(MyGJScoreCell, GJScoreCell) {
    void loadFromScore(GJUserScore* p0);
};