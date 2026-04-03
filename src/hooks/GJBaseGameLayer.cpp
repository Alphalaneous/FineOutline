#include "GJBaseGameLayer.hpp"
#include "../FineOutline.hpp"

void MyGJBaseGameLayer::createPlayer() {
    GJBaseGameLayer::createPlayer();

    alpha::fine_outline::impl::setOutlineColor(m_player1, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
    alpha::fine_outline::impl::setOutlineColor(m_player2, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::TWO));
}

void MyGJBaseGameLayer::resetPlayer() {
    GJBaseGameLayer::resetPlayer();

    alpha::fine_outline::impl::updateOutline(m_player1);
    alpha::fine_outline::impl::updateOutline(m_player2);
}

void MyGJBaseGameLayer::playExitDualEffect(PlayerObject* player) {
    std::set<SimplePlayer*> simplePlayers;

    for (auto child : getChildrenExt()) {
        if (auto sp = typeinfo_cast<SimplePlayer*>(child)) {
            simplePlayers.insert(sp);
        }
    }

    GJBaseGameLayer::playExitDualEffect(player);
    
    SimplePlayer* actualPlayer;

    for (auto child : getChildrenExt()) {
        if (auto sp = typeinfo_cast<SimplePlayer*>(child)) {
            if (simplePlayers.contains(sp)) continue;
            actualPlayer = sp;
        }
    }

    if (!actualPlayer) return;

    if (player->isPlayer1()) {
        alpha::fine_outline::impl::setOutlineColor(actualPlayer, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
    }
    else if (player->isPlayer2()) {
        alpha::fine_outline::impl::setOutlineColor(actualPlayer, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::TWO));
    }
}