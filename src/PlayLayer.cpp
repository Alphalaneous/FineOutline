#include "PlayLayer.hpp"
#include "SimplePlayer.hpp"
#include "Utils.hpp"

bool MyPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
    if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

    schedule(schedule_selector(MyPlayLayer::checkGlobed));
    return true;
}

void MyPlayLayer::checkGlobed(float dt) {
    if (auto wrapper = m_progressBar->getChildByID("dankmeme.globed2/progress-bar-wrapper")) {
        if (auto progressIcon = wrapper->getChildByID("dankmeme.globed2/self-player-progress")) {
            if (auto globedSimplePlayer = progressIcon->getChildByType<GlobedSimplePlayer>(0)) {
                if (auto player = globedSimplePlayer->getChildByType<SimplePlayer>(0)) {
                    static_cast<MySimplePlayer*>(player)->setOutlineColor(alpha::fine_outline::getP1Color());
                    unschedule(schedule_selector(MyPlayLayer::checkGlobed));
                }
            }
        }
    }
}