#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>

using namespace geode::prelude;

class $modify(MyGJGarageLayer, GJGarageLayer) {
    static void onModify(auto& self) {
        (void)self.setHookPriority("GJGarageLayer::init", -2);
        (void)self.setHookPriority("GJGarageLayer::onSelect", -2);\
    }

    struct Fields {
        SEL_MenuHandler m_swapHandler;
    };

    bool init();
    void onSelect(cocos2d::CCObject* sender);
    void onSwap(cocos2d::CCObject* sender);
};
