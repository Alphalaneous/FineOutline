#include "CommentCell.hpp"
#include "SimplePlayer.hpp"

void MyCommentCell::loadFromComment(GJComment* p0) {
    CommentCell::loadFromComment(p0);

    if (p0->m_accountID != GJAccountManager::get()->m_accountID) return;

    if (auto player = typeinfo_cast<SimplePlayer*>(getChildByIDRecursive("player-icon"))) {
        if (Mod::get()->getSavedValue<bool>("override-color")) {
            static_cast<MySimplePlayer*>(player)->setOutlineColor(Mod::get()->getSavedValue<ccColor3B>("p1-color"), false);
        }
        else {
            static_cast<MySimplePlayer*>(player)->setOutlineColor(GameManager::get()->colorForIdx(Mod::get()->getSavedValue<int64_t>("outline-color-p1")), false);
        }
    }
}