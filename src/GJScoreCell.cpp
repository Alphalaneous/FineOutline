#include "GJScoreCell.hpp"
#include "../include/FineOutline.hpp"

void MyGJScoreCell::loadFromScore(GJUserScore* p0) {
    GJScoreCell::loadFromScore(p0);

    if (p0->m_accountID != GJAccountManager::get()->m_accountID) return;

    if (auto player = typeinfo_cast<SimplePlayer*>(getChildByIDRecursive("player-icon"))) {
        alpha::fine_outline::setOutlineColorS(player, alpha::fine_outline::getColor(alpha::fine_outline::PlayerIcon::ONE));
    }
}