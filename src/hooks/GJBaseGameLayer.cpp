#include "GJBaseGameLayer.hpp"
#include "../FineOutline.hpp"

void MyGJBaseGameLayer::createPlayer() {
    GJBaseGameLayer::createPlayer();

    alpha::fine_outline::impl::setOutlineColor(m_player1, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
    alpha::fine_outline::impl::setOutlineColor(m_player2, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::TWO));
}