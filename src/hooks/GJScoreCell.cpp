#include "GJScoreCell.hpp"
#include "../FineOutline.hpp"
#include <hiimjasmine00.user_data_api/include/Events.hpp>
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>

void MyGJScoreCell::loadFromScore(GJUserScore* userScore) {
    GJScoreCell::loadFromScore(userScore);

    auto player = typeinfo_cast<SimplePlayer*>(getChildByIDRecursive("player-icon"));
    if (!player) return;

    if (userScore->m_accountID == GJAccountManager::get()->m_accountID) {
        alpha::fine_outline::impl::setOutlineColor(player, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
    }
    else {
        user_data::handleScoreCell(this, [this, player] (GJUserScore* score) {
            alpha::fine_outline::impl::setOutlineColorOnline("player-1-color", score, player);
        });
    }
}