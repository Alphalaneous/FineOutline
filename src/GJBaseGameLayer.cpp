#include "GJBaseGameLayer.hpp"
#include "../include/FineOutline.hpp"

void MyGJBaseGameLayer::createPlayer() {
    GJBaseGameLayer::createPlayer();

    alpha::fine_outline::setOutlineColorP(m_player1, alpha::fine_outline::getColor(alpha::fine_outline::PlayerIcon::ONE));
    alpha::fine_outline::setOutlineColorP(m_player2, alpha::fine_outline::getColor(alpha::fine_outline::PlayerIcon::TWO));
}