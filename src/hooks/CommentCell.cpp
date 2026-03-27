#include "CommentCell.hpp"
#include "../FineOutline.hpp"
#include <hiimjasmine00.user_data_api/include/Events.hpp>
#include <hiimjasmine00.user_data_api/include/UserDataAPI.hpp>

void MyCommentCell::loadFromComment(GJComment* comment) {
    CommentCell::loadFromComment(comment);

    if (!comment) return;

    auto player = typeinfo_cast<SimplePlayer*>(getChildByIDRecursive("player-icon"));
    if (!player) return;

    if (comment->m_accountID == GJAccountManager::get()->m_accountID) {
        alpha::fine_outline::impl::setOutlineColor(player, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
    }
    else {
        user_data::handleCommentCell(this, [player = Ref(player)] (GJComment* comment) {
            if (!player || !comment) return;
            alpha::fine_outline::impl::setOutlineColorOnline("player-1-color", comment, player);
        });
    }
}