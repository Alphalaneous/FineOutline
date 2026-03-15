#include "GJScoreCell.hpp"
#include "../FineOutline.hpp"

void MyGJScoreCell::loadFromScore(GJUserScore* p0) {
    GJScoreCell::loadFromScore(p0);

    if (p0->m_accountID != GJAccountManager::get()->m_accountID) return;

    if (auto player = typeinfo_cast<SimplePlayer*>(getChildByIDRecursive("player-icon"))) {
        alpha::fine_outline::impl::setOutlineColor(player, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
    }
}