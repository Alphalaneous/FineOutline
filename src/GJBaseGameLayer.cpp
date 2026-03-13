#include "GJBaseGameLayer.hpp"
#include "PlayerObject.hpp"
#include "Utils.hpp"

void MyGJBaseGameLayer::createPlayer() {
    GJBaseGameLayer::createPlayer();

    auto p1 = static_cast<MyPlayerObject*>(m_player1);
    auto p2 = static_cast<MyPlayerObject*>(m_player2);

    p1->setOutlineColor(alpha::fine_outline::getP1Color());
    p2->setOutlineColor(alpha::fine_outline::getP2Color());
}