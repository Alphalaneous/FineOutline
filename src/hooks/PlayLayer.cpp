#include "PlayLayer.hpp"
#include "../FineOutline.hpp"

bool MyPlayLayer::init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
    if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

    schedule(schedule_selector(MyPlayLayer::checkGlobed));
    return true;
}

void MyPlayLayer::checkGlobed(float dt) {
    auto wrapper = m_progressBar->getChildByID("dankmeme.globed2/progress-bar-wrapper");
    if (!wrapper) return;

    auto progressIcon = wrapper->getChildByID("dankmeme.globed2/self-player-progress");
    if (!progressIcon) return;

    auto globedSimplePlayer = progressIcon->getChildByType<GlobedSimplePlayer>(0);
    if (!globedSimplePlayer) return;
    
    if (auto player = globedSimplePlayer->getChildByType<SimplePlayer>(0)) {
        alpha::fine_outline::impl::setOutlineColor(player, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
        unschedule(schedule_selector(MyPlayLayer::checkGlobed));
    }
}