#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/GJRequestCell.hpp>

using namespace geode::prelude;

class $modify(MyGJRequestCell, GJRequestCell) {
    void loadFromScore(GJUserScore* p0);
};