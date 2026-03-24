#include "GJRequestCell.hpp"
#include "../FineOutline.hpp"
#include <hiimjasmine00.user_data_api/include/Events.hpp>
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>

void MyGJRequestCell::loadFromScore(GJUserScore* userScore) {
    GJRequestCell::loadFromScore(userScore);

    auto player = typeinfo_cast<SimplePlayer*>(getChildByIDRecursive("player-icon"));
    if (!player) return;

    user_data::handleRequestCell(this, [this, player] (GJUserScore* score) {
        alpha::fine_outline::impl::setOutlineColorOnline("player-1-color", score, player);
    });
}