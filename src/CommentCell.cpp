#include "CommentCell.hpp"
#include "../include/FineOutline.hpp"

void MyCommentCell::loadFromComment(GJComment* p0) {
    CommentCell::loadFromComment(p0);

    if (p0->m_accountID != GJAccountManager::get()->m_accountID) return;

    if (auto player = typeinfo_cast<SimplePlayer*>(getChildByIDRecursive("player-icon"))) {
        alpha::fine_outline::setOutlineColorS(player, alpha::fine_outline::getColor(alpha::fine_outline::PlayerIcon::ONE));
    }
}