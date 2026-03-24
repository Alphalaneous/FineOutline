#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/GJUserCell.hpp>

using namespace geode::prelude;

class $modify(MyGJUserCell, GJUserCell) {
    void loadFromScore(GJUserScore* p0);
};