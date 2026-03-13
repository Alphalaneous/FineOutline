#include "GJScoreCell.hpp"
#include "SimplePlayer.hpp"
#include "Utils.hpp"

void MyGJScoreCell::loadFromScore(GJUserScore* p0) {
    GJScoreCell::loadFromScore(p0);

    if (p0->m_accountID != GJAccountManager::get()->m_accountID) return;

    if (auto player = typeinfo_cast<SimplePlayer*>(getChildByIDRecursive("player-icon"))) {
        auto p = static_cast<MySimplePlayer*>(player);
        p->setOutlineColor(alpha::fine_outline::getP1Color());
    }
}