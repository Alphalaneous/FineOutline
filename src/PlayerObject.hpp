#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/PlayerObject.hpp>

using namespace geode::prelude;

class $modify(MyPlayerObject, PlayerObject) {
    static void onModify(auto& self) {
        (void)self.setHookPriority("PlayerObject::playSpawnEffect", -2);
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerFrame", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerShipFrame", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerRollFrame", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerBirdFrame", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerDartFrame", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::createRobot", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::createSpider", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerSwingFrame", "hiimjustin000.more_icons");
        (void)self.setHookPriorityAfterPost("PlayerObject::updatePlayerJetpackFrame", "hiimjustin000.more_icons");
    }

    void updatePlayerShaders();
    bool init(int player, int ship, GJBaseGameLayer* gameLayer, CCLayer* layer, bool highGraphics);
    void updatePlayerFrame(int frame);
    void updatePlayerShipFrame(int frame);
    void updatePlayerRollFrame(int frame);
    void updatePlayerBirdFrame(int frame);
    void updatePlayerDartFrame(int frame);
    void* getCustomVTablePtr();
    void replaceBatchWithNode(CCSpriteBatchNode* batchNode);
    void createRobot(int frame);
    void createSpider(int frame);
    void updatePlayerSwingFrame(int frame);
    void updatePlayerJetpackFrame(int frame);
};
