#include "CommentCell.hpp"
#include "../FineOutline.hpp"

void MyCommentCell::loadFromComment(GJComment* p0) {
    CommentCell::loadFromComment(p0);

    if (p0->m_accountID != GJAccountManager::get()->m_accountID) return;

    if (auto player = typeinfo_cast<SimplePlayer*>(getChildByIDRecursive("player-icon"))) {
        alpha::fine_outline::impl::setOutlineColor(player, alpha::fine_outline::impl::getColor(alpha::fine_outline::PlayerIcon::ONE));
    }
}